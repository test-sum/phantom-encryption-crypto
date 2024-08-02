#ifndef ENCRYPTION_HPP
#define ENCRYPTION_HPP

#include <vector>
#include <tuple>
#include <cstdint>

std::tuple<std::vector<uint8_t>, std::vector<std::vector<uint8_t>>, std::vector<uint8_t>>
encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key);

std::tuple<std::vector<uint8_t>, bool>
decrypt(const std::vector<uint8_t>& C_0,
        const std::vector<std::vector<uint8_t>>& C,
        const std::vector<uint8_t>& R,
        const std::vector<uint8_t>& key);

#endif // ENCRYPTION_HPP
