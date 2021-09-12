package dk.hisimon.mmlgb;

import java.util.List;
import java.util.ArrayList;
import java.util.regex.Pattern;
import java.util.regex.Matcher;

public  class Lexer {
	public static enum TokenType {
		COMMENT(";.*"),
		NUMBER("[0-9]+"),
		HEXNUMBER("0x[0-9]+"),
		BINNUMBER("0b[0-1]+"),
		CHANNEL("[ABCD]"),
		NOTE("[cdefgab]"),
		SHARP("[\\#\\+]"),
		DASH("-"),
		COMMAND("[rwo<>lvtysL]"),
		DOT("\\."),
		COMMA(","),
		TIE("\\^"),
		MACRO("(@@|@po|@p|@ns|@ve|@v|@wave|@wd)"),
		ASSIGN("="),
		LCURLY("\\{"),
		RCURLY("\\}"),
		LBRACKET("\\["),
		RBRACKET("\\]"),
		NEWLINE("\\n"),
		WHITESPACE("[ \\t\\f\\r]+"),
		EOF("");

		public final String pattern;

		private TokenType(String pattern) {
			this.pattern = pattern;
		}
	}

	public static class Token {
		public TokenType type;
		public String data;
		public int line, pos;

		public Token(TokenType type, String data) {
			this.type = type;
			this.data = data;
		}

		public Token(TokenType type, String data, int line, int pos) {
			this.type = type;
			this.data = data;
			this.line = line;
			this.pos = pos;
		}

		public Token(Token o) {
			this(o.type, o.data, o.line, o.pos);
		}

		public String toString() {
			return String.format("(%s, %s)", type.name(), data);
		}
	}

	public static List<Token> lex(String input) {
		List<Token> tokens = new ArrayList<Token>();

		StringBuilder patternsBuffer = new StringBuilder();
		for(TokenType tokenType : TokenType.values()) {
			patternsBuffer.append(String.format("|(?<%s>%s)", tokenType.name(), tokenType.pattern));
		}
		Pattern tokenPatterns = Pattern.compile(new String(patternsBuffer.substring(1)));

		int line = 1;
		int linepos = 0;
		Matcher matcher = tokenPatterns.matcher(input);
		while(matcher.find()) {
			int pos = matcher.start() - linepos + 1;
			if(matcher.group(TokenType.COMMENT.name()) != null) {
				continue;
			}
			else if(matcher.group(TokenType.NUMBER.name()) != null) {
				tokens.add(new Token(TokenType.NUMBER, matcher.group(TokenType.NUMBER.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.HEXNUMBER.name()) != null) {
				tokens.add(new Token(TokenType.HEXNUMBER, matcher.group(TokenType.HEXNUMBER.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.BINNUMBER.name()) != null) {
				tokens.add(new Token(TokenType.BINNUMBER, matcher.group(TokenType.BINNUMBER.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.CHANNEL.name()) != null) {
				tokens.add(new Token(TokenType.CHANNEL, matcher.group(TokenType.CHANNEL.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.NOTE.name()) != null) {
				tokens.add(new Token(TokenType.NOTE, matcher.group(TokenType.NOTE.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.SHARP.name()) != null) {
				tokens.add(new Token(TokenType.SHARP, matcher.group(TokenType.SHARP.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.DASH.name()) != null) {
				tokens.add(new Token(TokenType.DASH, matcher.group(TokenType.DASH.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.COMMAND.name()) != null) {
				tokens.add(new Token(TokenType.COMMAND, matcher.group(TokenType.COMMAND.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.DOT.name()) != null) {
				tokens.add(new Token(TokenType.DOT, matcher.group(TokenType.DOT.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.COMMA.name()) != null) {
				tokens.add(new Token(TokenType.COMMA, matcher.group(TokenType.COMMA.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.TIE.name()) != null) {
				tokens.add(new Token(TokenType.TIE, matcher.group(TokenType.TIE.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.MACRO.name()) != null) {
				tokens.add(new Token(TokenType.MACRO, matcher.group(TokenType.MACRO.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.NEWLINE.name()) != null) {
				tokens.add(new Token(TokenType.NEWLINE, matcher.group(TokenType.NEWLINE.name()), line, pos));
				line++;
				linepos = matcher.start() + 1;
				continue;
			}
			else if(matcher.group(TokenType.ASSIGN.name()) != null) {
				tokens.add(new Token(TokenType.ASSIGN, matcher.group(TokenType.ASSIGN.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.LCURLY.name()) != null) {
				tokens.add(new Token(TokenType.LCURLY, matcher.group(TokenType.LCURLY.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.RCURLY.name()) != null) {
				tokens.add(new Token(TokenType.RCURLY, matcher.group(TokenType.RCURLY.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.LBRACKET.name()) != null) {
				tokens.add(new Token(TokenType.LBRACKET, matcher.group(TokenType.LBRACKET.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.RBRACKET.name()) != null) {
				tokens.add(new Token(TokenType.RBRACKET, matcher.group(TokenType.RBRACKET.name()), line, pos));
				continue;
			}
			else if(matcher.group(TokenType.WHITESPACE.name()) != null) {
				continue;
			}
		}

		tokens.add(new Token(TokenType.EOF, "", line, 0));

		return tokens;
	}
}
