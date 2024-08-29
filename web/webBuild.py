import os

def html_to_c_string(file_path, output_file, page_name):
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
    
    # Ensure the directory for the output file exists
    output_dir = os.path.dirname(output_file)
    if not os.path.exists(output_dir):
        os.makedirs(output_dir, exist_ok=True)

    # Write to .h file
    with open(output_file, 'w') as file:
        file.write(f"#ifndef _{page_name}_H\n")
        file.write(f"#define _{page_name}_H\n")
        file.write("#include <Arduino.h>\n")
        file.write(f"const PROGMEM char* {page_name} = \n{c_string};\n")
        file.write(f"#endif // _{page_name}_H\n")

# Usage
html_to_c_string("web/mqtt.html", "include/webpages/mqtt_webpage.h", "MQTT_SETTINGS_PAGE")
html_to_c_string("web/radio.html", "include/webpages/radio_webpage.h", "RADIO_SETTINGS_PAGE")