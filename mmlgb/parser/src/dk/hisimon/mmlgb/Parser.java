package dk.hisimon.mmlgb;

import java.util.List;
import java.util.ArrayList;
import java.util.HashMap;

public class Parser {
	private List<Lexer.Token> tokens;
	private Song song, song_root;

	private int position;
    private boolean in_macro;
	private Lexer.Token next;

	private static final String[] CHANNEL_NAMES = {"A","B","C","D"};
	private static final String[] NOTE_NAMES = {"c","cs","d","ds","e","f","fs","g","gs","a","as","b"};
	private static final int[] WAVE_VOLUMES = {0, 96, 64, 32};

	private static final int BEAT_STEPS = 48;
	private static final int BAR_STEPS = 4*BEAT_STEPS;
	private static final int TIMA_SPEED = 4096;

	public Parser(List<Lexer.Token> tokens) {
		this.tokens = tokens;
		song = new Song();
        song_root = song;
		position = 0;
        in_macro = false;
		next = tokens.get(0);
	}

	public Song parse() throws ParserException {
		while(next.type != Lexer.TokenType.EOF) {
			if(next.type == Lexer.TokenType.CHANNEL) {
				parseChannel();
			}
			else if(next.type == Lexer.TokenType.MACRO) {
				parseDefinition();
			}
			else if(next.type == Lexer.TokenType.NEWLINE) {
				eat();
			}
			else {
				throw new ParserException(String.format("Unexpected token %s.", next), next);
			}
		}

		for(int i = 0; i < 4; ++i) {
			song.addData(i, Song.CMD.T_EOF.ordinal());
		}

		return song;
	}

	private void parseDefinition() throws ParserException {
		if(next.data.equals("@wave")) {
			parseWaveData();
		} 
		else if(next.data.equals("@@")) {
			parseMacroData();
		}
		else {
			throw new ParserException(String.format("Unexpected token %s.", next), next);
		}
	}

	private void parseWaveData() throws ParserException {
		eat();

		// Parse data id
		if(next.type != Lexer.TokenType.NUMBER) {
			throw new ParserException("Expected wave data id.", next);
		}
		int id = parseInt(next.data);
		if(id < 0) {
			throw new ParserException("Invalid wave data id. Must be nonnegative integer.", next);
		}
		eat();

		eat(Lexer.TokenType.ASSIGN, "=");
		eat(Lexer.TokenType.LCURLY, "{");

		// Parse samples
        List<Integer> samples = new ArrayList<Integer>(32);
		for(int i = 0; i < 32; ++i) {
			while(next.type == Lexer.TokenType.NEWLINE) eat();
			if(next.type != Lexer.TokenType.NUMBER) {
				throw new ParserException("Invalid wave sample. Expected number.", next);
			}
			int sample = parseInt(next.data);
			eat();
			if(sample < 0 || sample > 15) {
				throw new ParserException(String.format("Invalid wave sample %d. Expected 0-15.", sample), next);
			}
			samples.add(sample);

			while(next.type == Lexer.TokenType.NEWLINE) eat();
		}

		eat(Lexer.TokenType.RCURLY, "}");
		eat(Lexer.TokenType.NEWLINE, "Line break");

		song.addWaveData(id, samples);
	}

	private void parseMacroData() throws ParserException {
		eat();

		if(next.type != Lexer.TokenType.NUMBER) {
			throw new ParserException("Expected macro id.", next);
		}

		int id = parseInt(next.data);
		if(id < 0) {
			throw new ParserException("Invalid macro id. Must be nonnegative integer.", next);
		}
		eat();

		eat(Lexer.TokenType.ASSIGN, "=");
		eat(Lexer.TokenType.LCURLY, "{");

		ArrayList<Lexer.Token> macro = new ArrayList<Lexer.Token>();
		while(next.type != Lexer.TokenType.RCURLY) {
            while(next.type == Lexer.TokenType.NEWLINE) eat();

			macro.add(new Lexer.Token(next));
			eat();

            while(next.type == Lexer.TokenType.NEWLINE) eat();
		}
        macro.add(new Lexer.Token(Lexer.TokenType.NEWLINE, "\n"));

		eat(Lexer.TokenType.RCURLY, "}");
		eat(Lexer.TokenType.NEWLINE, "Line break");

        List<Lexer.Token> tokens_bak = tokens;
        song = new Song();
        int position_bak = position;

        tokens = macro;
        position = 0;
        in_macro = true;
        next = tokens.get(0);

        boolean active[] = {true, false, false, false};
        parseCommands(active);
        List<Integer> macro_data = song.getChannel(0);

        tokens = tokens_bak;
        song = song_root;
        position = position_bak;
        in_macro = false;
        next = tokens.get(position);

        song.addMacroData(id, macro_data);
	}

