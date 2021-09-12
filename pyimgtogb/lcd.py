import math


def correct_curve(x):
    return 1 - (math.cos(x * math.pi) / 2 + 0.5)


def emulate_lcd(c):
    r = (c & 0x1F) / 31
    g = ((c >> 5) & 0x1F) / 31
    b = ((c >> 10) & 0x1F) / 31

    r, g, b = correct_curve(r), correct_curve(g), correct_curve(b)

    g = (g * 3 + b) / 4

    r, g, b = round(r*31), round(g*31), round(b*31)

    return (r, g, b)


def build_lcd_map():
    return [emulate_lcd(c) for c in range(2**15)]


def find_best(c, lcd_map):
    r = (c & 0x1F)
    g = ((c >> 5) & 0x1F)
    b = ((c >> 10) & 0x1F)

    best_dist = 0
    best_color = None

    for c2 in lcd_map:
        dist = (r - c2[0])**2 + (g - c2[1])**2 + (b - c2[2])**2
        if best_color == None or dist < best_dist:
            best_color = c2
            best_dist = dist

    return best_color[0] + (best_color[1] << 5) + (best_color[2] << 10)
