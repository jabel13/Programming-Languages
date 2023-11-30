import os
import subprocess
import sys

if __name__ == "__main__":
    
    directory = sys.argv[1]

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if os.path.isfile(file_path):
            # Run the command and capture the output
            result = subprocess.run(['wc', '-l', file_path], capture_output=True, text=True)
            # Split the result and extract the line count from the output of the wc command
            line_count = result.stdout.split()[0]
            print(f"{filename}: {line_count} lines")





