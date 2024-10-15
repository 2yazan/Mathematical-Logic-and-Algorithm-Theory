#include <deque>
#include <string>

class Brainfuck {

    public:
        Brainfuck(const std::string& input);
        void execute(std::string& output, bool ascii);

    private:
        std::deque<std::uint8_t> cells;
        std::string input;
        bool ascii;
        int current_index;
        int current_char;
        void remove_spaces();
        void remove_new_lines();
        void find_matching_end_bracket();
        void find_matching_start_bracket();
    void op_codes(const char& cur, std::string& output);
};
