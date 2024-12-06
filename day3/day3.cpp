#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day3 <input file>" << std::endl;
        return 1;
    }
    const std::regex RE_MUL(R"(mul\(\d{1,3}\,\d{1,3}\))");
    std::ifstream ifile(argv[1]);
    std::string line;
    uint64_t sum = 0;
    uint64_t line_num = 1;
    while(std::getline(ifile, line)) {
        const char *const LINE_START = line.data();
        const char *const LINE_END = line.data() + line.size();
        const char *p = LINE_START;
        while(p != LINE_END) {
            std::cmatch cm;
            // std::cerr << "DEBUG: " << p << std::endl;
            std::regex_search(p, cm, RE_MUL);
            if(cm.size() == 0) {
                std::cerr << "DEBUG: No more matches on line "
                    << line_num << std::endl;
                break;
            }
            // std::cerr << "DEBUG: " << cm.str() << " start, end "
            //     << cm[0].first - LINE_START << ", "
            //     << cm[0].second - LINE_START;
            char buff[16];
            uint32_t x, y;
            memset(&buff[0], 0, sizeof(char) * 16);
            memcpy(&buff[0], cm[0].first, cm[0].second - cm[0].first);
            sscanf(&buff[0], "mul(%u,%u)", &x, &y);
            // std::cerr << ", computing " << x << " * " << y << " + " << sum
            //     << " = " << x * y + sum << std::endl;
            sum += x * y;
            p = cm[0].second;
        }
    }
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
