#include <algorithm>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "async_lzw.hpp"
#include "exec_time.hpp"
#include "file.hpp"
#include "lzw.hpp"

int main(int argc, char *argv[]) {
    // fs::path input_path =
    //     "/Users/ilolss/Desktop/multithread LZW/tests/test1.txt";
    // fs::path encode_path = "/Users/ilolss/Desktop/multithread
    // LZW/encode.bin"; fs::path output_path =
    //     "/Users/ilolss/Desktop/multithread LZW/tests/sync_test1.txt";

    if (argc < 4) {
        std::cerr << "[ERROR] too few arguments" << std::endl;
        return 0;
    }

    fs::create_directory("tmp");

    // bool is_async = true;
    bool is_async = std::stoi(argv[1]);

    fs::path input_path = argv[2];
    fs::path encode_path = "/tmp/encode.bin";
    fs::path output_path = argv[3];

    if (is_async) {
        //----------------------------------------------------------------
        // async LZW
        //----------------------------------------------------------------

        async_lzw::AsyncLZW lzw;
        ExecTime<void> encode([&]() { lzw.encode(input_path, encode_path); });
        encode("async encode");
        ExecTime<void> decode([&]() { lzw.decode(encode_path, output_path); });
        decode("async decode");

    } else {
        //----------------------------------------------------------------
        // sync LZW
        //----------------------------------------------------------------

        ExecTime<void> encode(lzw::encode, input_path, encode_path);
        encode("sync encode");
        ExecTime<void> decode(lzw::decode, encode_path, output_path);
        decode("sync decode");
    }

    fs::remove_all("tmp");
}