    private void parseChannel() throws ParserException {
		boolean active[] = new boolean[4];
		// Parse active channels
		while(next.type == Lexer.TokenType.CHANNEL) {
			for(int i = 0; i < 4; ++i) {
				if(next.data.equals(CHANNEL_NAMES[i])) {
					active[i] = true;
					break;
				}
			}
			eat();
		}

        parseCommands(active);
    }

	private void parseCommands(boolean active[]) throws ParserException {
		// Parse commands
		while(next.type != Lexer.TokenType.NEWLINE) {
			if(next.type == Lexer.TokenType.NOTE) {
				final int FIRST_NOTE = Song.CMD.T_C.ordinal();
				final int LAST_NOTE = Song.CMD.T_B.ordinal();

				int note = 0;

				// Read note
				for(int i = 0; i < 12; ++i) {
					if(next.data.equals(NOTE_NAMES[i])) {
						note = FIRST_NOTE + i;
						break;
					}
				}
				eat();

				// Sharp or flat
				if(next.type == Lexer.TokenType.SHARP) {
					note++;
					eat();
				}
				else if(next.type == Lexer.TokenType.DASH) {
					note--;
					eat();
				}
				if(note == FIRST_NOTE-1) note = LAST_NOTE;
				else if(note == LAST_NOTE+1) note = FIRST_NOTE;

				int length = parseLength(false);

				// Emit data
				if(length == 0) {
					song.addData(active, note);
				} else {
					song.addData(active, note | 0x80);
					song.addData(active, length % 255);
					for(int i = 0; i < length / 255; ++i) {
						song.addData(active, Song.CMD.T_WAIT.ordinal() | 0x80);
						song.addData(active, 255);
					}
				}
			}
			else if(next.type == Lexer.TokenType.COMMAND) {
				if(next.data.equals("r")) {
					eat();

					int length = parseLength(false);

					if(length == 0) {
						song.addData(active, Song.CMD.T_REST.ordinal());
					} else {
						song.addData(active, Song.CMD.T_REST.ordinal() | 0x80);
						song.addData(active, length);
						for(int i = 0; i < length / 255; ++i) {
							song.addData(active, Song.CMD.T_WAIT.ordinal() | 0x80);
							song.addData(active, 255);
						}
					}
				}
				else if(next.data.equals("w")) {
					eat();

					int length = parseLength(false);

					if(length == 0) {
						song.addData(active, Song.CMD.T_WAIT.ordinal());
					} else {
						song.addData(active, Song.CMD.T_WAIT.ordinal() | 0x80);
						song.addData(active, length);
						for(int i = 0; i < length / 255; ++i) {
							song.addData(active, Song.CMD.T_WAIT.ordinal() | 0x80);
							song.addData(active, 255);
						}
					}
				}
				else if(next.data.equals("o")) {
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected number after octave command.", next);
					}

					int octave = parseInt(next.data);
					eat();

					song.addData(active, Song.CMD.T_OCTAVE.ordinal());
					song.addData(active, octave);
				}
				else if(next.data.equals("<")) {
					eat();
					song.addData(active, Song.CMD.T_OCT_DOWN.ordinal());
				}
				else if(next.data.equals(">")) {
					eat();
					song.addData(active, Song.CMD.T_OCT_UP.ordinal());
				}
				else if(next.data.equals("l")) {
					eat();

					Lexer.Token length_token = next;
					int length = parseLength(true);
					if(length > 255) {
						throw new ParserException("Length overflow. Lengths more than 255 frames not allowed for l command.", length_token);
					}

					song.addData(active, Song.CMD.T_LENGTH.ordinal());
					song.addData(active, length);
				}
				else if(next.data.equals("v")) {
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Invalid volume. Expected number.", next);
					}
					int volume = parseInt(next.data);
					if(active[2] && (volume < 0 || volume > 3)) {
						throw new ParserException("Invalid volume for wave channel. Expected 0-3.", next);
					}
					if(volume < 0 || volume > 15) {
						throw new ParserException("Invalid volume value. Expected 0-15.", next);
					}
					eat();

					for(int i = 0; i < 4; ++i) {
						if(active[i]) {
							if(i == 2) {
								song.addData(i, Song.CMD.T_VOL.ordinal());
								song.addData(i, WAVE_VOLUMES[volume]);
							} else {
								song.addData(i, Song.CMD.T_VOL.ordinal());
								song.addData(i, volume << 4);
							}
						}
					}
				}
				else if(next.data.equals("t")) {
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Invalid tempo command. Expected number.", next);
					}
					int bpm = parseInt(next.data);
					eat();

					float ups = (float)bpm / 60.0f * (float)BEAT_STEPS;
					int mod = (int)Math.round((float)TIMA_SPEED / ups);
					song.addData(active, Song.CMD.T_TEMPO.ordinal());
					song.addData(active, 255 - mod);
				}
				else if(next.data.equals("y")) {
					eat();

					boolean negative = false;
					if(next.type == Lexer.TokenType.DASH) {
						eat();
						negative = true;
					}

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Invalid panning command. Expected number.", next);
					}

