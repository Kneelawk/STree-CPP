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

void writeByte(std::ostream *stream, std::uint8_t data);
std::uint8_t readByte(std::istream *stream);

void writeShort(std::ostream *stream, std::uint16_t data);
std::uint16_t readShort(std::istream *stream);

void writeInt(std::ostream *stream, std::uint32_t data);
std::uint32_t readInt(std::istream *stream);

void writeLong(std::ostream *stream, std::uint64_t data);
std::uint64_t readLong(std::istream *stream);

void writeFloat(std::ostream *stream, float data);
float readFloat(std::istream *stream);

void writeDouble(std::ostream *stream, double data);
double readDouble(std::istream *stream);

int writeUTF32ToJava(std::ostream *stream, std::u32string data);
int writeUTF16ToJava(std::ostream *stream, std::u16string data);
int writeUTF8ToJava(std::ostream *stream, std::string data);
int writeAsciiToJava(std::ostream *stream, std::string data);
std::u32string readJavaToUTF32(std::istream *stream);
std::u16string readJavaToUTF16(std::istream *stream);
std::string readJavaToUTF8(std::istream *stream);
std::string readJavaToAscii(std::istream *stream);

}

}

#endif
