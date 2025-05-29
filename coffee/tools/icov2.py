import os
import struct
from PIL import Image

# 定义 LVGL 的颜色格式枚举值
# LV_COLOR_FORMAT_ARGB8888 (LittlevGL's equivalent of ARGB8888 in LVGL v8+)
# This value (27) is specific to LVGL's internal representation for ARGB8888.
# You might need to adjust this if your LVGL version or custom color format differs.
LV_COLOR_FORMAT_ARGB8888 = 5 # For LVGL v8+

def png_to_raw_argb8888_bytes(image_path):
    """
    将 PNG 图片转换为原始的 ARGB8888 字节数据。
    返回 (argb_bytes, width, height) 或 (None, -1, -1) 如果转换失败。
    """
    try:
        img = Image.open(image_path).convert("RGBA") # 确保是 RGBA 模式
        width, height = img.size
        
        # 获取原始像素数据
        pixel_data = list(img.getdata())
        
        # 转换为 ARGB8888 字节数组
        argb_bytes = bytearray()
        for r, g, b, a in pixel_data:
            # 组合成 ARGB 32位整数: AABBCCDD -> AA (alpha) BB (red) CC (green) DD (blue)
            argb_int = (a << 24) | (r << 16) | (g << 8) | b
            argb_bytes.extend(struct.pack('<I', argb_int)) # 打包为 little-endian
            
        return bytes(argb_bytes), width, height

    except Exception as e:
        print(f"Error converting {image_path}: {e}")
        return None, -1, -1

