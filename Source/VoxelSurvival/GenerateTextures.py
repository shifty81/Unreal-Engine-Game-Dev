#!/usr/bin/env python3
"""
Texture Generator for Unreal Engine Resource System
Generates procedural textures for resources: trees, rocks, ores
"""

import os
import sys
from PIL import Image, ImageDraw, ImageFilter
import random
import math

# Output directory
OUTPUT_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "Content", "Textures", "Resources")

def create_output_dir():
    """Create output directory if it doesn't exist"""
    os.makedirs(OUTPUT_DIR, exist_ok=True)
    print(f"Output directory: {OUTPUT_DIR}")

def generate_noise(width, height, scale=1.0):
    """Generate Perlin-like noise pattern"""
    noise_array = []
    for y in range(height):
        row = []
        for x in range(width):
            value = (math.sin(x * scale * 0.1) + math.cos(y * scale * 0.1)) * 127.5 + 127.5
            value += random.randint(-30, 30)
            value = max(0, min(255, int(value)))
            row.append(value)
        noise_array.append(row)
    return noise_array

def generate_wood_texture(name, width=512, height=512):
    """Generate wood bark texture"""
    img = Image.new('RGB', (width, height))
    pixels = img.load()
    
    # Base brown color
    base_r, base_g, base_b = 101, 67, 33
    
    # Create wood grain pattern
    for y in range(height):
        for x in range(width):
            # Vertical grain pattern
            grain = math.sin(x * 0.05 + random.uniform(-0.5, 0.5)) * 30
            
            # Add noise
            noise = random.randint(-20, 20)
            
            r = int(max(0, min(255, base_r + grain + noise)))
            g = int(max(0, min(255, base_g + grain * 0.8 + noise)))
            b = int(max(0, min(255, base_b + grain * 0.5 + noise)))
            
            pixels[x, y] = (r, g, b)
    
    # Apply blur for smoother texture
    img = img.filter(ImageFilter.GaussianBlur(radius=1))
    
    filepath = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(filepath)
    print(f"Generated: {filepath}")
    return filepath

def generate_leaves_texture(name, width=512, height=512):
    """Generate tree leaves texture"""
    img = Image.new('RGBA', (width, height))
    pixels = img.load()
    
    # Base green colors
    greens = [
        (34, 139, 34),   # Forest green
        (50, 205, 50),   # Lime green
        (0, 128, 0),     # Green
        (85, 107, 47),   # Dark olive green
    ]
    
    # Create leafy pattern
    for y in range(height):
        for x in range(width):
            # Random leaf shapes
            if random.random() < 0.6:
                base_color = random.choice(greens)
                noise = random.randint(-20, 20)
                r = max(0, min(255, base_color[0] + noise))
                g = max(0, min(255, base_color[1] + noise))
                b = max(0, min(255, base_color[2] + noise))
                alpha = random.randint(200, 255)
                pixels[x, y] = (r, g, b, alpha)
            else:
                # Gaps between leaves
                pixels[x, y] = (0, 0, 0, 0)
    
    # Apply blur for natural look
    img = img.filter(ImageFilter.GaussianBlur(radius=2))
    
    filepath = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(filepath)
    print(f"Generated: {filepath}")
    return filepath

def generate_rock_texture(name, base_color, width=512, height=512):
    """Generate rock/stone texture"""
    img = Image.new('RGB', (width, height))
    pixels = img.load()
    
    # Generate noise pattern
    noise = generate_noise(width, height, scale=2.0)
    
    for y in range(height):
        for x in range(width):
            # Add noise to base color
            variation = noise[y][x] - 127
            r = max(0, min(255, base_color[0] + variation))
            g = max(0, min(255, base_color[1] + variation))
            b = max(0, min(255, base_color[2] + variation))
            pixels[x, y] = (r, g, b)
    
    # Apply blur
    img = img.filter(ImageFilter.GaussianBlur(radius=1))
    
    filepath = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(filepath)
    print(f"Generated: {filepath}")
    return filepath

