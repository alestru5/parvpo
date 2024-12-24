FROM gcc:latest

WORKDIR /app

COPY . .

RUN apt-get update && apt-get install -y \
    libpq-dev \
    libpqxx-dev \
    libprotobuf-dev \
    protobuf-compiler \
    libgrpc-dev \
    libgrpc++-dev \
    && rm -rf /var/lib/apt/lists/*

RUN git clone -b v1.54.2 --depth 1 --recurse-submodules https://github.com/grpc/grpc /grpc

WORKDIR /grpc
RUN mkdir -p cmake/build
WORKDIR /grpc/cmake/build
RUN cmake ../.. && \
    make -j4 && \
    make install

ENV PATH=$PATH:/usr/local/bin

WORKDIR /app

COPY . .

RUN mkdir -p /tmp/protoc && \
    curl -L https://github.com/google/protobuf/releases/download/v3.2.0/protoc-3.2.0-linux-x86_64.zip > /tmp/protoc/protoc.zip && \
    cd /tmp/protoc && \
    unzip protoc.zip && \
    cp /tmp/protoc/bin/protoc /usr/local/bin && \
    cd /tmp && \
    rm -r /tmp/protoc

RUN protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` notification_service.proto
RUN protoc --cpp_out=. notification_service.proto

RUN g++ -std=c++20 -o notification_service notification_service.cpp `pkg-config --cflags --libs grpc++` -lpqxx -lpq -lpthread

CMD ["./notification_service"]
