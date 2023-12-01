#include "dictionary.hpp"

EncodeDictionary::EncodeDictionary() {
    init();
}

void EncodeDictionary::clear() {
    dictionary_.clear();
    init();
}

void EncodeDictionary::init() {
    for (int i = 0; i < 256; i++) {
        dictionary_[std::string(1, static_cast<char>(i))] = i;
    }
    free_code_ = 256;
    count_bit_ = 9;
}

bool EncodeDictionary::find(const std::string &key) const {
    return dictionary_.find(key) != dictionary_.end();
}

void EncodeDictionary::add(const std::string &key) {
    dictionary_[key] = free_code_++;

    if (free_code_ >= (1 << count_bit_)) {
        count_bit_++;
    }

    if (free_code_ > MAX_SIZE) {
        init();
    }
}

uint4 EncodeDictionary::get_count_bit() const {
    return count_bit_;
}

const uint4 &EncodeDictionary::operator[](const std::string &key) {
    return dictionary_[key];
}

DecodeDictionary::DecodeDictionary() {
    init();
}

void DecodeDictionary::clear() {
    dictionary_.clear();
    init();
}

void DecodeDictionary::init() {
    for (int i = 0; i < 256; i++) {
        dictionary_[i] = std::string(1, static_cast<char>(i));
    }
    free_code_ = 256;
    count_bit_ = 9;
}

bool DecodeDictionary::find(uint4 key) const {
    return dictionary_.find(key) != dictionary_.end();
}

void DecodeDictionary::add(const std::string &value) {
    dictionary_[free_code_++] = value;

    if (free_code_ >= (1 << count_bit_) - 1) {
        count_bit_++;
    }

    if (free_code_ > MAX_SIZE) {
        init();
    }
}

uint4 DecodeDictionary::get_count_bit() const {
    return count_bit_;
}

const std::string &DecodeDictionary::operator[](uint4 key) {
    return dictionary_[key];
}