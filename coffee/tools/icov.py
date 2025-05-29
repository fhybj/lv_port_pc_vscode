import os
from PIL import Image

def png_to_raw_argb8888_bin(png_path, output_bin_path):
    """
    将PNG图片转换为纯ARGB8888格式的BIN文件。
    假定LVGL在小端序系统上，ARGB8888内存布局为 BB GG RR AA。
    
    Args:
        png_path (str): 输入PNG图片的路径。
        output_bin_path (str): 输出BIN文件的路径。
    """
    try:
        img = Image.open(png_path).convert('RGBA') # 转换为RGBA模式
        width, height = img.size
        
        print(f"Converting '{png_path}' ({width}x{height}) to raw ARGB8888 BIN...")
        
        rgba_bytes = img.tobytes() # 获取原始RGBA字节数据 (R, G, B, A, R, G, B, A, ...)
        
        argb_bytes = bytearray(width * height * 4) # 预分配空间给ARGB8888
        
        # 将RGBA (R G B A) 转换为 ABGR (B G R A) 字节序
        # 对于小端序系统，存储时是低字节在前，所以为了在LVGL中读出ARGB8888，
        # 我们要写入的是 B G R A，因为 LVGL 的 ARGB8888 通常是 0xAARRGGBB，
        # 在小端序内存中是 BB GG RR AA。
        for i in range(0, len(rgba_bytes), 4):
            r = rgba_bytes[i]
            g = rgba_bytes[i+1]
            b = rgba_bytes[i+2]
            a = rgba_bytes[i+3]
            
            # 存储为小端序的 BB GG RR AA
            argb_bytes[i+0] = b # Blue component
            argb_bytes[i+1] = g # Green component
            argb_bytes[i+2] = r # Red component
            argb_bytes[i+3] = a # Alpha component
            
        with open(output_bin_path, 'wb') as f:
            f.write(argb_bytes)
            
        print(f"Successfully converted and saved to '{output_bin_path}'.")
        print(f"File size: {os.path.getsize(output_bin_path)} bytes.")

    except FileNotFoundError:
        print(f"Error: PNG file not found at '{png_path}'.")
    except Exception as e:
        print(f"An error occurred: {e}")

# --- 使用示例 ---
if __name__ == "__main__":
    # 确保你有一个名为 'input.png' 的PNG图片文件在同一个目录下
    # 或者提供一个完整的路径
    input_png_file = 'input.png'  # 替换为你的PNG文件路径
    output_bin_file = 'output.bin'

    # 创建一个简单的10x10红色带半透明的PNG图片用于测试
    # from PIL import ImageDraw
    # test_img = Image.new('RGBA', (10, 10), (255, 0, 0, 128)) # 半透明红色
    # test_img.save(input_png_file)
    # print(f"Test PNG '{input_png_file}' created.")

    # 调用转换函数
    png_to_raw_argb8888_bin(input_png_file, output_bin_file)
