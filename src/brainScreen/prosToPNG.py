from PIL import Image, ImageDraw

INPUT_IMAGE = "src/brainScreen/image.png"
OUTPUT_CPP = "src/brainScreen/drawLogo.cpp"
OUTPUT_SIM = "src/brainScreen/simulatedImage.png"

WIDTH = 480
HEIGHT = 240

# Colors
RED = (200, 33, 40)
WHITE = (255, 255, 255)

def is_white(pixel, threshold=200):
    r, g, b = pixel
    return r >= threshold and g >= threshold and b >= threshold

img = Image.open(INPUT_IMAGE).convert("RGB")
img = img.resize((WIDTH, HEIGHT), Image.LANCZOS)
px = img.load()

# Detect horizontal runs of white pixels
runs_per_row = [[] for _ in range(HEIGHT)]
for y in range(HEIGHT):
    x = 0
    while x < WIDTH:
        if not is_white(px[x, y]):
            x += 1
            continue
        start_x = x
        while x < WIDTH and is_white(px[x, y]):
            x += 1
        runs_per_row[y].append((start_x, x - 1))

# Merge vertical rectangles
rects = []
prev_row = {}
for y, runs in enumerate(runs_per_row):
    current_row = {}
    for start_x, end_x in runs:
        key = (start_x, end_x)
        if key in prev_row:
            rect = prev_row[key]
            rect[3] = y
            current_row[key] = rect
        else:
            rect = [start_x, y, end_x, y]
            current_row[key] = rect
    for key, rect in prev_row.items():
        if key not in current_row:
            rects.append(rect)
    prev_row = current_row
for rect in prev_row.values():
    rects.append(rect)

# Write PROS C++ code
with open(OUTPUT_CPP, "w", encoding="utf-8") as f:
    f.write('#include "main.h"\n')
    f.write("// Auto-generated PROS Brain screen code\n")
    f.write("// Background red: #c82128, White foreground with vertical merging & anti-alias fix\n\n")
    f.write("void drawImage() {\n")
    f.write("  pros::screen::set_pen(0xFFC82128);\n")
    f.write("  pros::screen::fill_rect(0, 0, 479, 239);  // fill background\n\n")
    f.write("  pros::screen::set_pen(0xFFFFFFFF);  // white foreground\n")
    for x0, y0, x1, y1 in rects:
        f.write(f"  pros::screen::fill_rect({x0}, {y0}, {x1}, {y1});\n")
    f.write("}\n")

print(f"PROS Brain code written to {OUTPUT_CPP}")

# Generate simulated image
sim = Image.new("RGB", (WIDTH, HEIGHT), RED)
draw = ImageDraw.Draw(sim)
for x0, y0, x1, y1 in rects:
    draw.rectangle([x0, y0, x1, y1], fill=WHITE)
sim.save(OUTPUT_SIM)
print(f"Simulated image saved to {OUTPUT_SIM}")