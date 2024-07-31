#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "encryption.hpp"
#include "utils.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <path to input file> <256-bit key in hex>" << std::endl;
        return 1;
    }

    std::string input_file = argv[1];
    std::string key_hex = argv[2];

    std::vector<uint8_t> key(key_hex.size() / 2);
    for (size_t i = 0; i < key.size(); ++i) {
        sscanf(key_hex.c_str() + 2*i, "%2hhx", &key[i]);
    }

    std::ifstream file(input_file, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return 1;
    }
    std::vector<uint8_t> plaintext((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    auto [C_0, C, R, mac, nonce] = encrypt(plaintext, key);

    json ciphertext_json;
    ciphertext_json["filename"] = input_file.substr(input_file.find_last_of("/\\") + 1);
    ciphertext_json["C_0"] = bytes_to_hex(C_0);
    for (const auto& c : C) {
        ciphertext_json["C"].push_back(bytes_to_hex(c));
    }
    ciphertext_json["R"] = bytes_to_hex(R);
    ciphertext_json["MAC"] = bytes_to_hex(mac);
    ciphertext_json["nonce"] = bytes_to_hex(nonce);

    std::ofstream output_file("ciphertext.json");
    if (!output_file) {
        std::cerr << "Error: Unable to open output file." << std::endl;
        return 1;
    }
    output_file << ciphertext_json.dump(4);

    std::cout << "Encryption complete. Ciphertext saved to ciphertext.json." << std::endl;

    return 0;
}
