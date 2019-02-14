#!/usr/bin/env python3
import argparse
import png
import numpy as np

def rgb_to_5bit(r, g, b):
    r = round(r  / 255 * 31)
    g = round(g  / 255 * 31)
    b = round(b  / 255 * 31)
    return r + (g << 5) + (b << 10)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="Image file.", type=str)
    args = parser.parse_args()

    source = png.Reader(args.infile)
    width, height, data_map, meta = source.read()

    if width != 4:
        raise ValueError("Image must be 4 pixels wide.")
    if "palette" not in meta:
        raise ValueError("PNG image is not indexed.")

    data = np.array(list(data_map)).transpose()

    palette = meta["palette"]

    out = [rgb_to_5bit(*palette[data[x,y]]) for y in range(height) for x in range(4)]
    print(", ".join(map(str, out)))

if __name__ == "__main__":
    main()