def create_packed_animation_bin(input_frame_dir, output_bin_path):
    """
    将目录中的 PNG 帧图片打包成一个 LVGL 动画的 BIN 文件。
    BIN 文件格式：
    - Magic Word (4 bytes: "ANIM")
    - LVGL img_header_t 值 (4 bytes: 打包了颜色格式、宽度、高度)
    - 帧数量 (2 bytes: uint16)
    - 每帧像素数据大小 (4 bytes: uint32)
    - 帧索引表 (每个帧一个条目，每个条目 8 bytes: offset(uint32), length(uint32))
    - 帧像素数据 (所有帧的原始像素数据，按顺序排列)
    """
    print(f"Packing animations from '{input_frame_dir}' to '{output_bin_path}'...")

    png_files = sorted([f for f in os.listdir(input_frame_dir) if f.lower().endswith('.png')])
    if not png_files:
        print(f"No PNG files found in '{input_frame_dir}'. Aborting.")
        return False

    frames_data = []
    frame_sizes = [] # 存储每帧的实际字节大小 (应与 pixel_data_size_per_frame 一致)
    
    img_width = -1
    img_height = -1
    pixel_data_size_per_frame = -1 # 初始化为 -1，表示尚未确定

    # 1. 遍历所有 PNG 文件，转换并收集数据
    actual_frame_count_temp = 0
    for i, f_name in enumerate(png_files):
        f_path = os.path.join(input_frame_dir, f_name)
        print(f"  Converting frame {i+1}/{len(png_files)}: {f_name}")
        
        argb_bytes, w, h = png_to_raw_argb8888_bytes(f_path)
        if argb_bytes is None:
            print(f"  Skipping {f_name} due to conversion error.")
            continue
        
        if img_width == -1: # 第一次循环时，确定动画的统一尺寸和每帧数据大小
            img_width = w
            img_height = h
            pixel_data_size_per_frame = w * h * 4 # ARGB8888 = 4 bytes per pixel
            print(f"  [PYTHON DEBUG] First frame dimensions: {img_width}x{img_height}.")
            print(f"  [PYTHON DEBUG] Initial pixel_data_size_per_frame calculated as: {pixel_data_size_per_frame}.")
        else: # 校验后续图片尺寸和数据大小是否一致
            if w != img_width or h != img_height:
                print(f"Error: Image '{f_name}' has inconsistent size {w}x{h}. Expected {img_width}x{img_height}. Aborting.")
                return False
            if len(argb_bytes) != pixel_data_size_per_frame:
                 print(f"Error: Image '{f_name}' has inconsistent data size {len(argb_bytes)}. Expected {pixel_data_size_per_frame}. Aborting.")
                 return False
                 
        frames_data.append(argb_bytes)
        frame_sizes.append(len(argb_bytes))
        actual_frame_count_temp += 1
    
    if not frames_data:
        print("No valid frames were converted. Output file will not be created.")
        return False

    actual_frame_count = actual_frame_count_temp
    if actual_frame_count == 0:
        print("No frames successfully processed for packing. Skipping output.")
        return False

    # --- ！！！核心调试输出！！！ ---
    # 在写入文件头之前，打印所有用于构造文件头的值，确认它们是否一致
    print(f"\n[PYTHON DEBUG] Values used for BIN file header construction:")
    print(f"  Final img_width: {img_width}")
    print(f"  Final img_height: {img_height}")
    print(f"  Final pixel_data_size_per_frame: {pixel_data_size_per_frame}")
    print(f"  Final actual_frame_count: {actual_frame_count}")

    # 2. 构建文件头
    # Reconstruct the 4-byte header value (packed as a little-endian uint32)
    # The bytes (byte0, byte1, byte2, byte3) are constructed from img_width and img_height
    # based on the C-side unpacking logic.

    # byte0: (cf & 0x1F) | ((always_zero & 0x07) << 5)
    # (LV_COLOR_FORMAT_ARGB8888 (0x1B) is 5 bits, always_zero (0) is 3 bits)
    byte0 = (LV_COLOR_FORMAT_ARGB8888 & 0x1F) | ((0 & 0x07) << 5) 

    # byte1: (reserved & 0x03) | (w_msbs << 2)
    # reserved (0) is 2 bits, w_msbs (width high 6 bits) is 6 bits
    # (img_width >> 5) & 0x3F gives w_msbs
    byte1 = ((0 & 0x03) << 0) | (((img_width >> 5) & 0x3F) << 2) # <-- 修正点：w_msbs 需要左移 2 位

    # byte2: (w_lsbs & 0x1F) | (h_msbs << 5)
    # w_lsbs (width low 5 bits) is 5 bits, h_msbs (height high 3 bits) is 3 bits
    # (img_width & 0x1F) gives w_lsbs
    # (img_height >> 8) & 0x07 gives h_msbs
    byte2 = ((img_width & 0x1F) << 0) | (((img_height >> 8) & 0x07) << 5) # <-- 修正点：h_msbs 需要左移 5 位

    # byte3: h_lsbs
    # h_lsbs (height low 8 bits) is 8 bits
    # (img_height & 0xFF) gives h_lsbs
    byte3 = (img_height & 0xFF)
    
    # 组合成最终的 32 位 little-endian 整数
    lv_img_header_val = (byte3 << 24) | (byte2 << 16) | (byte1 << 8) | byte0
    
    print(f"  [PYTHON DEBUG] lv_img_header_val (calculated from img_width/height): 0x{lv_img_header_val:08X}")
    print(f"  [PYTHON DEBUG] Raw bytes for lv_img_header_val: byte0=0x{byte0:02X}, byte1=0x{byte1:02X}, byte2=0x{byte2:02X}, byte3=0x{byte3:02X}")


    current_offset = 4 + 4 + 2 + 4 # File header fixed size: Magic + LVGL_Header_Val + Frame_Count + Pixel_Data_Size_Per_Frame
    current_offset += actual_frame_count * (4 + 4) # Add size of index table (offset + length for each frame)

    index_table_data = bytearray()
    for size in frame_sizes:
        index_table_data.extend(struct.pack('<I', current_offset)) # offset (little-endian unsigned int)
        index_table_data.extend(struct.pack('<I', size)) # length (little-endian unsigned int)
        current_offset += size # Update offset for next frame

    # 3. 写入 BIN 文件
    try:
        with open(output_bin_path, 'wb') as f:
            # 写入文件头
            f.write(b'ANIM') # Magic Word (4 bytes)
            f.write(struct.pack('<I', lv_img_header_val)) # Packed lv_img_header_t (4 bytes)
            f.write(struct.pack('<H', actual_frame_count)) # Frame Count (2 bytes)
            f.write(struct.pack('<I', pixel_data_size_per_frame)) # Pixel Data Size Per Frame (4 bytes)
            
            # 写入索引表
            f.write(index_table_data)
            
            # 写入所有帧的像素数据
            for frame_bytes in frames_data:
                f.write(frame_bytes)
        
        print(f"Successfully created packed animation file: {output_bin_path}")
        print(f"  Animation properties: {img_width}x{img_height}, {actual_frame_count} frames, {pixel_data_size_per_frame} bytes per frame.")
        return True

    except Exception as e:
        print(f"Error writing packed animation file: {e}")
        return False

if __name__ == "__main__":
    # 示例用法
    # 请根据您的实际路径修改 input_frame_dir 和 output_bin_path
    
    # 假设你的动画帧在当前脚本同级目录下的 'bootanim_frames' 文件夹
    input_frame_directory = os.path.join(os.path.dirname(__file__), 'bootanim_frames')
    output_bin_file = os.path.join(os.path.dirname(__file__), 'bootanim.bin')

    print(f"Attempting to pack frames from: {input_frame_directory}")
    print(f"Output will be written to: {output_bin_file}")

    if not os.path.exists(input_frame_directory):
        print(f"Error: Input directory '{input_frame_directory}' does not exist.")
        print("Please create the directory and place your PNG frames inside it,")
        print("or modify the 'input_frame_directory' variable in the script.")
    else:
        create_packed_animation_bin(input_frame_directory, output_bin_file)
