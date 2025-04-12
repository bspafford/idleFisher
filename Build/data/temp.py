import os

def list_png_files(root_dir, output_file):
    with open(output_file, 'w') as f:
        for dirpath, dirnames, filenames in os.walk(root_dir):
            for filename in filenames:
                if filename.lower().endswith('.png'):
                    full_path = os.path.join(dirpath, filename)
                    rel_path = full_path.replace("\\", "/")
                    rel_path = rel_path.replace(".PNG", ".png")
                    rel_path = rel_path.replace("Q:/Documents/VisualStudios/SDL2", ".")
                    f.write(f"{rel_path.lower()}\n")  # Write the absolute path

def list_png_files1(root_dir, output_file):
    with open(output_file, 'a') as f:
        for dirpath, dirnames, filenames in os.walk(root_dir):
            for filename in filenames:
                if filename.lower().endswith('.png'):
                    full_path = os.path.join(dirpath, filename)
                    rel_path = full_path.replace("\\", "/")
                    rel_path = rel_path.replace("Q:/Documents/VisualStudios/SDL2", ".")
                    rel_path = rel_path.replace(".PNG", ".png")
                    f.write(f"{(rel_path.lower())}\n")  # Write the absolute path
        
        f.write("\n")
        f.write("None".lower())

# Example usage:
list_png_files("Q:/Documents/VisualStudios/SDL2/images", "Q:/Documents/VisualStudios/SDL2/data/png_files.txt")
list_png_files1("Q:/Documents/VisualStudios/SDL2/fonts", "Q:/Documents/VisualStudios/SDL2/data/png_files.txt")