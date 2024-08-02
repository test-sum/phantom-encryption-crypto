#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "../include/encryption.hpp"
#include "../include/utils.hpp"
#include "../include/json.hpp" // Include JSON library
#include <cstdint> // Include cstdint for uint8_t

using json = nlohmann::json;

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

    auto [decrypted_message, is_authentic] = decrypt(C_0, C, R, key);

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
