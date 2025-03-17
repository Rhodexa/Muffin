from PIL import Image

def image_to_st7565_bitmap(image_path):
    # Open the image and convert to monochrome (1-bit color)
    img = Image.open(image_path).convert('1')
    
    # Ensure image is 128x64
    img = img.resize((128, 64))
    pixels = img.load()
    
    # Prepare byte array
    bitmap = []
    
    for page in range(8):  # 8 pages, each 8 pixels high
        for x in range(128):
            byte = 0
            for bit in range(8):
                y = page * 8 + bit
                if pixels[x, y] == 0:  # Black pixel
                    byte |= (1 << bit)
            bitmap.append(byte)
    
    return bitmap

def save_bitmap_as_c_array(bitmap, output_path):
    with open(output_path, 'w') as f:
        f.write("#include <stdint.h>\n\n")
        f.write("const uint8_t bitmap[1024] = {\n")
        for i, byte in enumerate(bitmap):
            f.write(f" 0x{byte:02X},")
            if (i + 1) % 16 == 0:
                f.write("\n")
        f.write("\n};\n")
    print(f"C array saved to {output_path}")

if __name__ == "__main__":
    import sys
    if len(sys.argv) != 3:
        print("Usage: python script.py <input_image> <output_c_file>")
        sys.exit(1)
    
    input_image = sys.argv[1]
    output_c_file = sys.argv[2]
    bitmap = image_to_st7565_bitmap(input_image)
    save_bitmap_as_c_array(bitmap, output_c_file)
