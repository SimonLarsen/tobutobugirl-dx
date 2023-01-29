import os
import math
from string import Template
from rle import compress as rle_compress


def pretty_data(data, w=16):
    return ",\n    ".join([", ".join(map(lambda x: str(x).rjust(3), data[i:i+w])) for i in range(0, len(data), w)])


def split_data_parts(data, parts, chunk_size=16):
    part_size = math.ceil(len(data) / chunk_size / parts) * chunk_size
    return [data[(i*part_size) : ((i+1)*part_size)] for i in range(parts)]


def gen_sprites_data(name, tile_data, palettes, palette_data, rle):
    has_palettes = palettes != None

    tile_data_length = int(len(tile_data) / 16)
    palette_data_length = 0
    if has_palettes:
        palette_data_length = int(len(palette_data) / 4)

    if rle:
        tile_data = rle_compress(tile_data)
        if has_palettes:
            palettes = rle_compress(palettes)

    pal_defs = ""
    pal_data = ""

    if has_palettes:
        pal_defs = Template("""#define ${name}_palette_data_length ${pdlen}U""").substitute(
            name=name,
            pdlen=palette_data_length
        )

        pal_data = Template("""const unsigned int ${name}_palette_data[] = {
    ${palette_data}
};
const unsigned char ${name}_palettes[] = {
    ${palettes}
};""").substitute(
            name=name,
            palette_data=pretty_data(palette_data, 4),
            palettes=pretty_data(palettes)
        )

    spr_defs = Template("""#define ${name}_data_length ${datalength}U""").substitute(
        name=name,
        datalength=tile_data_length
    )

    spr_data = Template("""const unsigned char ${name}_data[] = {
    ${data}
};""").substitute(
        name=name,
        data=pretty_data(tile_data)
    )

    return spr_defs, spr_data, pal_defs, pal_data


def write_sprites_c_header(path, tile_data, palettes=None, palette_data=None, rle=False):
    name = os.path.splitext(os.path.basename(path))[0]

    spr_defs, spr_data, pal_defs, pal_data = gen_sprites_data(name, tile_data, palettes, palette_data, rle)

    s = Template("""#ifndef ${uname}_SPRITES_H
#define ${uname}_SPRITES_H
${spr_defs}
${spr_data}
${pal_defs}
${pal_data}
#endif
""").substitute(
        uname=name.upper(),
        spr_defs=spr_defs,
        spr_data=spr_data,
        pal_defs=pal_defs,
        pal_data=pal_data
    )

    with open(path, "w") as f:
        f.write(s)


def write_sprites_c_source(cpath, hpath, tile_data, palettes=None, palette_data=None, rle=False):
    name = os.path.splitext(os.path.basename(hpath))[0]

    spr_defs, spr_data, pal_defs, pal_data = gen_sprites_data(name, tile_data, palettes, palette_data, rle)

    has_palettes = palettes != None

    s_externs = "extern const unsigned char ${name}_data[];"

    if has_palettes:
        s_externs += "extern const unsigned char ${name}_palettes[];\n"
        s_externs += "extern const unsigned int ${name}_palette_data[];\n"

    s_header = Template("""#ifndef ${uname}_SPRITES_H
#define ${uname}_SPRITES_H
${spr_defs}
${pal_defs}
${externs}
#endif
""").substitute(
        uname=name.upper(),
        spr_defs=spr_defs,
        pal_defs=pal_defs,
        externs=Template(s_externs).substitute(name=name)
    )

    s_source = Template("""#include "${hpath}"
${spr_data}
${pal_data}
""").substitute(
        hpath=hpath,
        spr_data=spr_data,
        pal_data=pal_data
    )

    with open(cpath, "w") as f:
        f.write(s_source)

    with open(hpath, "w") as f:
        f.write(s_header)


def gen_map_data(name, tile_data, tiles, tiles_width, tiles_height, tiles_offset, split_data=1, split_tiles=1, palettes=None, palette_data=None, palette_offset=None, rle_data=False, rle_tiles=False):
    has_palettes = palettes != None

    # split tile data
    tile_data = split_data_parts(tile_data, split_data, 16)
    tile_data_length = [int(len(d) / 16) for d in tile_data]

    tiles = split_data_parts(tiles, split_tiles, 1)
    tiles_height = math.floor(tiles_height / split_tiles)
    
    palette_data_length = 0
    if has_palettes:
        palettes = split_data_parts(palettes, split_tiles, 1)
        palette_data_length = int(len(palette_data) / 4)

    if rle_data:
        tile_data = [rle_compress(d) for d in tile_data]

    if rle_tiles:
        tiles = [rle_compress(d) for d in tiles]
        if has_palettes:
            palettes = [rle_compress(d) for d in palettes]

    pal_defs = ""
    pal_data = ""

    if has_palettes:
        pal_defs = Template("""#define ${name}_palette_data_length ${pdlen}U
#define ${name}_palette_offset ${paloffset}U""").substitute(
            name=name,
            pdlen=palette_data_length,
            paloffset=palette_offset
        )

        pal_data = """const unsigned int {0}_palette_data[] = {{\n\t{1}\n}};\n""".format(name, pretty_data(palette_data, 4))

        for i in range(split_tiles):
            id = "" if i == 0 else str(i+1)
            pal_data += """const unsigned char {0}_palettes{1}[] = {{\n\t{2}\n}};\n""".format(name, id, pretty_data(palettes[i], 20))


    map_defs = ""
    for i in range(split_data):
        id = "" if i == 0 else str(i+1)
        map_defs += "#define {0}_data_length{1} {2}U\n".format(name, id, tile_data_length[i])

    map_defs += Template("""#define ${name}_tiles_width ${width}U
#define ${name}_tiles_height ${height}U
#define ${name}_tiles_offset ${offset}U""").substitute(
        name=name,
        length=tile_data_length,
        width=tiles_width,
        height=tiles_height,
        offset=tiles_offset
    )

    map_data = ""
    for i in range(split_data):
        id = "" if i == 0 else str(i+1)
        map_data += """const unsigned char {0}_data{1}[] = {{\n\t{2}\n}};\n""".format(name, id, pretty_data(tile_data[i]))

    for i in range(split_tiles):
        id = "" if i == 0 else str(i+1)
        map_data += """const unsigned char {0}_tiles{1}[] = {{\n\t{2}\n}};\n""".format(name, id, pretty_data(tiles[i], 20))

    return map_defs, map_data, pal_defs, pal_data

