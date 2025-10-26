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

RUN echo 'export PS1='\''\[\e[32m\]\u@\h\[\e[0m\]:\[\e[34m\]\w\[\e[0m\]$(if git rev-parse --git-dir > /dev/null 2>&1; then echo " \[\e[33m\]$(git branch --show-current)"; tag=$(git describe --tags --exact-match 2>/dev/null); if [ -n "$tag" ]; then echo " \[\e[35m\]$tag"; fi; fi)$(if [ $? -ne 0 ]; then echo " \[\e[31m\]<E2><9C><97>"; fi)\[\e[0m\] $ '\'' ' >> /root/.bashrc

# RUN g++ -std=c++23 \
#     -Iinclude \
#     src/*.cc \
#     -lvsomeip3 -lboost_system -lpthread \
#     -o /usr/local/bin/app
