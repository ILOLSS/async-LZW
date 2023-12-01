#include "async_lzw.hpp"

void async_lzw::AsyncLZW::encode(
    const fs::path &input_path,
    const fs::path &output_path
) {
    uint1 cnt_of_chunks = DEFAULT_COUNT_OF_CHUNKS;
    uint4 input_file_size = fs::file_size(input_path);
    uint4 chunk_size;
    if (input_file_size % cnt_of_chunks == 0) {
        chunk_size = input_file_size / cnt_of_chunks;
    } else {
        chunk_size = input_file_size / cnt_of_chunks + 1;
    }

    // create tmp file
    fs::create_directory("encode_tmp");
    std::vector<fs::path> tmp_paths(cnt_of_chunks);

    std::vector<std::future<int>> futures(cnt_of_chunks);

    for (int i = 0; i < cnt_of_chunks; i++) {
        tmp_paths[i] = "encode_tmp/" + std::to_string(i) + "_tmp.din";

        uint4 offset = i * chunk_size;

        // create task
        std::unique_ptr<AsyncEncodeTask> task_ptr =
            std::make_unique<AsyncEncodeTask>(
                input_path, tmp_paths[i], offset, chunk_size
            );
        futures[i] = std::move(task_ptr->get_future());
        tasks_pool.submit(std::move(task_ptr));
    }

    // append of tmp file to output_file
    std::ofstream encode_file(output_path, std::ios::binary);

    if (encode_file.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
    }

    encode_file.write(reinterpret_cast<const char *>(&cnt_of_chunks), 1);

    for (int i = 0; i < cnt_of_chunks; i++) {
        uint4 cnt_bits = futures[i].get();
        std::ifstream tmp_file(tmp_paths[i], std::ios::binary);

        if (tmp_file.is_open() == false) {
            std::cerr << RED << "[WARNING] Canot open file: " << tmp_paths[i]
                      << RESET_COLOR << std::endl;
        }

        // write cnt bites in this chunk
        encode_file.write(reinterpret_cast<const char *>(&cnt_bits), 4);

        encode_file << tmp_file.rdbuf();
        tmp_file.close();
    }

    encode_file.close();

    // delete tmp dir
    fs::remove_all("encode_tmp");
}

void async_lzw::AsyncLZW::decode(
    const fs::path &input_path,
    const fs::path &output_path
) {
    FileReader in(input_path);

    if (in.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << input_path.generic_string() << RESET_COLOR << std::endl;
    }

    uint4 cnt_of_chunks;
    in.get_bit(cnt_of_chunks, 8);
    uint4 bit_offset = 8;

    // create tmp file
    fs::create_directory("decode_tmp");
    std::vector<fs::path> tmp_paths(cnt_of_chunks);

    std::vector<std::future<void>> futures(cnt_of_chunks);

    for (int i = 0; i < cnt_of_chunks; i++) {
        tmp_paths[i] = "decode_tmp/" + std::to_string(i) + "_tmp.txt";

        // read 32 bits for bit_cbunk_size, because this butes is reversed
        uint4 bit_chunk_size = 0;
        uint4 temp = 0;
        in.get_bit(temp, 8);
        bit_chunk_size += temp;
        in.get_bit(temp, 8);
        bit_chunk_size += (temp << 8);
        in.get_bit(temp, 8);
        bit_chunk_size += (temp << 16);
        in.get_bit(temp, 8);
        bit_chunk_size += (temp << 24);

        bit_offset += 32;

        // create task
        std::unique_ptr<AsyncDecodeTask> task_ptr =
            std::make_unique<AsyncDecodeTask>(
                input_path, tmp_paths[i], bit_offset, bit_chunk_size
            );
        futures[i] = std::move(task_ptr->get_future());
        tasks_pool.submit(std::move(task_ptr));

        if (bit_chunk_size % 8 != 0) {
            bit_chunk_size += 8 - bit_chunk_size % 8;
        }
        in.bit_seekg(bit_chunk_size, std::ios::cur);
        bit_offset += bit_chunk_size;
    }

    in.close();

    // append of tmp file to output_file
    std::ofstream decode_file(output_path, std::ios::binary);

    if (decode_file.is_open() == false) {
        std::cerr << RED << "[WARNING] Canot open file: "
                  << output_path.generic_string() << RESET_COLOR << std::endl;
    }

    for (int i = 0; i < cnt_of_chunks; i++) {
        futures[i].get();
        std::ifstream tmp_file(tmp_paths[i], std::ios::binary);

        if (tmp_file.is_open() == false) {
            std::cerr << RED << "[WARNING] Canot open file: " << tmp_paths[i]
                      << RESET_COLOR << std::endl;
        }

        decode_file << tmp_file.rdbuf();
        tmp_file.close();
    }

    decode_file.close();

    // delete tmp dir
    fs::remove_all("decode_tmp");
}