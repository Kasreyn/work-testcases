import sys
import re

def filter_preprocessed_output(output):
    unwanted_patterns = [
        re.compile(r'^\s*#\s*(line)?\s*\d+\s*".*"$', re.IGNORECASE),  # Remove line markers
        re.compile(r'/usr/include|<built-in>|<command-line>', re.IGNORECASE),  # System paths
        re.compile(r'^\s*namespace std\b', re.IGNORECASE),  # Start of std namespace
        re.compile(r'^\s*typedef\b.*\b(size_t|ptrdiff_t|uintptr_t|intptr_t)', re.IGNORECASE),  # Common typedefs
        re.compile(r'^\s*extern\b.*;\s*$', re.IGNORECASE),  # Function prototypes
        re.compile(r'^\s*struct\b', re.IGNORECASE),  # struct definitions
        re.compile(r'^\s*class\b', re.IGNORECASE),   # class definitions
    ]

    filtered_lines = []
    inside_std_namespace = False

    for line in output.splitlines():
        # Skip any lines matching unwanted patterns
        if any(pattern.search(line) for pattern in unwanted_patterns):
            # Track inside namespace std block
            if "namespace std" in line:
                inside_std_namespace = True
            continue

        # Skip the contents of `namespace std` block
        if inside_std_namespace:
            if line.strip() == "}":
                inside_std_namespace = False
            continue

        filtered_lines.append(line)

    return "\n".join(filtered_lines)

if __name__ == "__main__":
    input_data = sys.stdin.read()
    print(filter_preprocessed_output(input_data))
