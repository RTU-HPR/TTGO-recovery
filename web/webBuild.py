import os

def html_to_c_string(file_path, output_file):
    # Delete the output file if it exists
    if os.path.exists(output_file):
        os.remove(output_file)
    
    with open(file_path, 'r') as file:
        lines = file.readlines()

    # Escape special characters and wrap each line into a C string
    c_lines = []
    for line in lines:
        line = line.replace("\\", "\\\\")
        line = line.replace("\"", "\\\"")
        line = line.replace("\r", "\\r")
        line = line.replace("\t", "\\t")
        line = line.rstrip() + "\\n"  # Add \n to the end of each line
        c_lines.append(f"\"{line.rstrip()}\"")

    # Join the lines with a newline and a starting quote
    c_string = "\n".join(c_lines)

    # Write to .h file
    with open(output_file, 'w') as file:
        file.write("#ifndef _WEBPAGE_H\n")
        file.write("#define _WEBPAGE_H\n")
        file.write("#include <Arduino.h>\n")
        file.write(f"const char* webPage = \n{c_string};\n")
        file.write("#endif // _WEBPAGE_H\n")

# Usage
html_to_c_string("web/index.html", "include/webpage.h")