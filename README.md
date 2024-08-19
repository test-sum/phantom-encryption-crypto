# PHANTOM Cryptographic Encryption Algorithm

## Overview

PHANTOM is an implementation of a new post-quantum cryptographic algorithm based on the research of Dr. Luu Hong Dung, as detailed in his paper "THE POST-QUANTUM CRYPTOGRAPHIC ALGORITHM BASED ON OTP CIPHER AND HASH FUNCTIONS". It combines OTP-like security with practical applicability for arbitrary message lengths, aiming to be resistant against quantum attacks. Unlike traditional OTP which requires a key as long as the message, PHANTOM can encrypt messages and files larger than the key size, making it practical for real-world use.

### Why PHANTOM is Different

PHANTOM stands out:

- **Perfect Secrecy**: Inherits the "perfect secrecy" property from OTP, ensuring that there is no relationship between the plaintext and the ciphertext, making it impossible for an attacker to derive the plaintext from the ciphertext.
- **Scalable for Large Messages**: Encrypts messages/files larger than the key size, making it practical for real-world use.
- **Quantum Resistance**: Designed to resist attacks even with the assistance of quantum computers, making it a strong candidate for post-quantum cryptography.

This implementation of PHANTOM provides a practical realization of Dr. Luu's work.

## Features

- **Post-quantum resistant encryption**: Secure against quantum computing attacks.
- **OTP-like security properties**: Provides a high level of security inspired by the One-Time Pad.
- **Supports large messages**: Can encrypt messages/files larger than the key size.
- **File encryption and decryption**: Command-line tools for file encryption and decryption.

## Performance
On AWS EC" t2.small (1vCPU and 2GB Memory)
- **Encryption**: 2.5 MB/s
- **Decryption**: 2.5 MB/s

( I have not spent much time on optimisations just yet, but currently using SHA256, and moving to Blake2 should yeild savings)

## Installation

### Prebuilt Binaries

Prebuilt standalone binaries (with all dependencies included) are available for Debian 12 and should work on Ubuntu and other similar distributions.

1. Download the prebuilt binaries from the 'build' folder.
2. Run the binaries directly.

### Building from Source

#### Prerequisites

- OpenSSL (version 1.1.1 or newer)
- C++17 compatible compiler (e.g., g++)
- Make

#### Steps

1. Clone the repository:

   ```sh
   git clone https://github.com/test-sum/phantom-encryption-crypto.git
   cd phantom-encryption-crypto/
   ```

2. Ensure OpenSSL, Make and g++  is installed. On Debian/Ubuntu, you can install it using:

   ```sh
   sudo apt update
   sudo apt install git libssl-dev make g++
   ```

3. Build the project:

   ```sh
   make
   ```

This will generate two executables: `encrypt` and `decrypt`.

## Usage

### Encryption

To encrypt a file:

```sh
./encrypt <path to input file> <256-bit key in hex>
```

Example:

```sh
./encrypt message.txt aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899
```

This will produce `ciphertext.json` in the same directory.

![image](https://github.com/user-attachments/assets/0df25fe2-1ea3-4408-bcb2-6586c9d7e1f8)



### Decryption

To decrypt a file:

```sh
./decrypt <path to ciphertext JSON file> <256-bit key in hex>
```

Example:

```sh
./decrypt ciphertext.json aabbccddeeff00112233445566778899aabbccddeeff00112233445566778899
```

This will produce the decrypted file with the prefix `decrypted_` in the same directory.

![image](https://github.com/user-attachments/assets/b6fddfc4-a030-47e5-bfbc-ea9346dce98b)




## Dependencies

- **OpenSSL**: Used for cryptographic functions (version 1.1.1 or newer)
- **nlohmann/json**: JSON library for C++ (included as `json.hpp`)

## Security Considerations

- **Key Management**: Ensure your keys are kept secure and not reused.
- **Nonce Usage**: Each encryption must use a unique nonce to prevent replay attacks.
- **File Security**: Encrypted files should be transmitted and stored securely to prevent unauthorized access.

## Contributing

We welcome contributions to the PHANTOM project. Please fork the repository and submit pull requests for review.

### Guidelines

1. Follow the existing code style.
2. Include tests for new features.
3. Ensure all existing tests pass.

## Research Background

This implementation is based on the paper "THE POST-QUANTUM CRYPTOGRAPHIC ALGORITHM BASED ON OTP CIPHER AND HASH FUNCTIONS" by Dr. Luu Hong Dung. The original paper focuses on the theoretical aspects of a post-quantum cryptographic algorithm, while this implementation provides a practical realization with additional features such as file encryption, MAC for authentication, and nonce for replay attack prevention.

### Key Principles from the Paper

- **Perfect Secrecy**: According to Shannon's theory, if the key is a random bit sequence, there is no relationship between the plaintext and the ciphertext, achieving "perfect secrecy". This means that even with brute force, an attacker cannot find the plaintext from the ciphertext.
- **Hash Function Utilization**: The algorithm uses hash functions to generate encryption keys, ensuring randomness and resistance to attacks. Even if the one-way property of the hash function is broken, the attacker cannot derive the secret key.
- **Spoofing Attack Resistance**: Unlike traditional OTP, PHANTOM verifies the origin and integrity of the message using a MAC, preventing attackers from sending fake ciphertexts.

You can find the original research paper here:
[THE POST-QUANTUM CRYPTOGRAPHIC ALGORITHM BASED ON OTP CIPHER AND HASH FUNCTIONS](https://www.researchgate.net/publication/382455890_THE_POST-_QUANTUM_CRYPTOGRAPHIC_ALGORITHM_BASED_ON_OTP_CIPHER_AND_HASH_FUNCTIONS)

## License

This project is licensed under the CC BY 4.0 License

## Contact

For questions or collaboration, please reach out to luuhongdung@mta.edu.vn and jamie.brian.gilchrist@gmail.com

## Acknowledgements

- **Dr. Luu Hong Dung**: For the original research on which this implementation is based.
- **Contributors to this repo**: Jamie Gilchrist
