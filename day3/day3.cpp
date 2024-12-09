#include <cassert>
#include <cstring>
#include <fstream>
#include <iostream>
#include <regex>

enum State {
    STATE_DO,
    STATE_DONT
};

static const std::regex RE_MUL(R"(mul\(\d{1,3}\,\d{1,3}\))");

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day3 <input file>" << std::endl;
        return 1;
    }
    std::ifstream ifile(argv[1]);
    std::string line;
    uint64_t sum = 0;
    State state = STATE_DO;
    while(std::getline(ifile, line)) {
        const char *const LINE_START = line.data();
        const char *const LINE_END = line.data() + line.size();
        const char *p = LINE_START;
        while(p != LINE_END) {
            // std::cerr << "DEBUG: " << p << std::endl;
            switch(state) {
                case STATE_DO: {
                    std::cmatch cm;
                    const char *dont_ptr = strstr(p, "don't()");
                    bool found_mul = std::regex_search(p, cm, RE_MUL);
                    // If we have both, pick the one that happens next
                    if(dont_ptr != nullptr  && found_mul) {
                        if(cm[0].first < dont_ptr) {
                            dont_ptr = nullptr;
                        }
                        else {
                            found_mul = false;
                        }
                    }
                    assert(!(dont_ptr != nullptr && found_mul));
                    // Execute the correct operation or none at all
                    if(dont_ptr != nullptr) {
                        state = STATE_DONT;
                        p = dont_ptr + 6;
                    }
                    else if(found_mul) {
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
                    else {
                        p = LINE_END;
                    }
                    break;
                }
                case STATE_DONT: {
                    const char *const do_ptr = strstr(p, "do()");
                    if(do_ptr != nullptr) {
                        state = STATE_DO;
                        p = do_ptr + 4;
                    }
                    else {
                        p = LINE_END;
                    }
                    break;
                }
            }
        }
    }
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
