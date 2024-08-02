CXX = g++
CXXFLAGS = -std=c++17 -I./include

all: phantom_encrypt phantom_decrypt

phantom_encrypt: src/main_encrypt.cpp src/encryption.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o phantom_encrypt src/main_encrypt.cpp src/encryption.cpp src/utils.cpp -lssl -lcrypto

phantom_decrypt: src/main_decrypt.cpp src/encryption.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o phantom_decrypt src/main_decrypt.cpp src/encryption.cpp src/utils.cpp -lssl -lcrypto

clean:
	rm -f phantom_encrypt phantom_decrypt
