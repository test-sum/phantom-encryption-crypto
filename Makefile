CXX = g++
CXXFLAGS = -std=c++17 -I./include

all: encrypt decrypt

encrypt: src/main_encrypt.cpp src/encryption.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o encrypt src/main_encrypt.cpp src/encryption.cpp src/utils.cpp -lssl -lcrypto

decrypt: src/main_decrypt.cpp src/encryption.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o decrypt src/main_decrypt.cpp src/encryption.cpp src/utils.cpp -lssl -lcrypto

clean:
	rm -f encrypt decrypt
