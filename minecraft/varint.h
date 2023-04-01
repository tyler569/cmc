#ifndef GL_MINECRAFT_VARINT_H
#define GL_MINECRAFT_VARINT_H

#include <cstdlib>
#include <iterator>

#define SEGMENT_BITS 0x7F
#define CONTINUE_BIT 0x80

void writeVarint(int value, std::vector<uint8_t> &v);
int readVarint(std::vector<uint8_t>::const_iterator &it);

void writeVarlong(long value, std::vector<uint8_t> &v);
long readVarlong(std::vector<uint8_t>::const_iterator &it);

class InvalidVarint : std::exception {};


void testVarint();

#endif