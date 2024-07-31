#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <string>
#include <set> // Add this line

std::vector<uint8_t> hash_function(const std::vector<uint8_t>& data);
std::vector<uint8_t> xor_bytes(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
std::vector<uint8_t> generate_random_bytes(size_t length);
std::string bytes_to_hex(const std::vector<uint8_t>& bytes);
std::vector<uint8_t> hex_to_bytes(const std::string& hex);
std::vector<uint8_t> concat(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b);
std::vector<uint8_t> remove_padding(const std::vector<uint8_t>& padded_data);
bool nonce_used(const std::vector<uint8_t>& nonce, std::set<std::string>& used_nonces);

#endif // UTILS_HPP

