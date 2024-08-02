#include "../include/encryption.hpp"
#include "../include/utils.hpp"
#include <cstdint> // Include cstdint for uint8_t

std::tuple<std::vector<uint8_t>, std::vector<std::vector<uint8_t>>, std::vector<uint8_t>>
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

    std::vector<uint8_t> K_e0 = hash_function(concat(hash_function(concat(N_s, key)), hash_function(concat(key, N_s))));
    std::vector<uint8_t> P_0 = hash_function(concat(hash_function(concat(N_s, K_e0)), hash_function(concat(K_e0, N_s))));

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

    return {C_0, C, R};
}

std::tuple<std::vector<uint8_t>, bool>
decrypt(const std::vector<uint8_t>& C_0,
        const std::vector<std::vector<uint8_t>>& C,
        const std::vector<uint8_t>& R,
        const std::vector<uint8_t>& key) {
    size_t m = key.size();
    size_t n = C.size();

    std::vector<uint8_t> N_r = xor_bytes(C_0, key);

    std::vector<uint8_t> K_d0 = hash_function(concat(hash_function(concat(N_r, key)), hash_function(concat(key, N_r))));
    std::vector<uint8_t> M_0 = hash_function(concat(hash_function(concat(N_r, K_d0)), hash_function(concat(K_d0, N_r))));

    std::vector<std::vector<uint8_t>> K_di(n + 1);
    K_di[0] = K_d0;
    std::vector<std::vector<uint8_t>> M(n);

    for (size_t i = 1; i <= n; ++i) {
        if (i == 1) {
            K_di[i] = hash_function(concat(C_0, K_di[i-1]));
        } else {
            K_di[i] = hash_function(concat(M[i-2], K_di[i-1]));
        }
        M[i-1] = xor_bytes(xor_bytes(xor_bytes(C[i-1], K_di[i]), M_0), K_d0);
    }

    std::vector<uint8_t> V = hash_function(concat(M[n-1], xor_bytes(xor_bytes(K_di[n], M_0), K_d0)));

    if (V == R) {
        std::vector<uint8_t> decrypted_message;
        for (const auto& m : M) {
            decrypted_message.insert(decrypted_message.end(), m.begin(), m.end());
        }
        decrypted_message = remove_padding(decrypted_message);
        return std::make_tuple(decrypted_message, true);
    } else {
        std::vector<uint8_t> decrypted_message;
        for (const auto& m : M) {
            decrypted_message.insert(decrypted_message.end(), m.begin(), m.end());
        }
        decrypted_message = remove_padding(decrypted_message);
        return std::make_tuple(decrypted_message, false);
    }
}
