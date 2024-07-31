#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include "encryption.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <path to ciphertext JSON file> <256-bit key in hex>" << std::endl;
        return 1;
    }

    std::string ciphertext_file = argv[1];
    std::string key_hex = argv[2];

    std::vector<uint8_t> key(key_hex.size() / 2);
    for (size_t i = 0; i < key.size(); ++i) {
        sscanf(key_hex.c_str() + 2*i, "%2hhx", &key[i]);
    }

    std::ifstream file(ciphertext_file);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open ciphertext file." << std::endl;
        return 1;
    }

    json ciphertext_json;
    file >> ciphertext_json;

    std::string filename = ciphertext_json["filename"];
    std::vector<uint8_t> C_0 = hex_to_bytes(ciphertext_json["C_0"]);
    std::vector<std::vector<uint8_t>> C;
    for (const auto& c : ciphertext_json["C"]) {
        C.push_back(hex_to_bytes(c));
    }
    std::vector<uint8_t> R = hex_to_bytes(ciphertext_json["R"]);
    std::vector<uint8_t> mac = hex_to_bytes(ciphertext_json["MAC"]);
    std::vector<uint8_t> nonce = hex_to_bytes(ciphertext_json["nonce"]);

    std::cout << "Filename: " << filename << std::endl;
    std::cout << "C_0: " << bytes_to_hex(C_0) << std::endl;
    for (size_t i = 0; i < C.size(); ++i) {
        std::cout << "C[" << i << "]: " << bytes_to_hex(C[i]) << std::endl;
    }
    std::cout << "Read R: " << bytes_to_hex(R) << std::endl;
    std::cout << "Read MAC: " << bytes_to_hex(mac) << std::endl;
    std::cout << "Read Nonce: " << bytes_to_hex(nonce) << std::endl;

    std::set<std::string> used_nonces;
    auto [decrypted_message, is_authentic] = decrypt(C_0, C, R, mac, nonce, key, used_nonces);

    std::string output_file = "decrypted_" + filename;
    std::ofstream out(output_file, std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "Error: Unable to open output file for writing." << std::endl;
        return 1;
    }
    out.write(reinterpret_cast<const char*>(decrypted_message.data()), decrypted_message.size());

    std::cout << "Decrypted Message written to: " << output_file << std::endl;
    std::cout << "Is Authentic: " << (is_authentic ? "Yes" : "No") << std::endl;

    return 0;
}
