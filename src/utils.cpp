#include "../include/utils.hpp"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <cstdint> // Include cstdint for uint8_t

std::vector<uint8_t> hash_function(const std::vector<uint8_t>& data) {
    std::vector<uint8_t> hash(EVP_MAX_MD_SIZE);
    unsigned int hash_len;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(ctx, data.data(), data.size());
    EVP_DigestFinal_ex(ctx, hash.data(), &hash_len);
    EVP_MD_CTX_free(ctx);

    hash.resize(hash_len);
    return hash;
}

std::vector<uint8_t> xor_bytes(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint8_t> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

std::vector<uint8_t> generate_random_bytes(size_t length) {
    std::vector<uint8_t> random_bytes(length);
    RAND_bytes(random_bytes.data(), length);
    return random_bytes;
}

std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (const auto& byte : bytes) {
        ss << std::setw(2) << static_cast<int>(byte);
    }
    return ss.str();
}

std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
    std::vector<uint8_t> bytes;
    for (unsigned int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        uint8_t byte = (uint8_t) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

std::vector<uint8_t> concat(const std::vector<uint8_t>& a, const std::vector<uint8_t>& b) {
    std::vector<uint8_t> result;
    result.reserve(a.size() + b.size());
    result.insert(result.end(), a.begin(), a.end());
    result.insert(result.end(), b.begin(), b.end());
    return result;
}

std::vector<uint8_t> add_padding(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_length = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded_data = data;
    padded_data.insert(padded_data.end(), padding_length, static_cast<uint8_t>(padding_length));
    return padded_data;
}

std::vector<uint8_t> remove_padding(const std::vector<uint8_t>& padded_data) {
    size_t padding_length = padded_data.back();
    if (padding_length > padded_data.size() || padding_length == 0) {
        throw std::runtime_error("Invalid padding length");
    }
    return std::vector<uint8_t>(padded_data.begin(), padded_data.end() - padding_length);
}
