package dk.hisimon.mmlgb;

public class WaveData {
	private int[] data;

	public WaveData() {
		data = new int[32];
	}

	public WaveData(int[] data) {
		setData(data);
	}

	public int[] getData() {
		return data;
	}

	public void setData(int[] data) {
		if(data.length != 32) {
			throw new IllegalArgumentException(
				String.format("Array contains %d samples. Expected 32.", data.length)
			);
		}

		this.data = data;
	}
}