					int pan = parseInt(next.data);
					eat();

					int val;
					if(pan == 0) {
						val = 17; // 00010001
					} else if(negative) {
						val = 16; // 00010000
					} else {
						val = 1;  // 00000001
					}

					song.addData(active, Song.CMD.T_PAN.ordinal());
					song.addData(active, val);
				}
				else if(next.data.equals("L")) {
					eat();

					song.addData(active, Song.CMD.T_LOOP.ordinal());
				}
			}
			else if(next.type == Lexer.TokenType.TIE) {
				// Parse dangling tie as wait command
				eat();

				int length = parseLength(false);

				if(length == 0) {
					song.addData(active, Song.CMD.T_WAIT.ordinal());
				} else {
					song.addData(active, Song.CMD.T_WAIT.ordinal() | 0x80);
					song.addData(active, length);
				}
			}
			else if(next.type == Lexer.TokenType.MACRO) {
				if(next.data.equals("@wave")) {
					if(active[0] || active[1] || active[3]) {
						throw new ParserException("@wave only allowed in channel 3.", next);
					}
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected wave data id.", next);
					}
					Integer id = song.getWaveIndex(parseInt(next.data));
                    if(id == null) throw new ParserException(String.format("Wave \"%s\" not defined.", next.data), next);
					eat();

					song.addData(active, Song.CMD.T_WAVE.ordinal());
					song.addData(active, id);
				}
				else if(next.data.equals("@ve")) {
					eat();

					boolean increasing = true;
					if(next.type == Lexer.TokenType.DASH) {
						eat();
						increasing = false;
					}

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Invalid volume envelope. Expected number.", next);
					}

					int envelope = parseInt(next.data);
					eat();
					if(envelope > 7) {
						throw new ParserException("Invalid volume envelope. Expected values from -7 to 7.", next);
					}

					if(increasing) {
						envelope = envelope | (1 << 3);
					}

					song.addData(active, Song.CMD.T_ENV.ordinal());
					song.addData(active, envelope);
				}
				else if(next.data.equals("@wd")) {
					if(active[2] || active[3]) {
						throw new ParserException("@wd only allowed in channel 1 and 2.", next);
					}

					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Invalid wave duty. Expected number.", next);
					}
					int duty = parseInt(next.data);
					if(duty < 0 || duty > 3) {
						throw new ParserException("Invalid wave duty. Expected values 0-3.", next);
					}
					eat();

					song.addData(active, Song.CMD.T_WAVEDUTY.ordinal());
					song.addData(active, duty << 6);
				}
				else if(next.data.equals("@p")) {
					if(active[2]) {
						throw new ParserException("@p only allowed in channel 1, 2 and 4.", next);
					}
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected speed after @p macro.", next);
					}

					int speed = parseInt(next.data);
					if(speed < 0 || speed > 128) {
						throw new ParserException(String.format("Invalid portamento speed. Expected 0-128.", speed), next);
					}
					eat();

					song.addData(active, Song.CMD.T_PORTAMENTO.ordinal());
					song.addData(active, speed);
				}
				else if(next.data.equals("@po")) {
					if(active[3]) {
						throw new ParserException("@po only allowed in channel 1, 2 and 3.", next);
					}
					eat();

					boolean negative = false;
					if(next.type == Lexer.TokenType.DASH) {
						negative = true;
						eat();
					}

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected number after @po macro.", next);
					}

					int offset = parseInt(next.data);
					if(offset < 0 || offset > 127) {
						throw new ParserException("Invalid pitch offset. Expacted values 0-127.", next);
					}
					eat();

					if(negative) offset = -offset;

					song.addData(active, Song.CMD.T_PITCH_OFFSET.ordinal());
					song.addData(active, offset + 128);
				}
				else if(next.data.equals("@v")) {
					if(active[2] || active[3]) {
						throw new ParserException("@v only allowed in channel 1 and 2.", next);
					}
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected vibrato speed after @v macro.", next);
					}
					int speed = parseInt(next.data);
					if(speed < 0 || speed > 15) {
						throw new ParserException("Invalid vibrato speed. Expected values 0-15.", next);
					}
					eat();

					eat(Lexer.TokenType.COMMA, "comma");

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected vibrato depth after @v macro.", next);
					}
					int depth = parseInt(next.data);
					if(depth < 0 || depth > 4) {
						throw new ParserException("Invalid vibrato depth. Expected values 0-4.", next);
					}
					eat();

					int delay = 0;
					if(next.type == Lexer.TokenType.COMMA) {
						eat();

						delay = parseLength(true);
					}

					if(delay > 0) {
						song.addData(active, Song.CMD.T_VIBRATO_DELAY.ordinal());
						song.addData(active, speed | (depth << 4));
						song.addData(active, delay);
					} else {
						song.addData(active, Song.CMD.T_VIBRATO.ordinal());
						song.addData(active, speed | (depth << 4));
					}
				} else if(next.data.equals("@ns")) {
					if(active[0] || active[1] || active[2]) {
						throw new ParserException("@ns only allowed in channel 4.", next);
					}
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected 0 or 1 after @ns macro.", next);
					}
					int state = parseInt(next.data);
					if(state < 0 || state > 1) {
						throw new ParserException("Expected 0 or 1 after @ns macro.", next);
					}
					eat();

					song.addData(active, Song.CMD.T_NOISE_STEP.ordinal());
					song.addData(active, state);
				} else if(next.data.equals("@@")) {
					eat();

					if(next.type != Lexer.TokenType.NUMBER) {
						throw new ParserException("Expected macro id.", next);
					}

					Integer id = parseInt(next.data);
					if(id == null) {
						throw new ParserException(String.format("Macro @@%d not found.", id), next);
					}
					eat();

                    if(in_macro) {
                        List<Integer> macro_data = song_root.getMacroData(id);
                        song.addData(active, macro_data);
                    } else {
                        song.addData(active, Song.CMD.T_MACRO.ordinal());
                        song.addData(active, song_root.getMacroIndex(id));
                    }
				}
			}
			else if(next.type == Lexer.TokenType.LBRACKET) {
				eat();

				song.addData(active, Song.CMD.T_REP_START.ordinal());
			}
			else if(next.type == Lexer.TokenType.RBRACKET) {
				eat();

				if(next.type != Lexer.TokenType.NUMBER) {
					throw new ParserException("Expected repetition count.", next);
				}
				int reps = parseInt(next.data);
				if(reps < 2) {
					throw new ParserException("Invalid repetition count. Must be >= 2.", next);
				}
				eat();

				song.addData(active, Song.CMD.T_REP_END.ordinal());
				song.addData(active, reps);
			}
			else if(next.type == Lexer.TokenType.EOF) {
				break;
			}
			else {
				throw new ParserException(String.format("Unexpected token %s.", next), next);
			}
		}
	}

	private int parseLength(boolean required) throws ParserException {
		int length = 0;
		// Length
		if(next.type == Lexer.TokenType.NUMBER) {
			length = parseInt(next.data);
			if(length < 1 || length > BAR_STEPS) {
				throw new ParserException(String.format("Invalid note length %d. Expected 1-%d.", length, BAR_STEPS), next);
			}
			eat();

			// Check if BAR_STEPS is divisible by length
			if((int)(BAR_STEPS / length)*length != BAR_STEPS) {
				throw new ParserException(String.format("Invalid note length %d. Not enough precision.", length), next);
			}

			// Divide with note fraction.
			length = BAR_STEPS / length;

			// Add dots
			int dot = length / 2;
			while(next.type == Lexer.TokenType.DOT) {
				if(dot <= 0) {
					throw new ParserException("Too many dots in length. Not enough precision.", next);
				}
				eat();
				length += dot;
				dot = dot / 2;
			}
		} else if(next.type == Lexer.TokenType.ASSIGN) {
			eat();

			length = parseInt(next.data);
			if(length < 1 || length > 255) {
				throw new ParserException(String.format("Invalid note frame length %d. Expected 1-255.", length), next);
			}
			eat();
		} else if(required) {
			throw new ParserException("Expected note length.", next);
		}

		if(next.type == Lexer.TokenType.TIE) {
			eat();
			length += parseLength(true);
		}

		return length;
	}

	private void eat(Lexer.TokenType expected, String message) throws ParserException {
		if(next.type != expected) {
			throw new ParserException(
				String.format("Found token %s. Expected %s.", next.data, message), next
			);
		}

		eat();
	}

	private void eat() throws ParserException {
		position++;
		next = tokens.get(position);

		if(position >= tokens.size()) {
			throw new ParserException("End of file reached.", next);
		}
	}

	private static int parseInt(String s) {
		if(s.startsWith("-")) {
			return -parseInt(s.substring(1));
		}
		else if(s.startsWith("0x")) {
			return Integer.parseInt(s.substring(2), 16);
		}
		else if(s.startsWith("0b")) {
			return Integer.parseInt(s.substring(2), 2);
		}
		else {
			return Integer.parseInt(s);
		}
	}
}
