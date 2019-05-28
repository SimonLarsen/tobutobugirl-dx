#!/usr/bin/env python3
import argparse
import re


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("infile", help="Code file.", type=str)
    parser.add_argument("outfile", help="Outfile file.", type=str)
    args = parser.parse_args()

    infile = open(args.infile, "r")
    lines = infile.readlines()
    infile.close()

    outfile = open(args.outfile, "w")
    for line in lines:
        skip = False

        if re.match("^\s*\.optsdcc", line):
            line = "; " + line

        if re.match("^\s*;", line) and len(line) > 128:
            line = line[0:127] + "\n"
        skip = skip or re.match("^\s*.area\s+_CABS", line) != None
        skip = skip or re.match("^\s*.area\s+_DABS", line) != None

        if not skip:
            outfile.write(line)

    outfile.close()


if __name__ == "__main__":
    main()
