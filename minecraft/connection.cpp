#include <iostream>

#include <asio.hpp>
#include <fmt/core.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

#include "minecraft/packet.h"
#include "minecraft/varint.h"
#include "util/dump.h"

using asio::ip::tcp;

constexpr std::string_view serverName = "localhost";

void writePacket(std::vector<uint8_t> &data, tcp::socket &socket) {
    auto length = data.size();
    std::vector<uint8_t> packet = {};
    writeVarint(length, packet);
    packet.insert(packet.end(), data.begin(), data.end());

    fmt::print("<<\n");
    dump(packet);

    asio::error_code error;
    asio::write(socket, asio::buffer(packet), error);
    if (error) {
        throw asio::system_error(error);
    }
}

void writePing(tcp::socket &socket) {
    std::vector<uint8_t> packet = {};
    {
        auto ser = packetSerializer{packet};
        // handshake type
        ser.writeVarint(0);
        // protocol version
        ser.writeVarint(758);
        // server address
        ser.write(serverName);
        // server port
        ser.write((short) 25565);
        // next state: status
        ser.writeVarint(1);
        writePacket(packet, socket);
    }
    packet.clear();
    {
        auto ser = packetSerializer{packet};
        // status request
        ser.writeVarint(0);
        writePacket(packet, socket);
    }
}

int asyncmain() {

    asio::io_context io;
    tcp::resolver resolver(io);
    tcp::resolver::results_type endpoints;
    try {
        endpoints = resolver.resolve(serverName, "25565");
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::exit(1);
    }

    for (auto &endpoint : endpoints) {
        std::cout << endpoint.host_name() << std::endl;
        std::cout << endpoint.endpoint() << std::endl;
    }

    tcp::socket socket{io};
    asio::connect(socket, endpoints);

    writePing(socket);

    std::vector<uint8_t> data(1 << 16);
    for (;;) {
        asio::error_code error;
        size_t len = socket.read_some(asio::buffer(data), error);
        if (error == asio::error::eof) {
            printf("eof.");
            break;
        } else if (error) {
            throw asio::system_error(error);
        }

        data.resize(len);

        fmt::print(">>\n");
        dump(data);

        auto deser = packetDeserializer{data};
        auto packetLength = deser.readVarint();
        auto packetId = deser.readVarint();
        if (packetId == 0) {
            auto string = deser.read<std::string>();
            fmt::print("Info string: {}", string);
        }

        socket.close();
        break;
    }

    return 0;
}

struct Connection {
    tcp::socket socket;
    bool encryptionEnabled;
    bool compressionEnabled;
    int compressionThreshhold;
    asio::error_code error;

    std::vector<uint8_t> readPacket() {
        std::vector<uint8_t> rawData{};
        std::vector<uint8_t> packetData{};
        int originalLen;
        int compressedLen;
        socket.read_some(asio::buffer(rawData), error);
        auto iterator = rawData.cbegin();
        originalLen = readVarint(iterator);
        return packetData;
    }
};