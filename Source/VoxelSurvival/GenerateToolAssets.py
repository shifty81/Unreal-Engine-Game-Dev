#!/usr/bin/env python3
"""
Tool Texture and Mesh Generator for Unreal Engine
Generates textures and basic OBJ meshes for tools
"""

import os
import sys
from PIL import Image, ImageDraw
import math

# Output directories
TEXTURE_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "Content", "Textures", "Tools")
MESH_DIR = os.path.join(os.path.dirname(__file__), "..", "..", "Content", "Meshes", "Tools")

def create_output_dirs():
    """Create output directories if they don't exist"""
    os.makedirs(TEXTURE_DIR, exist_ok=True)
    os.makedirs(MESH_DIR, exist_ok=True)
    print(f"Texture directory: {TEXTURE_DIR}")
    print(f"Mesh directory: {MESH_DIR}")

def generate_tool_texture(tool_name, head_color, handle_color, width=256, height=256):
    """Generate texture for a tool"""
    img = Image.new('RGB', (width, height), color=(255, 255, 255, 0))
    draw = ImageDraw.Draw(img)
    
    # Draw tool head (top half)
    head_height = int(height * 0.4)
    draw.rectangle([0, 0, width, head_height], fill=head_color)
    
    # Add some shading to the head
    for i in range(10):
        shade = int(255 * (i / 10))
        x = int(width * i / 10)
        w = int(width / 10)
        overlay_color = (
            max(0, min(255, head_color[0] - 20 + (shade // 12))),
            max(0, min(255, head_color[1] - 20 + (shade // 12))),
            max(0, min(255, head_color[2] - 20 + (shade // 12)))
        )
        draw.rectangle([x, 0, x + w, head_height], fill=overlay_color)
    
    # Draw handle (bottom half)
    handle_start = head_height
    handle_width = int(width * 0.3)
    handle_x = (width - handle_width) // 2
    draw.rectangle([handle_x, handle_start, handle_x + handle_width, height], fill=handle_color)
    
    # Add wood grain to handle
    for y in range(handle_start, height, 5):
        grain_darkness = (y % 20) * 2
        grain_color = (
            max(0, handle_color[0] - grain_darkness),
            max(0, handle_color[1] - grain_darkness),
            max(0, handle_color[2] - grain_darkness)
        )
        draw.line([(handle_x, y), (handle_x + handle_width, y)], fill=grain_color, width=2)
    
    # Add edge highlights
    draw.rectangle([0, 0, width, 2], fill=(255, 255, 255))  # Top highlight
    draw.rectangle([0, 0, 2, head_height], fill=(255, 255, 255))  # Left highlight
    
    # Add edge shadows
    draw.rectangle([0, head_height - 2, width, head_height], fill=(0, 0, 0))  # Bottom shadow
    draw.rectangle([width - 2, 0, width, head_height], fill=(50, 50, 50))  # Right shadow
    
    filepath = os.path.join(TEXTURE_DIR, f"{tool_name}.png")
    img.save(filepath)
    print(f"Generated texture: {filepath}")
    return filepath

def generate_pickaxe_mesh(filename, scale=1.0):
    """Generate a simple pickaxe OBJ mesh"""
    vertices = []
    faces = []
    
    # Pickaxe head (double-sided pick)
    # Left pick point
    vertices.extend([
        (-1.5 * scale, 0.1 * scale, 0.0 * scale),
        (-0.5 * scale, 0.1 * scale, 0.0 * scale),
        (-0.5 * scale, -0.1 * scale, 0.0 * scale),
        (-1.5 * scale, -0.1 * scale, 0.0 * scale),
    ])
    
    # Right pick point
    vertices.extend([
        (0.5 * scale, 0.1 * scale, 0.0 * scale),
        (1.5 * scale, 0.1 * scale, 0.0 * scale),
        (1.5 * scale, -0.1 * scale, 0.0 * scale),
        (0.5 * scale, -0.1 * scale, 0.0 * scale),
    ])
    
    # Handle
    for i in range(10):
        t = i / 9.0
        y = -0.2 * scale - t * 2.0 * scale
        x = 0.05 * scale
        vertices.extend([
            (-x, y, x),
            (x, y, x),
            (x, y, -x),
            (-x, y, -x)
        ])
    
    # Generate faces for pickaxe head
    faces.extend([
        (1, 2, 3), (1, 3, 4),  # Left pick
        (5, 6, 7), (5, 7, 8),  # Right pick
    ])
    
    # Generate faces for handle
    base = 8
    for i in range(9):
        v = base + i * 4
        # Connect segments
        faces.extend([
            (v+1, v+2, v+6), (v+1, v+6, v+5),
            (v+2, v+3, v+7), (v+2, v+7, v+6),
            (v+3, v+4, v+8), (v+3, v+8, v+7),
            (v+4, v+1, v+5), (v+4, v+5, v+8),
        ])
    
    write_obj_file(filename, vertices, faces)

def generate_axe_mesh(filename, scale=1.0):
    """Generate a simple axe OBJ mesh"""
    vertices = []
    faces = []
    
    # Axe head (wedge shape)
    vertices.extend([
        # Front face
        (-0.8 * scale, 0.2 * scale, 0.1 * scale),
        (0.8 * scale, 0.2 * scale, 0.1 * scale),
        (0.3 * scale, -0.2 * scale, 0.1 * scale),
        (-0.3 * scale, -0.2 * scale, 0.1 * scale),
        # Back face
        (-0.8 * scale, 0.2 * scale, -0.1 * scale),
        (0.8 * scale, 0.2 * scale, -0.1 * scale),
        (0.3 * scale, -0.2 * scale, -0.1 * scale),
        (-0.3 * scale, -0.2 * scale, -0.1 * scale),
    ])
    
    # Handle (cylinder-like)
    for i in range(10):
        t = i / 9.0
        y = -0.4 * scale - t * 2.0 * scale
        x = 0.05 * scale
        vertices.extend([
            (-x, y, x),
            (x, y, x),
            (x, y, -x),
            (-x, y, -x)
        ])
    
    # Axe head faces
    faces.extend([
        (1, 2, 3), (1, 3, 4),  # Front
        (5, 6, 7), (5, 7, 8),  # Back
        (1, 2, 6), (1, 6, 5),  # Top
        (3, 4, 8), (3, 8, 7),  # Bottom
        (1, 4, 8), (1, 8, 5),  # Left
        (2, 3, 7), (2, 7, 6),  # Right
    ])
    
    # Handle faces
    base = 8
    for i in range(9):
        v = base + i * 4
        faces.extend([
            (v+1, v+2, v+6), (v+1, v+6, v+5),
            (v+2, v+3, v+7), (v+2, v+7, v+6),
            (v+3, v+4, v+8), (v+3, v+8, v+7),
            (v+4, v+1, v+5), (v+4, v+5, v+8),
        ])
    
    write_obj_file(filename, vertices, faces)

def generate_shovel_mesh(filename, scale=1.0):
    """Generate a simple shovel OBJ mesh"""
    vertices = []
    faces = []
    
    # Shovel blade (flat rectangle)
    vertices.extend([
        (-0.4 * scale, 0.2 * scale, 0.05 * scale),
        (0.4 * scale, 0.2 * scale, 0.05 * scale),
        (0.4 * scale, -0.4 * scale, 0.05 * scale),
        (-0.4 * scale, -0.4 * scale, 0.05 * scale),
        (-0.4 * scale, 0.2 * scale, -0.05 * scale),
        (0.4 * scale, 0.2 * scale, -0.05 * scale),
        (0.4 * scale, -0.4 * scale, -0.05 * scale),
        (-0.4 * scale, -0.4 * scale, -0.05 * scale),
    ])
    
    # Handle
    for i in range(10):
        t = i / 9.0
        y = 0.3 * scale + t * 2.0 * scale
        x = 0.04 * scale
        vertices.extend([
            (-x, y, x),
            (x, y, x),
            (x, y, -x),
            (-x, y, -x)
        ])
    
    # Blade faces
    faces.extend([
        (1, 2, 3), (1, 3, 4),  # Front
        (5, 6, 7), (5, 7, 8),  # Back
        (1, 2, 6), (1, 6, 5),  # Top
        (3, 4, 8), (3, 8, 7),  # Bottom
        (1, 4, 8), (1, 8, 5),  # Left
        (2, 3, 7), (2, 7, 6),  # Right
    ])
    
    # Handle faces
    base = 8
    for i in range(9):
        v = base + i * 4
        faces.extend([
            (v+1, v+2, v+6), (v+1, v+6, v+5),
            (v+2, v+3, v+7), (v+2, v+7, v+6),
            (v+3, v+4, v+8), (v+3, v+8, v+7),
            (v+4, v+1, v+5), (v+4, v+5, v+8),
        ])
    
    write_obj_file(filename, vertices, faces)

def generate_hammer_mesh(filename, scale=1.0):
    """Generate a simple hammer OBJ mesh"""
    vertices = []
    faces = []
    
    # Hammer head (rectangular block)
    vertices.extend([
        # Front face
        (-0.6 * scale, 0.2 * scale, 0.2 * scale),
        (0.6 * scale, 0.2 * scale, 0.2 * scale),
        (0.6 * scale, -0.2 * scale, 0.2 * scale),
        (-0.6 * scale, -0.2 * scale, 0.2 * scale),
        # Back face
        (-0.6 * scale, 0.2 * scale, -0.2 * scale),
        (0.6 * scale, 0.2 * scale, -0.2 * scale),
        (0.6 * scale, -0.2 * scale, -0.2 * scale),
        (-0.6 * scale, -0.2 * scale, -0.2 * scale),
    ])
    
    # Handle
    for i in range(10):
        t = i / 9.0
        y = -0.3 * scale - t * 2.0 * scale
        x = 0.05 * scale
        vertices.extend([
            (-x, y, x),
            (x, y, x),
            (x, y, -x),
            (-x, y, -x)
        ])
    
    # Hammer head faces
    faces.extend([
        (1, 2, 3), (1, 3, 4),  # Front
        (5, 6, 7), (5, 7, 8),  # Back
        (1, 2, 6), (1, 6, 5),  # Top
        (3, 4, 8), (3, 8, 7),  # Bottom
        (1, 4, 8), (1, 8, 5),  # Left
        (2, 3, 7), (2, 7, 6),  # Right
    ])
    
    # Handle faces
    base = 8
    for i in range(9):
        v = base + i * 4
        faces.extend([
            (v+1, v+2, v+6), (v+1, v+6, v+5),
            (v+2, v+3, v+7), (v+2, v+7, v+6),
            (v+3, v+4, v+8), (v+3, v+8, v+7),
            (v+4, v+1, v+5), (v+4, v+5, v+8),
        ])
    
    write_obj_file(filename, vertices, faces)

def write_obj_file(filename, vertices, faces):
    """Write vertices and faces to OBJ file"""
    filepath = os.path.join(MESH_DIR, filename)
    
    with open(filepath, 'w') as f:
        f.write("# Tool mesh generated by Unreal Engine tool generator\n")
        f.write(f"# {filename}\n\n")
        
        # Write vertices
        for v in vertices:
            f.write(f"v {v[0]:.6f} {v[1]:.6f} {v[2]:.6f}\n")
        
        f.write("\n")
        
        # Write faces (OBJ uses 1-based indexing)
        for face in faces:
            f.write(f"f {face[0]} {face[1]} {face[2]}\n")
    
    print(f"Generated mesh: {filepath}")

def generate_all_tools():
    """Generate all tool textures and meshes"""
    print("Generating tool assets...")
    create_output_dirs()
    
    # Material colors
    wood_color = (101, 67, 33)
    stone_color = (128, 128, 128)
    iron_color = (140, 140, 140)
    steel_color = (105, 105, 105)
    handle_color = (139, 90, 43)
    
    # Wood tools
    generate_tool_texture("T_WoodPickaxe", wood_color, handle_color)
    generate_tool_texture("T_WoodAxe", wood_color, handle_color)
    generate_pickaxe_mesh("SM_WoodPickaxe.obj")
    generate_axe_mesh("SM_WoodAxe.obj")
    
    # Stone tools
    generate_tool_texture("T_StonePickaxe", stone_color, handle_color)
    generate_tool_texture("T_StoneAxe", stone_color, handle_color)
    generate_tool_texture("T_StoneShovel", stone_color, handle_color)
    generate_pickaxe_mesh("SM_StonePickaxe.obj")
    generate_axe_mesh("SM_StoneAxe.obj")
    generate_shovel_mesh("SM_StoneShovel.obj")
    
    # Iron tools
    generate_tool_texture("T_IronPickaxe", iron_color, handle_color)
    generate_tool_texture("T_IronAxe", iron_color, handle_color)
    generate_tool_texture("T_IronShovel", iron_color, handle_color)
    generate_tool_texture("T_IronHammer", iron_color, handle_color)
    generate_pickaxe_mesh("SM_IronPickaxe.obj")
    generate_axe_mesh("SM_IronAxe.obj")
    generate_shovel_mesh("SM_IronShovel.obj")
    generate_hammer_mesh("SM_IronHammer.obj")
    
    # Steel tools
    generate_tool_texture("T_SteelPickaxe", steel_color, handle_color)
    generate_tool_texture("T_SteelAxe", steel_color, handle_color)
    generate_pickaxe_mesh("SM_SteelPickaxe.obj")
    generate_axe_mesh("SM_SteelAxe.obj")
    
    print("\nTool asset generation complete!")
    print(f"Textures saved to: {TEXTURE_DIR}")
    print(f"Meshes saved to: {MESH_DIR}")

if __name__ == "__main__":
    generate_all_tools()
