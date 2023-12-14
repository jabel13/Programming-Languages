import os
import re
import tarfile
import subprocess

def listFiles(directory):
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(('.ml', '.clj', '.c', '.lp', '.py')): 
                yield os.path.join(root, file)

def cIdentifiers(text):
    # C identifier extraction logic
    identifiers = set()
    
    for line in text.splitlines():
        # Remove comments and excess whitespace
        line = re.sub(r'//.*$', '', line).strip()

        # Check for type definitions, variable and function declarations
        type_matches = re.findall(r'\b(?:struct)?\s*([a-zA-Z_][a-zA-Z0-9_]*)(?=\s|\[|\()|([a-zA-Z_][a-zA-Z0-9_]*)(?=\()', line)
        for match in type_matches:
            # Flatten the tuple and remove empty strings
            match = [m for m in match if m]
            if match:
                identifiers.add(match[0])

    return sorted(identifiers)



def cljIdentifiers(text):
    identifiers = set()

    for line in text.splitlines():
        def_matches = re.findall(r'\b(defn|let|loop [^\[]*)(\s+(\S+))?', line, re.I)
        if def_matches:
            for match in def_matches:
                if match[2]:
                    identifier = match[2].strip()
                    if identifier.startswith('['):
                        identifier = identifier.lstrip('[')
                    identifiers.add(identifier)

        # Extract identifiers from function parameters and local variables
        fn_params_and_locals = re.findall(r'\[([^\[\]]+)\]', line)
        for group in fn_params_and_locals:
            # Use a regex that captures words not surrounded by brackets
            words = re.findall(r'\b([^\s,:()[\]]+)\b', group)
            for word in words:
                if not word.isdigit() and not word.startswith(':') and not word.startswith('[') and not word.endswith(']'):
                    identifier = word.strip()
                    if identifier.startswith('['):
                        identifier = identifier.lstrip('[')
                    identifiers.add(identifier)

    return sorted(identifiers)

def oCamlIdentifiers(text):
    identifiers = set()

    # Regular expression to match OCaml identifiers
    pattern = r'\b[a-zA-Z_][\w\']+\b'

    # Split the text into lines
    lines = text.splitlines()

    in_comment = False
    for line in lines:
        # Check for comment start and end
        if '(*' in line:
            in_comment = True
        if '*)' in line:
            in_comment = False
            continue  # Skip the rest of the comment line

        # Process the line if it's not a comment
        if not in_comment and '"' not in line:
            matches = re.findall(pattern, line)
            identifiers.update(matches)

    return sorted(identifiers)

def pyIdentifiers(text):
    identifiers = set()

    # Remove single-line comments
    text = re.sub(r'#.*', '', text) 

    # Remove string literals
    text = re.sub(r'\".*?\"', '', text)
    text = re.sub(r"\'.*?\'", '', text)

    # Match all words that could be Python identifiers
    matches = re.findall(r'\b[a-z][a-zA-Z0-9_]*\b', text)

    matches = [m for m in matches if len(m) > 1]

    identifiers.update(matches)

    # Return sorted list of unique identifiers
    return sorted(identifiers)


def aspIdentifiers(text):

    identifiers = set()

    for line in text.splitlines():
        # Ignore comments
        line = line.split('%')[0]
        
        # Extract all words that start with a letter
        matches = re.findall(r'\b[a-z]\w*', line)
        identifiers.update(matches)
    return sorted(identifiers)

def getExtension(file_path):
    # Open the file and read its contents
    with open(file_path, 'r') as file:
        text = file.read()

    # Get the file extension
    _, extension = os.path.splitext(file_path)

    # Depending on the language, call the appropriate custom extraction function
    if extension == '.c':
        return cIdentifiers(text)
    elif extension == '.clj':
        return cljIdentifiers(text)
    elif extension == '.ml':
        return oCamlIdentifiers(text)
    elif extension == '.py':
        return pyIdentifiers(text)
    elif extension == '.lp':  
        return aspIdentifiers(text)



def summaryFile(directory, assignment_num):

    summary_filename = os.path.join(directory, f'summary_a{assignment_num}.html')

    identifiers_set = set()
    file_summaries = []

    for file_path in listFiles(directory):
        file = open(file_path, 'r')
        lines = file.readlines()
        file.close()
        num_lines = len(lines)

        identifiers = getExtension(file_path)
        identifiers_set.update(identifiers)

        # Prepare file summary line
        rel_path = os.path.relpath(file_path, directory)
        file_summaries.append(f'<li><a href="{rel_path}">{os.path.basename(file_path)}</a> - {num_lines} lines</li>')

    # Open, write, and close the summary file
    summary_file = open(summary_filename, 'w')
    summary_file.write('<html><body>\n<h1>Assignment Summary</h1>\n<ul>\n')
    summary_file.write('\n'.join(file_summaries))
    summary_file.write('</ul>\n<h2>Identifiers</h2>\n<ul>\n')

    # Write sorted identifiers
    sorted_identifiers = sorted(identifiers_set)
    for identifier in sorted_identifiers:
        summary_file.write(f'<li>{identifier}</li>\n')

    summary_file.write('</ul>\n</body></html>\n')
    summary_file.close()

    return os.path.basename(summary_filename) 

def indexFile(csc344_dir, summary_files):
    with open(os.path.join(csc344_dir, 'index.html'), 'w') as index_file:
        index_file.write('<!DOCTYPE html>\n<html><body>\n<h1>All Assignments</h1>\n<ul>\n')

        for i, summary_file in enumerate(summary_files, start=1):
            rel_path = os.path.join(f'a{i}', summary_file)
            index_file.write(f'<li><a href="{rel_path}">Assignment {i}</a></li>\n')

        index_file.write('</ul>\n</body></html>\n')


def makeTarFile(csc344_dir):
    with tarfile.open('csc344.tar.gz', 'w:gz') as tar:
        tar.add(csc344_dir, arcname=os.path.basename(csc344_dir))

def sendEmail(email_address):
    subprocess.run(['mutt', '-s', 'CSC344 Assignments', '-a', 'csc344.tar.gz', '--', email_address])

def main():
    csc344_dir = 'csc344'
    summary_files = []

    for i in range(1, 6): 
        directory = os.path.join(csc344_dir, f'a{i}')
        summary_file = summaryFile(directory, i)
        summary_files.append(summary_file)

    indexFile(csc344_dir, summary_files)
    makeTarFile(csc344_dir)

    email_address = input("Enter the recipient's email address: ")
    sendEmail(email_address)

if __name__ == '__main__':
    main()