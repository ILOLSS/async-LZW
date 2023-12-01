#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include <string>
#include <unordered_map>

#define MAX_SIZE 65535

typedef unsigned char uint1;   // 8  bit
typedef unsigned short uint2;  // 16 bit
typedef unsigned int uint4;    // 32 bit

class EncodeDictionary {
public:
    EncodeDictionary();

    void clear();
    void init();

    bool find(const std::string &key) const;
    void add(const std::string &key);

    uint4 get_count_bit() const;
    const uint4 &operator[](const std::string &key);

private:
    std::unordered_map<std::string, uint4> dictionary_;
    uint4 free_code_;
    uint4 count_bit_;
};

class DecodeDictionary {
public:
    DecodeDictionary();

    void clear();
    void init();

    bool find(uint4 key) const;
    void add(const std::string &value);

    uint4 get_count_bit() const;
    const std::string &operator[](uint4 key);

private:
    std::unordered_map<uint4, std::string> dictionary_;
    uint4 free_code_;
    uint4 count_bit_;
};

#endif  // DICTIONARY_H_