def generate_ore_texture(name, base_color, vein_color, width=512, height=512):
    """Generate ore texture with mineral veins"""
    img = Image.new('RGB', (width, height))
    draw = ImageDraw.Draw(img)
    pixels = img.load()
    
    # Fill with base stone color
    for y in range(height):
        for x in range(width):
            noise = random.randint(-20, 20)
            r = max(0, min(255, base_color[0] + noise))
            g = max(0, min(255, base_color[1] + noise))
            b = max(0, min(255, base_color[2] + noise))
            pixels[x, y] = (r, g, b)
    
    # Add mineral veins
    num_veins = random.randint(8, 15)
    for _ in range(num_veins):
        x1 = random.randint(0, width)
        y1 = random.randint(0, height)
        x2 = x1 + random.randint(-50, 50)
        y2 = y1 + random.randint(-50, 50)
        
        thickness = random.randint(2, 8)
        draw.line([(x1, y1), (x2, y2)], fill=vein_color, width=thickness)
        
        # Add some ore chunks
        for _ in range(random.randint(2, 5)):
            cx = random.randint(min(x1, x2), max(x1, x2))
            cy = random.randint(min(y1, y2), max(y1, y2))
            size = random.randint(5, 15)
            draw.ellipse([cx-size, cy-size, cx+size, cy+size], fill=vein_color)
    
    # Apply blur
    img = img.filter(ImageFilter.GaussianBlur(radius=1))
    
    filepath = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(filepath)
    print(f"Generated: {filepath}")
    return filepath

def generate_metal_texture(name, base_color, width=512, height=512):
    """Generate metallic texture for refined materials"""
    img = Image.new('RGB', (width, height))
    pixels = img.load()
    
    for y in range(height):
        for x in range(width):
            # Metallic sheen effect
            sheen = math.sin(x * 0.02) * math.cos(y * 0.02) * 30
            noise = random.randint(-10, 10)
            
            r = max(0, min(255, int(base_color[0] + sheen + noise)))
            g = max(0, min(255, int(base_color[1] + sheen + noise)))
            b = max(0, min(255, int(base_color[2] + sheen + noise)))
            pixels[x, y] = (r, g, b)
    
    filepath = os.path.join(OUTPUT_DIR, f"{name}.png")
    img.save(filepath)
    print(f"Generated: {filepath}")
    return filepath

def generate_all_textures():
    """Generate all resource textures"""
    print("Generating resource textures...")
    create_output_dir()
    
    # Tree textures
    generate_wood_texture("T_Wood_Bark_Oak")
    generate_wood_texture("T_Wood_Bark_Pine")
    generate_leaves_texture("T_Leaves_Oak")
    generate_leaves_texture("T_Leaves_Pine")
    
    # Rock/Stone
    generate_rock_texture("T_Stone", (128, 128, 128))
    generate_rock_texture("T_Rock_Gray", (100, 100, 100))
    
    # Ore textures
    generate_ore_texture("T_IronOre", (80, 70, 60), (139, 69, 19))
    generate_ore_texture("T_CopperOre", (90, 80, 70), (184, 115, 51))
    generate_ore_texture("T_GoldOre", (100, 95, 80), (255, 215, 0))
    generate_ore_texture("T_AluminumOre", (120, 120, 115), (192, 192, 192))
    generate_ore_texture("T_TitaniumOre", (70, 75, 80), (169, 169, 169))
    generate_ore_texture("T_CoalOre", (50, 50, 50), (30, 30, 30))
    
    # Refined metal textures
    generate_metal_texture("T_Iron_Ingot", (140, 140, 140))
    generate_metal_texture("T_Copper_Ingot", (184, 115, 51))
    generate_metal_texture("T_Gold_Ingot", (255, 215, 0))
    generate_metal_texture("T_Aluminum_Ingot", (192, 192, 192))
    generate_metal_texture("T_Titanium_Ingot", (169, 169, 169))
    generate_metal_texture("T_Steel_Ingot", (105, 105, 105))
    
    # Charcoal
    generate_rock_texture("T_Charcoal", (30, 30, 30))
    generate_rock_texture("T_Coal", (20, 20, 20))
    
    print("\nTexture generation complete!")
    print(f"All textures saved to: {OUTPUT_DIR}")

if __name__ == "__main__":
    generate_all_textures()
