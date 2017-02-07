#include <iconv.h>
#include <string>
#include "utils/streamutils.h"
#include "utils/encoding_error.h"

namespace stree {

namespace utils {

/*
 * Integer writing/reading utilities
 */

void writeByte(std::ostream *stream, std::uint8_t data) {
	char buf[] = { (char) data };
	stream->write(buf, 1);
}
std::uint8_t readByte(std::istream *stream) {
	char buf[1];
	stream->read(buf, 1);
	return buf[0];
}

void writeShort(std::ostream *stream, std::uint16_t data) {
	char buf[] = { (char) ((data >> 0x08) & 0xFF), (char) (data & 0xFF) };
	stream->write(buf, 2);
}
std::uint16_t readShort(std::istream *stream) {
	char buf[2];
	stream->read(buf, 2);
	return ((buf[0] & 0xFF) << 0x08) | (buf[1] & 0xFF);
}

void writeInt(std::ostream *stream, std::uint32_t data) {
	char buf[] = { (char) ((data >> 0x18) & 0xFF),
			(char) ((data >> 0x10) & 0xFF), (char) ((data >> 0x08) & 0xFF),
			(char) (data & 0xFF) };
	stream->write(buf, 4);
}
std::uint32_t readInt(std::istream *stream) {
	char buf[4];
	stream->read(buf, 4);
	return ((buf[0] & 0xFF) << 0x18) | ((buf[1] & 0xFF) << 0x10)
			| ((buf[2] & 0xFF) << 0x08) | (buf[3] & 0xFF);
}

void writeLong(std::ostream *stream, std::uint64_t data) {
	char buf[] = { (char) ((data >> 0x38) & 0xFF),
			(char) ((data >> 0x30) & 0xFF), (char) ((data >> 0x28) & 0xFF),
			(char) ((data >> 0x20) & 0xFF), (char) ((data >> 0x18) & 0xFF),
			(char) ((data >> 0x10) & 0xFF), (char) ((data >> 0x08) & 0xFF),
			(char) (data & 0xFF) };
	stream->write(buf, 8);
}
std::uint64_t readLong(std::istream *stream) {
	char buf[8];
	stream->read(buf, 8);
	return ((std::uint64_t) (buf[0] & 0xFF) << 0x38)
			| (((std::uint64_t) (buf[1] & 0xFF)) << 0x30)
			| (((std::uint64_t) (buf[2] & 0xFF)) << 0x28)
			| (((std::uint64_t) (buf[3] & 0xFF)) << 0x20)
			| (((std::uint64_t) (buf[4] & 0xFF)) << 0x18)
			| (((std::uint64_t) (buf[5] & 0xFF)) << 0x10)
			| (((std::uint64_t) (buf[6] & 0xFF)) << 0x08)
			| ((std::uint64_t) (buf[7] & 0xFF));
}

/*
 * Floating point writing/reading utilities
 */

void writeFloat(std::ostream *stream, float data) {
	int_float c;
	c.f = data;
	writeInt(stream, c.i);
}
float readFloat(std::istream *stream) {
	int_float c;
	c.i = readInt(stream);
	return c.f;
}

void writeDouble(std::ostream *stream, double data) {
	long_double c;
	c.d = data;
	writeLong(stream, c.l);
}
double readDouble(std::istream *stream) {
	long_double c;
	c.l = readLong(stream);
	return c.d;
}

/*
 * String writing/reading utilities
 */

int writeUTF16ToJava(std::ostream *stream, std::u16string data) {
	// Translated from java.io.DataOutputStream.writeUTF(String, DataOutput)

	int datalen = data.length();

	int utflen = 0;
	char16_t c;
	for (int i = 0; i < datalen; i++) {
		c = data[i];
		if (c >= 0x0001 && c <= 0x007F) {
			utflen++;
		} else if (c <= 0x07FF) {
			utflen += 2;
		} else {
			utflen += 3;
		}
	}

	if (utflen > 65535) {
		throw new encoding_error("encoded string too long");
	}

	writeShort(stream, utflen);

	int i;
	for (i = 0; i < utflen; i++) {
		c = data[i];
		if (!(c >= 0x0001 && c <= 0x007F))
			break;
		writeByte(stream, (char) c);
	}

	for (; i < utflen; i++) {
		c = data[i];
		if (c >= 0x0001 && c <= 0x007F) {
			writeByte(stream, (char) c);
		} else if (c <= 0x07FF) {
			writeByte(stream, (char) (0xC0 | ((c >> 0x6) & 0x1F)));
			writeByte(stream, (char) (0x80 | ((c >> 0x0) & 0x3F)));
		} else {
			writeByte(stream, (char) (0xE0 | ((c >> 0xC) & 0x0F)));
			writeByte(stream, (char) (0x80 | ((c >> 0x6) & 0x3F)));
			writeByte(stream, (char) (0x80 | ((c >> 0x0) & 0x3F)));
		}
	}

	return utflen + 2;
}

std::u16string readUTF16FromJava(std::istream *stream) {
	int utflen = readShort(stream);

	std::basic_stringstream<char16_t> str;

	int i = 0;
	while (i < utflen) {
		uint8_t c = readByte(stream);
		if ((c & 0x80) == 0) {
			// single byte
			str << (char16_t) (c & 0x7F);
			i++;
		} else if ((c & 0xE0) == 0xC0) {
			// TODO two byte part
		} else if ((c & 0xF0) == 0xE0) {
			// TODO three byte part
		} else {
			throw new encoding_error("not a java utf-8 string");
		}
	}
}

}

}
