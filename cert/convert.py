import os

def get_cert(file_path):
    os.system(f"curl -o {file_path} https://letsencrypt.org/certs/isrgrootx1.pem")

def del_file(file_path):
    if os.path.exists(file_path):
        os.remove(file_path)

def convert_cert(file_path, output_file):
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
        file.write("#pragma once\n")
        file.write("#include <Arduino.h>\n")
        file.write(f"const PROGMEM char* MQTT_ROOT_CERTIFICATE = \n{c_string};\n")

# Usage
del_file("include/cert.h")
get_cert("cert/root.pem")
convert_cert("cert/root.pem", "include/cert.h")
del_file("cert/root.pem")