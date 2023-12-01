#include "file.hpp"

// FileReader

bool FileReader::get_bit(uint4 &buf, int count) {
    buf = 0;
    for (int i = 0; i < count; i++) {
        if (first_not_used_bit_ == 0) {
            if (!read_buffer()) {
                return false;
            }
        }
        buf = (buf << 1) + static_cast<bool>(buffer_ & first_not_used_bit_);
        first_not_used_bit_ >>= 1;
    }
    return true;
}

bool FileReader::is_open() const {
    return stream_.is_open();
}

void FileReader::close() {
    stream_.close();
}

int FileReader::size() const {
    return fs::file_size(file_path_);
}

int FileReader::tellg() {
    return stream_.tellg();
}

void FileReader::seekg(int pos) {
    stream_.seekg(pos);
    first_not_used_bit_ = 0;
}

void FileReader::seekg(int off, std::ios_base::seekdir dir) {
    stream_.seekg(off, dir);
    first_not_used_bit_ = 0;
}

void FileReader::bit_seekg(int bit_off, std::ios_base::seekdir dir) {
    int byte_off = bit_off / 8;
    bit_off = bit_off % 8;
    seekg(byte_off, dir);
    read_buffer();
    for (int i = 0; i < bit_off; i++) {
        first_not_used_bit_ >>= 1;
    }
}

bool FileReader::read_buffer() {
    bool is_good = bool(stream_.read(reinterpret_cast<char *>(&buffer_), 1));
    if (!is_good) {
        // std::cerr << RED << "[WARNING] FileReader: EOF" << RESET_COLOR
        //           << std::endl;
        return false;
    }
    first_not_used_bit_ = 128;
    return true;
}

FileReader::~FileReader() {
    if (stream_.is_open() == true) {
        stream_.close();
    }
}

// FileWriter

void FileWriter::put_bit(uint4 data, int count) {
    for (int i = count - 1; i >= 0; i--) {
        if (first_not_used_bit_ == 0) {
            flush();
        }
        if (data & (1 << i)) {
            buffer_ ^= first_not_used_bit_;
        }
        first_not_used_bit_ >>= 1;
    }
}

bool FileWriter::is_open() const {
    return stream_.is_open();
}

void FileWriter::close() {
    if (first_not_used_bit_ != 128) {
        flush();
    }
    stream_.close();
}

int FileWriter::size() const {
    return fs::file_size(file_path_);
}

void FileWriter::flush() {
    stream_.write(reinterpret_cast<const char *>(&buffer_), 1);
    buffer_ = 0;
    first_not_used_bit_ = 128;
}

FileWriter::~FileWriter() {
    if (first_not_used_bit_ != 128) {
        flush();
    }
    if (stream_.is_open() == true) {
        stream_.close();
    }
}