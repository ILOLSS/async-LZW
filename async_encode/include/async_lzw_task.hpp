#ifndef ASYNC_LZW_TASK_
#define ASYNC_LZW_TASK_

#include <filesystem>
#include <fstream>
#include "async_task.hpp"
#include "dictionary.hpp"
#include "file.hpp"

namespace fs = std::filesystem;

typedef unsigned char uint1;   // 8  bit
typedef unsigned short uint2;  // 16 bit
typedef unsigned int uint4;    // 32 bit

class AsyncEncodeTask : public AsyncTask<int> {
public:
    AsyncEncodeTask(
        const fs::path &input_path,
        const fs::path &output_path,
        uint4 offset,
        uint4 len
    );

protected:
    int run() override;

private:
    std::ifstream in_;
    FileWriter out_;
    uint4 offset_;
    uint4 len_;
};

class AsyncDecodeTask : public AsyncTask<void> {
public:
    AsyncDecodeTask(
        const fs::path &input_path,
        const fs::path &output_path,
        uint4 bit_offset,
        uint4 bit_len
    );

protected:
    void run() override;

private:
    void print_string(const std::string &str);

private:
    FileReader in_;
    std::ofstream out_;
    uint4 bit_offset_;
    uint4 bit_len_;
};

#endif  // ASYNC_LZW_TASK_