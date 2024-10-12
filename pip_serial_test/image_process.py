from PIL import Image

def resize_and_pad(image, size=(480, 320), color=(0, 0, 0)):
    """
    Resize and pad the image to fit into the given size without changing proportions.
    """
    img_ratio = image.width / image.height
    target_ratio = size[0] / size[1]

    if img_ratio > target_ratio:
        # Image is wider than target, adjust height and pad vertically
        new_width = size[0]
        new_height = int(size[0] / img_ratio)
    else:
        # Image is taller than target, adjust width and pad horizontally
        new_height = size[1]
        new_width = int(size[1] * img_ratio)

    resized_img = image.resize((new_width, new_height), Image.Resampling.LANCZOS)

    # Create new image with the target size and background color
    new_image = Image.new('RGB', size, color)

    # Calculate top-left position to paste the resized image onto the new image
    paste_x = (size[0] - new_width) // 2
    paste_y = (size[1] - new_height) // 2

    # Paste the resized image onto the new image
    new_image.paste(resized_img, (paste_x, paste_y))
    return new_image

def rgb888_to_rgb565(r, g, b):
    """
    Convert 24-bit RGB (888) to 16-bit RGB (565).
    """
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3)

def split_into_bytes(rgb565_value):
    """
    Split a 16-bit RGB565 value into two 8-bit values (high byte and low byte).
    """
    high_byte = (rgb565_value >> 8) & 0xFF  # Старший байт
    low_byte = rgb565_value & 0xFF          # Младший байт
    return high_byte, low_byte

def image_to_rgb565_bytes(image_path):
    """
    Convert image to a 480x320 pixel array in RGB565 format, split into two bytes per pixel.
    """
    image = Image.open(image_path)
    image = resize_and_pad(image)
    image = image.convert('RGB')  # Ensure image is in RGB mode
    
    pixel_data = list(image.getdata())
    byte_array = []
    
    for i, (r, g, b) in enumerate(pixel_data):
        rgb565_value = rgb888_to_rgb565(r, g, b)
        
        # Debug output to ensure correct pixel conversion
        #print(f"Pixel {i}: RGB888({r}, {g}, {b}) -> RGB565({rgb565_value:#06X})")
        
        high_byte, low_byte = split_into_bytes(rgb565_value)
        
        # More debug output for byte splitting
        #print(f"RGB565({rgb565_value:#06X}) -> High byte: {high_byte:#04X}, Low byte: {low_byte:#04X}")
        
        byte_array.append(high_byte)
        byte_array.append(low_byte)

    return byte_array

