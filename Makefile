CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -I.
LDFLAGS = -L/home/admin/pqc/c++/openssl_static/lib -lssl -lcrypto -static-libgcc -static-libstdc++ -Wl,-Bstatic -Wl,-Bdynamic -ldl -lpthread

SRCS = src/encryption.cpp src/utils.cpp
OBJS = $(SRCS:.cpp=.o)

all: encrypt decrypt

encrypt: $(OBJS) src/main_encrypt.o
	$(CXX) -o $@ $^ $(LDFLAGS)

decrypt: $(OBJS) src/main_decrypt.o
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) src/main_encrypt.o src/main_decrypt.o encrypt decrypt

.PHONY: all clean

