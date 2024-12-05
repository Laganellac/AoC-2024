#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

// Read a space separated line of values into the given vector
template<typename T>
void parse_line(const std::string &line, std::vector<T> &values) {
    std::stringstream iss(line);
    values.clear();
    while(iss.good()) {
        T a;
        iss >> a;
        values.push_back(a);
    }
}

template<typename T>
std::string to_string(const std::vector<T> &values) {
    const uint32_t N = values.size();
    if(values.size() == 0) {
        return std::string("[]");
    }
    std::ostringstream oss;
    oss << "[";
    for(uint32_t i = 0; i < N; ++i) {
        oss << values[i] << ", ";
    }
    std::string s = oss.str();
    const uint32_t slen = s.size();
    s[slen - 2] = ']';
    s.resize(slen - 1);
    return s;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day2 <input file>" << std::endl;
        return 1;
    }
    // Reuse some of these variables
    std::ifstream input_file(argv[1]);
    std::string line;
    std::vector<int> levels;
    // Part 1
    {
        uint32_t total_safe = 0;
        uint32_t total_unsafe = 0;
        while(std::getline(input_file, line)) {
            parse_line(line, levels);
            const uint32_t N = levels.size();
            if(N == 1) {
                std::cerr << "ERROR: Encountered levels with size 1" << std::endl;
                continue;
            }
            bool safe = true;
            int last_diff;
            for(uint32_t i = 0; i < N - 1; ++i) {
                const int diff = levels[i] - levels[i+1];
                if(i == 0) {
                    last_diff = diff;
                }
                if(!(last_diff < 0 && diff < 0 && diff > -4)
                    && !(last_diff > 0 && diff > 0 && diff < 4)) {
                    safe = false;
                }
                else {
                    last_diff = diff;
                }
            }
            if(safe == true) {
                total_safe += 1;
            }
            else {
                total_unsafe += 1;
            }
        }
        std::cout << "Part 1:" << std::endl;
        std::cout << "Total safe: " << total_safe << std::endl;
        std::cout << "Total unsafe: " << total_unsafe << std::endl;
    }
    // Part 2
    {
        uint32_t total_safe = 0;
        uint32_t total_unsafe = 0;
        std::cout << "Part 2:" << std::endl;
        input_file.clear();
        input_file.seekg(std::ios::beg);
        uint32_t line_number = 0;
        while(std::getline(input_file, line) ) {
            line_number += 1;
            parse_line(line, levels);
            const uint32_t N = levels.size();
            if(N == 1) {
                std::cerr << "ERROR: Encountered levels with size 1" << std::endl;
                continue;
            }
            uint32_t skip_idxs[2] = {0, 0};
            bool safe = true;
            int last_diff;
            // First loop through to check if we pass the original rules
            for(uint32_t i = 0; i < N - 1; ++i) {
                const int diff = levels[i] - levels[i+1];
                if(i == 0 || (i == 1 && safe == false)) {
                    last_diff = diff;
                }
                if(!(last_diff < 0 && diff < 0 && diff > -4)
                    && !(last_diff > 0 && diff > 0 && diff < 4)) {
                    skip_idxs[0] = i;
                    skip_idxs[1] = i + 1;
                    safe = false;
                    break;
                }
                else {
                    last_diff = diff;
                }
            }
            if(!safe) {
                std::cerr << "DEBUG: UNSAFE LEVELS DETECTED " << to_string(levels)
                        << "\n       Check pair [" << skip_idxs[0] << ", "
                        << skip_idxs[1] << "]" << " line #" << line_number << std::endl;
                // Second loop through and check if we pass the more lienient rules
                for(uint32_t j = 0; j < 2u; ++j) {
                    uint32_t skip_idx = skip_idxs[j];
                    int temp;
                    last_diff = std::numeric_limits<int>::min();
                    safe = true;
                    uint32_t NN = skip_idx == N - 1 ? N - 2 : N - 1;
                    uint32_t i = skip_idx == 0 ? 1 : 0;
                    for(; i < N - 1; ++i) {
                        int diff;
                        // Avoid unwanted value
                        if(i + 1 == skip_idx) {
                            temp = levels[i];
                            continue;
                        }
                        else if(i == skip_idx) {
                            diff = temp - levels[i+1];
                        }
                        else {
                            diff = levels[i] - levels[i+1];
                        }
                        // If this is the first valid comparison just set last_diff
                        if(last_diff == std::numeric_limits<int>::min()) {
                            last_diff = diff;
                        }
                        // Do the safety test
                        if(!(last_diff < 0 && diff < 0 && diff > -4)
                            && !(last_diff > 0 && diff > 0 && diff < 4)) {
                            safe = false;
                            break;
                        }
                        else {
                            last_diff = diff;
                        }
                    }
                    // Don't need to test again if we were safe
                    if(safe == true) {
                        std::cerr << "DEBUG: After removing idx " << skip_idx << " levels are now safe" << std::endl;
                        break;
                    }
                    // Attempt also starting from 1
                    if(j == 0 && skip_idxs[0] == 1) {
                        skip_idxs[0] = 0;
                        --j;
                    }
                }
            }

            if(safe) {
                total_safe += 1;
            }
            else {
                total_unsafe += 1;
            }
        }
        std::cout << "Total safe: " << total_safe << std::endl;
        std::cout << "Total unsafe: " << total_unsafe << std::endl;
    }
    return 0;
}
