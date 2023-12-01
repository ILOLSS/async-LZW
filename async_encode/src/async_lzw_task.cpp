#include "async_lzw_task.hpp"

AsyncEncodeTask::AsyncEncodeTask(
    const fs::path &input_path,
    const fs::path &output_path,
    uint4 offset,
    uint4 len
)
    : in_(input_path, std::ios::binary),
      out_(output_path),
      offset_(offset),
      len_(len) {
    if (in_.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << input_path.generic_string() << RESET_COLOR << std::endl;
    }

    if (out_.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
    }

    in_.seekg(offset_, std::ios::beg);
}

int AsyncEncodeTask::run() {
    EncodeDictionary dict;

    char cur_char;
    std::string prev = "";
    uint4 cnt_bites = 0;
    while (len_ > 0 && in_.read(&cur_char, sizeof(char))) {
        len_--;

        std::string tmp(prev);
        tmp += cur_char;

        if (dict.find(tmp)) {
            prev += cur_char;
            continue;
        }

        out_.put_bit(dict[prev], dict.get_count_bit());
        cnt_bites += dict.get_count_bit();

        dict.add(tmp);

        prev = "";
        prev += cur_char;
    }

    if (!prev.empty()) {
        out_.put_bit(dict[prev], dict.get_count_bit());
        cnt_bites += dict.get_count_bit();
    }

    return cnt_bites;
}

// AsyncDecodeTask

AsyncDecodeTask::AsyncDecodeTask(
    const fs::path &input_path,
    const fs::path &output_path,
    uint4 bit_offset,
    uint4 bit_len
)
    : in_(input_path),
      out_(output_path, std::ios::binary),
      bit_offset_(bit_offset),
      bit_len_(bit_len) {
    if (in_.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << input_path.generic_string() << RESET_COLOR << std::endl;
    }

    if (out_.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
    }

    in_.bit_seekg(bit_offset_, std::ios::beg);
}

void AsyncDecodeTask::run() {
    DecodeDictionary dict;

    uint4 cur_code;
    std::string prev = "";
    while (bit_len_ > 0 && in_.get_bit(cur_code, dict.get_count_bit())) {
        bit_len_ -= dict.get_count_bit();

        if (dict.find(cur_code)) {
            print_string(dict[cur_code]);

            if (prev.empty() == false) {
                dict.add(prev + dict[cur_code][0]);
            }

            prev = dict[cur_code];
            continue;
        }

        prev += prev[0];
        dict.add(prev);

        print_string(prev);
    }
}

void AsyncDecodeTask::print_string(const std::string &str) {
    for (int i = 0; i < str.size(); i++) {
        out_.write(reinterpret_cast<const char *>(&str[i]), sizeof(char));
    }
}
