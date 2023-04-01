#ifndef GL_HANDSHAKE_H
#define GL_HANDSHAKE_H

#include <cstdint>
#include <string>

struct Handshake {
    enum NextState {
        STATUS = 1,
        LOGIN = 2,
    };

    int protocolVersion;
    std::string serverAddress;
    uint16_t serverPort;
    NextState nextState;
};

#endif //GL_HANDSHAKE_H
