#include "encryption.hpp"
#include "utils.hpp"
#include <iostream> // Add this line

std::vector<uint8_t> add_padding(const std::vector<uint8_t>& data, size_t block_size) {
    size_t padding_length = block_size - (data.size() % block_size);
    std::vector<uint8_t> padded_data = data;
    padded_data.insert(padded_data.end(), padding_length, static_cast<uint8_t>(padding_length));
    return padded_data;
}

std::tuple<std::vector<uint8_t>, std::vector<std::vector<uint8_t>>, std::vector<uint8_t>, std::vector<uint8_t>, std::vector<uint8_t>>
encrypt(const std::vector<uint8_t>& plaintext, const std::vector<uint8_t>& key) {
    size_t m = key.size();
    std::vector<uint8_t> padded_plaintext = add_padding(plaintext, m);
    size_t n = padded_plaintext.size() / m;
    std::vector<std::vector<uint8_t>> P(n);

    for (size_t i = 0; i < n; ++i) {
        P[i] = std::vector<uint8_t>(padded_plaintext.begin() + i * m,
                                    padded_plaintext.begin() + (i + 1) * m);
    }

    std::vector<uint8_t> N_s = generate_random_bytes(m);
    std::vector<uint8_t> C_0 = xor_bytes(N_s, key);
    std::vector<uint8_t> nonce = generate_random_bytes(16);

    std::vector<uint8_t> K_e0 = hash_function(concat(hash_function(N_s), hash_function(key)));
    std::vector<uint8_t> P_0 = hash_function(concat(hash_function(N_s), hash_function(K_e0)));

    std::vector<std::vector<uint8_t>> K_ei(n + 1);
    K_ei[0] = K_e0;
    std::vector<std::vector<uint8_t>> C(n);

    for (size_t i = 1; i <= n; ++i) {
        if (i == 1) {
            K_ei[i] = hash_function(concat(C_0, K_ei[i-1]));
        } else {
            K_ei[i] = hash_function(concat(P[i-2], K_ei[i-1]));
        }
        C[i-1] = xor_bytes(xor_bytes(xor_bytes(P[i-1], K_ei[i]), P_0), K_e0);
    }

    std::vector<uint8_t> R = hash_function(concat(P[n-1], xor_bytes(xor_bytes(K_ei[n], P_0), K_e0)));

    std::vector<uint8_t> all_C;
    for (const auto& c : C) {
        all_C.insert(all_C.end(), c.begin(), c.end());
    }
    std::vector<uint8_t> mac = hash_function(concat(concat(all_C, R), nonce));

    return {C_0, C, R, mac, nonce};
}

std::tuple<std::vector<uint8_t>, bool>
decrypt(const std::vector<uint8_t>& C_0,
        const std::vector<std::vector<uint8_t>>& C,
        const std::vector<uint8_t>& R,
        const std::vector<uint8_t>& mac,
        const std::vector<uint8_t>& nonce,
        const std::vector<uint8_t>& key,
        std::set<std::string>& used_nonces) {
    if (nonce_used(nonce, used_nonces)) {
        std::cout << "Nonce reuse detected! Replay attack!" << std::endl;
        return std::make_tuple(std::vector<uint8_t>{}, false);
    }

    size_t m = key.size();
    size_t n = C.size();

    std::vector<uint8_t> N_r = xor_bytes(C_0, key);
    std::cout << "N_r: " << bytes_to_hex(N_r) << std::endl;

    // Precompute hash values
    std::vector<uint8_t> hash_N_r = hash_function(N_r);
    std::vector<uint8_t> hash_key = hash_function(key);

    std::vector<uint8_t> K_d0 = hash_function(concat(hash_N_r, hash_key));
    std::vector<uint8_t> M_0 = hash_function(concat(hash_N_r, hash_function(K_d0)));

    std::cout << "K_d0: " << bytes_to_hex(K_d0) << std::endl;
    std::cout << "M_0: " << bytes_to_hex(M_0) << std::endl;

    std::vector<std::vector<uint8_t>> K_di(n + 1);
    K_di[0] = K_d0;
    std::vector<std::vector<uint8_t>> M(n);

    // Minimize data copying
    std::vector<uint8_t> decrypted_message;
    decrypted_message.reserve(C.size() * C[0].size());

    for (size_t i = 1; i <= n; ++i) {
        if (i == 1) {
            K_di[i] = hash_function(concat(C_0, K_di[i-1]));
        } else {
            K_di[i] = hash_function(concat(M[i-2], K_di[i-1]));
        }
        M[i-1] = xor_bytes(xor_bytes(xor_bytes(C[i-1], K_di[i]), M_0), K_d0);

        std::cout << "K_di[" << i << "]: " << bytes_to_hex(K_di[i]) << std::endl;
        std::cout << "M[" << i-1 << "]: " << bytes_to_hex(M[i-1]) << std::endl;
        std::cout << "C[" << i-1 << "]: " << bytes_to_hex(C[i-1]) << std::endl;

        // Minimize data copying
        decrypted_message.insert(decrypted_message.end(), M[i-1].begin(), M[i-1].end());
    }

    std::vector<uint8_t> V = hash_function(concat(M[n-1], xor_bytes(xor_bytes(K_di[n], M_0), K_d0)));
    std::cout << "V: " << bytes_to_hex(V) << std::endl;
    std::cout << "R: " << bytes_to_hex(R) << std::endl;

    // Minimize data copying
    std::vector<uint8_t> all_C;
    all_C.reserve(C.size() * C[0].size());
    for (const auto& c : C) {
        all_C.insert(all_C.end(), c.begin(), c.end());
    }
    std::vector<uint8_t> calc_mac = hash_function(concat(concat(all_C, R), nonce));

    std::cout << "Calculated MAC: " << bytes_to_hex(calc_mac) << std::endl;
    std::cout << "Provided MAC: " << bytes_to_hex(mac) << std::endl;

    if (calc_mac != mac) {
        std::cout << "MAC verification failed!" << std::endl;
        return std::make_tuple(std::vector<uint8_t>{}, false);
    }

    if (V == R) {
        used_nonces.insert(bytes_to_hex(nonce));
        decrypted_message = remove_padding(decrypted_message);
        // Use move semantics
        return std::make_tuple(std::move(decrypted_message), true);
    } else {
        decrypted_message = remove_padding(decrypted_message);
        // Use move semantics
        return std::make_tuple(std::move(decrypted_message), false);
    }
}

