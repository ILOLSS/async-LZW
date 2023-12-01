#ifndef FILE_H
#define FILE_H

#include <filesystem>
#include <fstream>
#include <iostream>

#define RED "\033[31m"
#define RESET_COLOR "\033[0m"

namespace fs = std::filesystem;

typedef unsigned char uint1;   // 8  bit
typedef unsigned short uint2;  // 16 bit
typedef unsigned int uint4;    // 32 bit

class FileReader {
public:
    FileReader(const fs::path &file_path)
        : file_path_(file_path),
          stream_(file_path_, std::ios::binary),
          buffer_(0),
          first_not_used_bit_(0) {
    }

    FileReader(fs::path &&file_path)
        : file_path_(std::move(file_path)),
          stream_(file_path_, std::ios::binary),
          buffer_(0),
          first_not_used_bit_(0) {
    }

    FileReader(FileReader &&) = default;
    FileReader &operator=(FileReader &&) = default;

    ~FileReader();

    bool get_bit(uint4 &buf, int count);

    bool is_open() const;
    void close();
    int size() const;

    int tellg();
    void seekg(int pos);
    void seekg(int off, std::ios_base::seekdir dir);
    void bit_seekg(int bit_off, std::ios_base::seekdir dir);

private:
    bool read_buffer();

private:
    fs::path file_path_;
    std::ifstream stream_;
    uint1 buffer_;
    uint1 first_not_used_bit_;
};

class FileWriter {
public:
    FileWriter(const fs::path &file_path)
        : file_path_(file_path),
          stream_(file_path_, std::ios::binary),
          buffer_(0),
          first_not_used_bit_(128) {
    }

    FileWriter(fs::path &&file_path)
        : file_path_(std::move(file_path)),
          stream_(file_path_, std::ios::binary),
          buffer_(0),
          first_not_used_bit_(128) {
    }

    FileWriter(FileWriter &&) = default;
    FileWriter &operator=(FileWriter &&) = default;

    ~FileWriter();

    void put_bit(uint4 data, int count);

    bool is_open() const;
    void close();
    int size() const;

private:
    void flush();

private:
    fs::path file_path_;
    std::ofstream stream_;
    uint1 buffer_;
    uint1 first_not_used_bit_;
};

#endif  // FILE_H