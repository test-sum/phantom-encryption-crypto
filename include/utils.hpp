#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <cstdint> // Include cstdint for uint8_t

std::vector<uint8_t> hash_function(const std::vector<uint8_t>& data);
std::vector<uint8_t> xor_bytes(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
std::vector<uint8_t> generate_random_bytes(size_t length);
std::string bytes_to_hex(const std::vector<uint8_t>& bytes);
std::vector<uint8_t> hex_to_bytes(const std::string& hex);
std::vector<uint8_t> concat(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
std::vector<uint8_t> add_padding(const std::vector<uint8_t>& data, size_t block_size);
std::vector<uint8_t> remove_padding(const std::vector<uint8_t>& padded_data);

#endif // UTILS_HPP
