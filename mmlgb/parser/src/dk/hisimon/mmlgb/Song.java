package dk.hisimon.mmlgb;

import java.util.List;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;
import java.util.Arrays;
import java.util.Collection;

public class Song {
	private String filename;

	private List<List<Integer>> channel;

	private List<List<Integer>> waveData;
    private Map<Integer,Integer> waveMap;

    private List<List<Integer>> macroData;
    private Map<Integer,Integer> macroMap;

	public enum CMD {
		T_LENGTH,
		T_OCTAVE,
		T_OCT_UP,
		T_OCT_DOWN,
		T_VOL,
		T_ENV,
		T_WAVEDUTY,
		T_PAN,
		T_PORTAMENTO,
		T_VIBRATO,
		T_VIBRATO_DELAY,
		T_REP_START,
		T_REP_END,
		T_LOOP,
		T_PITCH_OFFSET,
		T_TEMPO,
		T_NOISE_STEP,
		T_WAVE,
        T_MACRO,
		T_EOF,
		T_C,
		T_Cs,
		T_D,
		T_Ds,
		T_E,
		T_F,
		T_Fs,
		T_G,
		T_Gs,
		T_A,
		T_As,
		T_B,
		T_REST,
		T_WAIT
	};

	public Song() {
		waveData = new ArrayList<List<Integer>>();
        waveMap = new HashMap<Integer,Integer>();

        macroData = new ArrayList<List<Integer>>();
        macroMap = new HashMap<Integer,Integer>();

		channel = new ArrayList<List<Integer>>();
		for(int i = 0; i < 4; ++i) {
			channel.add(new ArrayList<Integer>());
		}
	}

	public void addWaveData(int id, List<Integer> data) {
        waveMap.put(id, waveData.size());
        waveData.add(data);
	}

    public Integer getWaveIndex(int id) {
        return waveMap.get(id);
    }

    public void addMacroData(int id, List<Integer> data) {
        macroMap.put(id, macroData.size());
        macroData.add(data);
    }

    public List<Integer> getMacroData(int id) {
        return macroData.get(getMacroIndex(id));
    }

    public Integer getMacroIndex(int id) {
        return macroMap.get(id);
    }

	public void addData(int chan, int value) {
		channel.get(chan).add(value);
	}

    public void addData(int chan, Collection<Integer> values) {
        channel.get(chan).addAll(values);
    }

	public void addData(boolean[] active, int value) {
		for(int i = 0; i < 4; ++i) {
			if(active[i]) {
				addData(i, value);
			}
		}
	}

	public void addData(boolean[] active, Collection<Integer> values) {
		for(int i = 0; i < 4; ++i) {
			if(active[i]) {
				addData(i, values);
			}
		}
	}

    public List<Integer> getChannel(int i) {
        return channel.get(i);
    }

	public List<Integer> getData() {
		List<Integer> data = new ArrayList<Integer>();

        int wave_start = (4 + 1 + macroData.size()) * 2;
        int pos = wave_start + waveData.size() * 16;
        int[] macro_start = new int[macroData.size()];
        for(int i = 0; i < macro_start.length; ++i) {
            macro_start[i] = pos;
            pos += macroData.get(i).size() + 1;
        }

		// Generate header
		int c1Start = pos;
		int c2Start = c1Start + channel.get(0).size();
		int c3Start = c2Start + channel.get(1).size();
		int c4Start = c3Start + channel.get(2).size();

        // Channel data start values
		data.add(c1Start & 0xFF);
		data.add(c1Start >> 8);
		data.add(c2Start & 0xFF);
		data.add(c2Start >> 8);
		data.add(c3Start & 0xFF);
		data.add(c3Start >> 8);
		data.add(c4Start & 0xFF);
		data.add(c4Start >> 8);

        data.add(wave_start & 0xFF);
        data.add(wave_start >> 8);

        for(int i = 0; i < macro_start.length; ++i) {
            data.add(macro_start[i] & 0xFF);
            data.add(macro_start[i] >> 8);
        }

		// Generate wave data
		for(int i = 0; i < waveData.size(); ++i) {
			List<Integer> samples = waveData.get(i);
            for(int j = 0; j < 32; j += 2) {
                int value = (samples.get(j) << 4) | samples.get(j+1);
                data.add(value);
            }
		}

        // Generate macro data
        for(int i = 0; i < macroData.size(); ++i) {
            data.addAll(macroData.get(i));
            data.add(Song.CMD.T_EOF.ordinal());
        }

		// Output channel data
		for(int i = 0; i < 4; ++i) {
			data.addAll(channel.get(i));
		}

		return data;
	}

	public void setFileName(String name) {
		this.filename = name;
	}

	public String emitC() {
		String[] parts = filename.split("\\.");
		String id = parts[0];
		String id_upper = id.toUpperCase();
		StringBuilder sb = new StringBuilder();

		sb.append(String.format("#ifndef %s_H\n", id_upper));
		sb.append(String.format("#define %s_H\n", id_upper));

		sb.append(String.format("\nconst UBYTE %s_data[] = {\n", id));

		for(Integer i : getData()) {
			sb.append("\t" + i + "U,\n");
		}
		sb.append("};\n\n");

		sb.append("#endif\n");

		return sb.toString();
	}

	public String emitASM() {
		String[] parts = filename.split("\\.");
		String id = parts[0];
		String id_lower = id.toLowerCase();
		StringBuilder sb = new StringBuilder();

		sb.append(String.format(".globl _%s_data\n", id_lower));
		sb.append(String.format("_%s_data:\n", id_lower));

		for(Integer i : getData()) {
			sb.append(String.format("\t.db 0x%02X\n", i));
		}

		return sb.toString();
	}
}
