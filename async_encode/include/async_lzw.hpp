#ifndef ASYNC_LZW_H_
#define ASYNC_LZW_H_

#include <filesystem>
#include <future>
#include <string>
#include <vector>
#include "async_lzw_task.hpp"
#include "async_tasks_pool.hpp"
#include "file.hpp"

#define DEFAULT_COUNT_OF_CHUNKS 8

namespace async_lzw {

namespace fs = std::filesystem;

typedef unsigned char uint1;   // 8  bit
typedef unsigned short uint2;  // 16 bit
typedef unsigned int uint4;    // 32 bit

class AsyncLZW {
public:
    void encode(const fs::path &input_path, const fs::path &output_path);
    void decode(const fs::path &input_path, const fs::path &output_path);

private:
    void read_32bit() {
    }

private:
    AsyncTaskPool tasks_pool;
};

}  // namespace async_lzw

#endif  // ASYNC_LZW_H_