#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

inline char get_char(const std::string &line, uint64_t idx) {
    return idx < line.size() ? line[idx] : 0;
}

inline char get_char(const std::vector<std::string> &lines, uint64_t y, uint64_t x) {
    return y < lines.size() ? (x < lines[y].size() ? lines[y][x] : 0) : 0;
}

inline bool match(const char *s) {
    std::cerr << "DEBUG: " << s << std::endl;
    return strcmp("XMAS", s) == 0;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day4 <input file>" << std::endl;
        return 1;
    }
    std::ifstream ifile(argv[1]);
    std::string line;
    std::vector<std::string> lines;
    while(std::getline(ifile, line)) {
        lines.push_back(line);
    }
    const uint64_t NY = lines.size();
    const uint64_t NX = lines.size() > 0 ? lines[0].size() : 0;
    char temp[5] = {0, 0, 0, 0, 0};
    uint64_t total = 0;
    for(uint64_t j = 0; j < NY; ++j) {
        const std::string &line = lines[j];
        assert(line.size() == NX);
        for(uint64_t i = 0; i < NX; ++i) {
            // Check forward
            temp[0] = line[i];
            temp[1] = get_char(line, i + 1);
            temp[2] = get_char(line, i + 2);
            temp[3] = get_char(line, i + 3);
            total += match(&temp[0]) ? 1 : 0;
            // Check backward
            temp[1] = get_char(line, i - 1);
            temp[2] = get_char(line, i - 2);
            temp[3] = get_char(line, i - 3);
            total += match(&temp[0]) ? 1 : 0;
            // Check up left
            temp[1] = get_char(lines, j - 1, i - 1);
            temp[2] = get_char(lines, j - 2, i - 2);
            temp[3] = get_char(lines, j - 3, i - 3);
            total += match(&temp[0]) ? 1 : 0;
            // Check up
            temp[1] = get_char(lines, j - 1, i);
            temp[2] = get_char(lines, j - 2, i);
            temp[3] = get_char(lines, j - 3, i);
            total += match(&temp[0]) ? 1 : 0;
            // Check up right
            temp[1] = get_char(lines, j - 1, i + 1);
            temp[2] = get_char(lines, j - 2, i + 2);
            temp[3] = get_char(lines, j - 3, i + 3);
            total += match(&temp[0]) ? 1 : 0;
            // Check down left
            temp[1] = get_char(lines, j + 1, i - 1);
            temp[2] = get_char(lines, j + 2, i - 2);
            temp[3] = get_char(lines, j + 3, i - 3);
            total += match(&temp[0]) ? 1 : 0;
            // Check down
            temp[1] = get_char(lines, j + 1, i);
            temp[2] = get_char(lines, j + 2, i);
            temp[3] = get_char(lines, j + 3, i);
            total += match(&temp[0]) ? 1 : 0;
            // Check down right
            temp[1] = get_char(lines, j + 1, i + 1);
            temp[2] = get_char(lines, j + 2, i + 2);
            temp[3] = get_char(lines, j + 3, i + 3);
            total += match(&temp[0]) ? 1 : 0;
        }
    }
    std::cout << "Total XMAS count: " << total << std::endl;
    return 0;
}
