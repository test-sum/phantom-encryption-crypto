#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <vector>
#include <tuple>
#include <string>
#include <set>
#include "json.hpp" // Include JSON library

using json = nlohmann::json;

std::vector<uint8_t> add_padding(const std::vector<uint8_t>& data, size_t block_size);
std::tuple<std::vector<uint8_t>, std::vector<std::vector<uint8_t>>, std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key);
std::tuple<std::vector<uint8_t>, bool>
decrypt(const std::vector<uint8_t>& C_0,
        const std::vector<std::vector<uint8_t>>& C,
        const std::vector<uint8_t>& R,
        const std::vector<uint8_t>& mac,
        const std::vector<uint8_t>& nonce,
        const std::vector<uint8_t>& key,
        std::set<std::string>& used_nonces);

#endif // ENCRYPTION_HPP
