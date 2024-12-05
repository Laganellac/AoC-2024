#include <array>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

class MemoryParser {
public:
    MemoryParser()
        : sum(0), x(0), state(STATE_NONE), digits(), digits_read(0)
    {
        memset(digits.data(), 0, sizeof(char) * 4);
    }

    uint64_t execute(std::istream &input);

protected:
    enum State {
        STATE_NONE,
        STATE_M,
        STATE_MU,
        STATE_MUL,
        STATE_MULP,
        STATE_MULPNC
    };

    // Converts the given character to its escape sequence it's not printable
    static std::string to_string(char c);
    // Converts the given state value to its string representation
    static std::string to_string(State s);

    void debug_print_state(char c) const;

    // Parse the 3-digit number, 3-digit number part of the the mul(X, Y) instruction
    void handle_mulp_mulpnc(char c);

    // Read the digits and digits_read variables to return a uint32_t
    uint32_t parse_digits();

    // Resets the internal state machine
    void reset();

    uint64_t sum;
    uint32_t x;
    State state;
    std::array<char, 4> digits;
    uint32_t digits_read;
};

// Parsen the given stream of memory and return the sum
// of all the valid mul operations.
uint64_t
MemoryParser::execute(std::istream &input) {
    reset();
    // Set the stream to include whitespace characters when reading
    if(input.good()) {
        input >> std::noskipws;
    }
    while(input.good()) {
        char c;
        input >> c;
        debug_print_state(c);
        switch(state) {
            case STATE_NONE:
                if(c == 'm') state = STATE_M;
                break;
            case STATE_M:
                if(c == 'u') state = STATE_MU;
                else reset();
                break;
            case STATE_MU:
                if(c == 'l') state = STATE_MUL;
                else reset();
                break;
            case STATE_MUL:
                if(c == '(') state = STATE_MULP;
                else reset();
                break;
            case STATE_MULP: // Fallthrough
            case STATE_MULPNC:
                handle_mulp_mulpnc(c);
                break;
        }
    }
    return sum;
}

std::string
MemoryParser::to_string(char c) {
    std::string s;
    if(c >= 32) {
        s.push_back(c);
    }
    else if(c == '\n') {
        s = "\\n";
    }
    else {
        std::ostringstream oss;
        oss << "\\\\" << (unsigned int)c;
        s = oss.str();
    }
    return s;
}

std::string
MemoryParser::to_string(State s) {
    std::string ss;
    switch(s) {
        case STATE_NONE:
            ss = "STATE_NONE";
            break;
        case STATE_M:
            ss = "STATE_M";
            break;
        case STATE_MU:
            ss = "STATE_MU";
            break;
        case STATE_MUL:
            ss = "STATE_MUL";
            break;
        case STATE_MULP:
            ss = "STATE_MULP";
            break;
        case STATE_MULPNC:
            ss = "STATE_MULPNC";
            break;
    }
    return ss;
}

void
MemoryParser::debug_print_state(char c) const {
    std::cerr << "DEBUG: Current state {\n"
        << "    \"c\": \"" << to_string(c)
        << "\"\n    \"sum\": " << sum
        << "\n    \"x\": " << x
        << "\n    \"state\": \"" << to_string(state)
        << "\"\n    \"digits\": \"" << digits.data()
        << "\"\n    \"digits_read\": " << digits_read
        << "\n}" << std::endl;
}

void
MemoryParser::handle_mulp_mulpnc(char c) {
    /*if(state == STATE_MULPNC) {
        assert(digits[digits.size() - 1] == 0);
        std::cerr << "DEBUG: MULPNC with char " << c
            << " and digits_read " << digits_read << " and digits "
            << digits.data() << std::endl;
    }*/
    if(c == ',' && digits_read == 0) {
        reset();
    }
    else if(c == ',' && digits_read > 0 && state == STATE_MULP) {
        x = parse_digits();
        //std::cerr << "DEBUG: Read X value " << x << std::endl;
        state = STATE_MULPNC;
    }
    else if(c == ')' && digits_read > 0 && state == STATE_MULPNC) {
        uint32_t y = parse_digits();
        //std::cerr << "DEBUG: Read Y value " << y << std::endl;
        sum += x * y;
        std::cerr << "DEBUG: " << x << " * " << y << " = " << x * y
            << " new sum = " << sum << std::endl;
        reset();
    }
    else if(std::isdigit(c)) {
        assert(digits_read < digits.size() - 1);
        digits[digits_read++] = c;
    }
    else {
        reset();
    }
}

uint32_t
MemoryParser::parse_digits() {
    assert(digits[digits.size() - 1] == 0);
    uint32_t value = (uint32_t)-1;
    sscanf(digits.data(), "%u", &value);
    memset(digits.data(), 0, sizeof(char) * digits.size());
    digits_read = 0;
    return value;
}

void
MemoryParser::reset() {
    //std::cerr << "DEBUG: Resetting from state " << state << std::endl;
    state = STATE_NONE;
    x = 0;
    memset(digits.data(), 0, sizeof(char) * 4);
    digits_read = 0;
}

int main(int argc, char **argv) {
    if(argc != 2) {
        std::cerr << "Usage: ./day3 <input file>" << std::endl;
        return 1;
    }
    std::ifstream ifile(argv[1]);
    MemoryParser p;
    uint64_t sum = p.execute(ifile);
    std::cout << "Sum: " << sum << std::endl;
    return 0;
}
