from __future__ import print_function, division
import colorsys

reds = []
greens = []
blues = []

for h in range(64):
    hh = h/64.0
    r, g, b = colorsys.hsv_to_rgb(hh, 1.0, 1.0)
    reds.append(int(r * 255))
    greens.append(int(g * 255))
    blues.append(int(b * 255))

print("uint8_t colours_saturated[][] PROGMEM = {")
print("    {")
print("    ", ", ".join([str(x) for x in reds]))
print("    }, {")
print("    ", ", ".join([str(x) for x in greens]))
print("    }, {")
print("    ", ", ".join([str(x) for x in blues]))
print("    }")
print("};")
