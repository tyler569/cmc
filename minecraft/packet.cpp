#include "minecraft/packet.h"
#include "util/dump.h"

void testPacket() {
    std::vector<uint8_t> data{};
    auto ser = packetSerializer{data};
    ser.writeVarint(-2356);
    ser.write(3.4f);
    ser.write(0x12345678);
    ser.write(3.4);
    ser.write(std::string_view("Hello World"));

    dump(data);

    auto deser = packetDeserializer{data};
    auto a = deser.readVarint();
    auto b = deser.read<float>();
    auto x = deser.read<int>();
    auto c = deser.read<double>();
    auto d = deser.read<std::string>();
    fmt::print("{} {} {} {} {}\n", a, b, x, c, d);
}
