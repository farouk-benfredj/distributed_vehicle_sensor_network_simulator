FROM ubuntu:24.04

RUN apt update && apt install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    libboost-all-dev \
    libssl-dev \
    curl && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /opt
RUN git clone --branch master https://github.com/COVESA/vsomeip.git && \
    cd vsomeip && mkdir build && cd build && \
    cmake .. && \
    make -j$(nproc) && \
    make install && ldconfig

WORKDIR /app
COPY . .

# RUN g++ -std=c++23 \
#     -Iinclude \
#     src/*.cc \
#     -lvsomeip3 -lboost_system -lpthread \
#     -o /usr/local/bin/app

ENTRYPOINT ["app"]
