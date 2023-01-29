#!/usr/bin/env python3
import os
import argparse
import png
import numpy as np
from string import Template
from colors import rgb_to_5bit
from export import pretty_data
import lcd


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="Image file.", type=str)
    parser.add_argument("outfile", help="Output file.", type=str) 
    parser.add_argument("-b","--bytes", help="Output bytes instead of words.", action="store_true")
    parser.add_argument("-l","--correct_lcd", help="Correct colors for GBC LCD.", action="store_true")
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

    if args.correct_lcd:
        lcd_map = lcd.build_lcd_map()
        out = [lcd.find_best(c, lcd_map) for c in out]

    datatype = "int"
    pdlen = len(out) // 4

    if args.bytes:
        datatype = "char"
        out2 = []
        for x in out:
            out2.append(x & 0xFF)
            out2.append(x >> 8)
        out = out2

    name = os.path.splitext(os.path.basename(args.outfile))[0]

    s = Template("""#ifndef ${uname}_PALETTE_H
#define ${uname}_PALETTE_H

#define ${name}_palette_data_length ${pdlen}U
const unsigned ${datatype} ${name}_palette_data[] = {
    ${data}
};

#endif
""").substitute(
        name=name,
        uname=name.upper(),
        pdlen=pdlen,
        datatype=datatype,
        data=pretty_data(out, 4)
    )

    with open(args.outfile, "w") as f:
        f.write(s)

if __name__ == "__main__":
    main()
