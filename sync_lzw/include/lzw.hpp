#ifndef LZW_H_
#define LZW_H_

#include <filesystem>
#include <fstream>
#include "dictionary.hpp"

#define RED "\033[31m"
#define RESET_COLOR "\033[0m"

namespace fs = std::filesystem;

typedef unsigned char uint1;   // 8  bit
typedef unsigned short uint2;  // 16 bit
typedef unsigned int uint4;    // 32 bit

namespace lzw {

static void print_string(std::ofstream &out, const std::string &str) {
    for (int i = 0; i < str.size(); i++) {
        out.write(reinterpret_cast<const char *>(&str[i]), sizeof(char));
    }
}

void encode(const fs::path &input_path, const fs::path &output_path) {
    std::ifstream in(input_path, std::ios::binary);
    FileWriter out(output_path);

    if (in.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << input_path.generic_string() << RESET_COLOR << std::endl;
        return;
    }

    if (out.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
        return;
    }

    EncodeDictionary dict;

    char cur_char;
    std::string prev = "";
    while (in.read(&cur_char, sizeof(char))) {
        std::string tmp(prev);
        tmp += cur_char;

        if (dict.find(tmp)) {
            prev += cur_char;
            continue;
        }

        out.put_bit(dict[prev], dict.get_count_bit());

        dict.add(tmp);

        prev = "";
        prev += cur_char;
    }

    if (!prev.empty()) {
        out.put_bit(dict[prev], dict.get_count_bit());
    }
}

void decode(const fs::path &input_path, const fs::path &output_path) {
    FileReader in(input_path);
    std::ofstream out(output_path, std::ios::binary);

    if (in.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << input_path.generic_string() << RESET_COLOR << std::endl;
        return;
    }

    if (out.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
        return;
    }

    DecodeDictionary dict;

    uint4 cur_code;
    std::string prev = "";
    while (in.get_bit(cur_code, dict.get_count_bit())) {
        if (dict.find(cur_code)) {
            print_string(out, dict[cur_code]);

            if (prev.empty() == false) {
                dict.add(prev + dict[cur_code][0]);
            }

            prev = dict[cur_code];
            continue;
        }

        prev += prev[0];
        dict.add(prev);

        print_string(out, prev);
    }
}
}  // namespace lzw

#endif  // LZW_H_