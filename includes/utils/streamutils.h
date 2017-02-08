#ifndef _STREAMUTILS_H_
#define _STREAMUTILS_H_

#include <cstdint>
#include <iostream>

namespace stree {

namespace utils {

union int_float {
	int i;
	float f;
};

union long_double {
	long int l;
	double d;
};

void writeByte(std::ostream &stream, std::uint8_t data);
std::uint8_t readByte(std::istream &stream);

void writeShort(std::ostream &stream, std::uint16_t data);
std::uint16_t readShort(std::istream &stream);

void writeInt(std::ostream &stream, std::uint32_t data);
std::uint32_t readInt(std::istream &stream);

void writeLong(std::ostream &stream, std::uint64_t data);
std::uint64_t readLong(std::istream &stream);

void writeFloat(std::ostream &stream, float data);
float readFloat(std::istream &stream);

void writeDouble(std::ostream &stream, double data);
double readDouble(std::istream &stream);

int writeUTF16ToJava(std::ostream &stream, std::u16string data);
std::u16string readUTF16FromJava(std::istream &stream);

}

}

#endif
