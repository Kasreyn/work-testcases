#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <tuple>

std::tuple<int, std::string> filterPP(const std::string& stdoutContent) {
    // Split the input by lines
    std::vector<std::string> lines;
    std::istringstream stream(stdoutContent);
    std::string line;
    while (std::getline(stream, line)) {
        lines.push_back(line);
    }

    // Regular expression to match preprocessor line annotations
    std::regex ppLineRe(R"(^\s*#\s*(?:line)?\s*\d+\s*\"((?:\\\"|[^\"])*)\")", std::regex_constants::icase);
    bool isInSourceRegion = true;
    int numberOfLinesFiltered = 0;
    std::vector<std::string> filteredLines;

    // Process each line
    for (const auto& line : lines) {
        std::smatch match;
        if (!std::regex_match(line, match, ppLineRe)) {
            // Line does not match the preprocessor pattern
            if (isInSourceRegion) {
                filteredLines.push_back(line);
            } else {
                numberOfLinesFiltered++;
            }
        } else {
            // Line matches the preprocessor pattern
            std::string path = match[1].str();
            if (path.empty() || path == "<source>" || path == "<stdin>" ||
                path.ends_with(".c") || path.ends_with(".cpp")) {
                isInSourceRegion = true;
            } else {
                isInSourceRegion = false;
            }
            numberOfLinesFiltered++;
        }
    }

    // Join filtered lines into a single string
    std::ostringstream result;
    for (const auto& filteredLine : filteredLines) {
        result << filteredLine << '\n';
    }

    return std::make_tuple(numberOfLinesFiltered, result.str());
}

int main() {
    // Read all input from stdin into a single string
    std::string inputContent((std::istreambuf_iterator<char>(std::cin)),
                              std::istreambuf_iterator<char>());

    // Call the filterPP function with the input content
    auto [numberOfLinesFiltered, filteredContent] = filterPP(inputContent);

    // Output the results
    std::cout << "Number of lines filtered: " << numberOfLinesFiltered << "\n";
    std::cout << "Filtered content:\n" << filteredContent;

    return 0;
}

