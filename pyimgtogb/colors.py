def rgb_to_5bit(r, g, b):
    r = round(r  / 255 * 31)
    g = round(g  / 255 * 31)
    b = round(b  / 255 * 31)
    return r + (g << 5) + (b << 10)
