FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    wget git cmake build-essential libssl-dev python3 libasio-dev\
    && rm -rf /var/lib/apt/lists/*

# Install MongoDB C++ Driver
RUN git clone https://github.com/mongodb/mongo-cxx-driver.git \
    && cd mongo-cxx-driver/build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local \
    && make -j$(nproc) && make install

# Install Crow (header-only library)
RUN wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/Crow-1.2.0-Linux.deb \
    && dpkg -i Crow-1.2.0-Linux.deb

# Copy app source
WORKDIR /app
COPY src/* .

ENV LD_LIBRARY_PATH="/usr/local/lib:$LD_LIBRARY_PATH"

# Build the C++ application
RUN g++ -o api api.cpp \
    -I/usr/local/include \
    -I/usr/local/include/mongocxx/v_noabi \
    -I/usr/local/include/bsoncxx/v_noabi \
    -lmongocxx -lbsoncxx -lpthread -lssl -lcrypto

    # Build the C++ application
RUN g++ -o cmd cmd.cpp \
    -I/usr/local/include \
    -I/usr/local/include/mongocxx/v_noabi \
    -I/usr/local/include/bsoncxx/v_noabi \
    -lmongocxx -lbsoncxx -lpthread -lssl -lcrypto

# Run the app
CMD ["./api"]