def write_map_c_header(path, tile_data, tiles, tiles_width, tiles_height, tiles_offset, split_data=1, split_tiles=1, palettes=None, palette_data=None, palette_offset=None, rle_data=False, rle_tiles=False):
    name = os.path.splitext(os.path.basename(path))[0]

    map_defs, map_data, pal_defs, pal_data = gen_map_data(name, tile_data, tiles, tiles_width, tiles_height, tiles_offset, split_data, split_tiles, palettes, palette_data, palette_offset, rle_data, rle_tiles)

    s = Template("""#ifndef ${uname}_MAP_H
#define ${uname}_MAP_H
${map_defs}
${map_data}
${pal_defs}
${pal_data}
#endif
""").substitute(
        uname=name.upper(),
        map_defs=map_defs,
        map_data=map_data,
        pal_defs=pal_defs,
        pal_data=pal_data
    )

    with open(path, "w") as f:
        f.write(s)


def write_map_c_source(cpath, hpath, tile_data, tiles, tiles_width, tiles_height, tiles_offset, split_data=1, split_tiles=1, palettes=None, palette_data=None, palette_offset=None, rle_data=False, rle_tiles=False):
    name = os.path.splitext(os.path.basename(hpath))[0]
    has_palettes = palettes != None

    map_defs, map_data, pal_defs, pal_data = gen_map_data(name, tile_data, tiles, tiles_width, tiles_height, tiles_offset, split_data, split_tiles, palettes, palette_data, palette_offset, rle_data, rle_tiles)

    s_externs = ""
    for i in range(split_data):
        id = "" if i == 0 else str(i+1)
        s_externs += "extern const unsigned char ${{name}}_data{0}[];\n".format(id)

    for i in range(split_tiles):
        id = "" if i == 0 else str(i+1)
        s_externs += "extern const unsigned char ${{name}}_tiles{0}[];\n".format(id)
        if has_palettes:
            s_externs += "extern const unsigned char ${{name}}_palettes{0}[];\n".format(id)

    if has_palettes:
        s_externs += "extern const unsigned int ${name}_palette_data[];\n"

    s_header = Template("""#ifndef ${uname}_MAP_H
#define ${uname}_MAP_H
${map_defs}
${pal_defs}
${externs}
#endif
""").substitute(
        uname=name.upper(),
        map_defs=map_defs,
        pal_defs=pal_defs,
        externs=Template(s_externs).substitute(name=name)
    )

    s_source = Template("""#include "${hpath}"
${map_data}
${pal_data}
""").substitute(
        hpath=hpath,
        map_data=map_data,
        pal_data=pal_data
    )

    with open(cpath, "w") as f:
        f.write(s_source)

    with open(hpath, "w") as f:
        f.write(s_header)


def write_border_c_header(path, tile_data, tiles, palettes, palette_data, rle=False):
    name = os.path.splitext(os.path.basename(path))[0]

    tile_data_length = len(tile_data) // 32
    palette_count = len(palette_data) // 16

    tile_data1 = tile_data[0:0x1000]
    tile_data2 = tile_data[0x1000:0x2000]

    if rle:
        tile_data1 = rle_compress(tile_data1)
        tile_data2 = rle_compress(tile_data2)
        tiles = rle_compress(tiles)
        palettes = rle_compress(palettes)

    s = Template("""#ifndef ${uname}_BORDER_H
#define ${uname}_BORDER_H
#define ${name}_data_length ${datalength}U
const unsigned char ${name}_data1[] = {
    ${data1}
};
const unsigned char ${name}_data2[] = {
    ${data2}
};
const unsigned char ${name}_tiles[] = {
    ${tiles}
};
const unsigned char ${name}_palettes[] = {
    ${palettes}
};
#define ${name}_num_palettes ${palettecount}U
const unsigned int ${name}_palette_data[] = {
    ${palettedata}
};
#endif
""").substitute(
        uname=name.upper(),
        name=name,
        datalength=tile_data_length,
        data1=pretty_data(tile_data1),
        data2=pretty_data(tile_data2),
        tiles=pretty_data(tiles, 32),
        palettes=pretty_data(palettes, 32),
        palettecount=palette_count,
        palettedata=pretty_data(palette_data)
    )

    with open(path, "w") as f:
        f.write(s)
