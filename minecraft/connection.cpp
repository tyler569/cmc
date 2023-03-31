#include <iostream>
#include <asio.hpp>
#include <fmt/core.h>
#include "varint.h"

using asio::ip::tcp;

struct handshake {
};

void writePacket(std::vector<uint8_t> &data, tcp::socket &socket) {
    auto length = data.size();
    std::vector<uint8_t> packet = {};
    writeVarint(length, packet);
    packet.insert(packet.end(), data.begin(), data.end());

    asio::error_code error;
    asio::write(socket, asio::buffer(packet), error);
    if (error) {
        throw asio::system_error(error);
    }
}

void writePing(tcp::socket &socket) {
    std::vector<uint8_t> packet = {};
    // handshake
    writeVarint(0, packet);
    // protocol version 758, 1.18.2
    writeVarint(758, packet);
    // server address (empty)
    writeVarint(0, packet);
    // server port (0u16)
    packet.push_back(0);
    packet.push_back(0);
    // next state: status
    writeVarint(1, packet);

    writePacket(packet, socket);

    packet.clear();
    // status request
    writeVarint(0, packet);
    writePacket(packet, socket);
}

int asyncmain() {
    asio::io_context io;
    tcp::resolver resolver(io);
    tcp::resolver::results_type endpoints;
    try {
        endpoints = resolver.resolve("mc.openredstone.org", "25565");
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

        auto it = data.begin();
        auto packetLength = readVarint(it);
        fmt::print("packet length: {}\n", packetLength);
        auto packetId = readVarint(it);
        fmt::print("packet ID:     {}\n", packetId);
        auto stringLength = readVarint(it);
        fmt::print("string length: {}\n", stringLength);
        for (int i = 0; i < stringLength; i++) {
            printf("%c", *it++);
        }
        printf("\n");

        socket.close();
        break;
    }

    return 0;
}