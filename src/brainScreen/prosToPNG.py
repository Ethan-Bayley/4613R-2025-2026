from PIL import Image, ImageDraw

INPUT_IMAGE = "src/brainScreen/image.png"
OUTPUT_CPP = "src/brainScreen/drawLogo.cpp"
OUTPUT_SIM = "src/brainScreen/simulatedImage.png"

WIDTH = 480
HEIGHT = 240

RED = (200, 33, 40)
WHITE = (255, 255, 255)

def is_white(pixel, threshold=200):
    r, g, b = pixel
    return r >= threshold and g >= threshold and b >= threshold

img = Image.open(INPUT_IMAGE).convert("RGB")
img = img.resize((WIDTH, HEIGHT), Image.LANCZOS)
px = img.load()

# Build boolean grid of white pixels
grid = [[is_white(px[x, y]) for x in range(WIDTH)] for y in range(HEIGHT)]

# Maximal rectangle merging using histogram method
def merge_rects(grid, h, w):
    rects = []
    remaining = [row[:] for row in grid]

    for y in range(h):
        for x in range(w):
            if not remaining[y][x]:
                continue

            # Find max width starting at (x, y)
            max_x = x
            while max_x + 1 < w and remaining[y][max_x + 1]:
                max_x += 1

            # Extend downward as far as the full width holds
            max_y = y
            while max_y + 1 < h and all(remaining[max_y + 1][cx] for cx in range(x, max_x + 1)):
                max_y += 1

            # Record and clear this rectangle
            rects.append((x, y, max_x, max_y))
            for ry in range(y, max_y + 1):
                for rx in range(x, max_x + 1):
                    remaining[ry][rx] = False

    return rects

rects = merge_rects(grid, HEIGHT, WIDTH)

# Write PROS C++ code
with open(OUTPUT_CPP, "w", encoding="utf-8") as f:
    f.write('#include "main.h"\n')
    f.write("// Auto-generated PROS Brain screen code\n\n")
    f.write("void drawImage() {\n")
    f.write("  pros::screen::set_pen(0xFFC82128);\n")
    f.write("  pros::screen::fill_rect(0, 0, 479, 239);\n\n")
    f.write("  pros::screen::set_pen(0xFFFFFFFF);\n")
    for x0, y0, x1, y1 in rects:
        f.write(f"  pros::screen::fill_rect({x0}, {y0}, {x1}, {y1});\n")
    f.write("}\n")

print(f"Written {len(rects)} rectangles to {OUTPUT_CPP}")

# Simulated output
sim = Image.new("RGB", (WIDTH, HEIGHT), RED)
draw = ImageDraw.Draw(sim)
for x0, y0, x1, y1 in rects:
    draw.rectangle([x0, y0, x1, y1], fill=WHITE)
sim.save(OUTPUT_SIM)
print(f"Simulated image saved to {OUTPUT_SIM}")