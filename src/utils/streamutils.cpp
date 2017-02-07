#include <iconv.h>
#include <string>
#include <sstream>
#include "utils/streamutils.h"
#include "utils/encoding_error.h"

using namespace std;

namespace stree {

namespace utils {

/*
 * Integer writing/reading utilities
 */

void writeByte(ostream *stream, uint8_t data) {
	char buf[] = { (char) data };
	stream->write(buf, 1);
}
uint8_t readByte(istream *stream) {
	char buf[1];
	stream->read(buf, 1);
	return buf[0];
}

void writeShort(ostream *stream, uint16_t data) {
	char buf[] = { (char) ((data >> 0x08) & 0xFF), (char) (data & 0xFF) };
	stream->write(buf, 2);
}
uint16_t readShort(istream *stream) {
	char buf[2];
	stream->read(buf, 2);
	return ((buf[0] & 0xFF) << 0x08) | (buf[1] & 0xFF);
}

void writeInt(ostream *stream, uint32_t data) {
	char buf[] = { (char) ((data >> 0x18) & 0xFF),
			(char) ((data >> 0x10) & 0xFF), (char) ((data >> 0x08) & 0xFF),
			(char) (data & 0xFF) };
	stream->write(buf, 4);
}
uint32_t readInt(istream *stream) {
	char buf[4];
	stream->read(buf, 4);
	return ((buf[0] & 0xFF) << 0x18) | ((buf[1] & 0xFF) << 0x10)
			| ((buf[2] & 0xFF) << 0x08) | (buf[3] & 0xFF);
}

void writeLong(ostream *stream, uint64_t data) {
	char buf[] = { (char) ((data >> 0x38) & 0xFF),
			(char) ((data >> 0x30) & 0xFF), (char) ((data >> 0x28) & 0xFF),
			(char) ((data >> 0x20) & 0xFF), (char) ((data >> 0x18) & 0xFF),
			(char) ((data >> 0x10) & 0xFF), (char) ((data >> 0x08) & 0xFF),
			(char) (data & 0xFF) };
	stream->write(buf, 8);
}
uint64_t readLong(istream *stream) {
	char buf[8];
	stream->read(buf, 8);
	return ((uint64_t) (buf[0] & 0xFF) << 0x38)
			| (((uint64_t) (buf[1] & 0xFF)) << 0x30)
			| (((uint64_t) (buf[2] & 0xFF)) << 0x28)
			| (((uint64_t) (buf[3] & 0xFF)) << 0x20)
			| (((uint64_t) (buf[4] & 0xFF)) << 0x18)
			| (((uint64_t) (buf[5] & 0xFF)) << 0x10)
			| (((uint64_t) (buf[6] & 0xFF)) << 0x08)
			| ((uint64_t) (buf[7] & 0xFF));
}

/*
 * Floating point writing/reading utilities
 */

void writeFloat(ostream *stream, float data) {
	int_float c;
	c.f = data;
	writeInt(stream, c.i);
}
float readFloat(istream *stream) {
	int_float c;
	c.i = readInt(stream);
	return c.f;
}

void writeDouble(ostream *stream, double data) {
	long_double c;
	c.d = data;
	writeLong(stream, c.l);
}
double readDouble(istream *stream) {
	long_double c;
	c.l = readLong(stream);
	return c.d;
}

/*
 * String writing/reading utilities
 */

int writeUTF16ToJava(ostream *stream, u16string data) {
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

u16string readUTF16FromJava(istream *stream) {
	// translated from java.io.DataInputStream

	int utflen = readShort(stream);

	// there can't be more utf16 chars than stream bytes
	char16_t *data = new char16_t[utflen];
	int dataIndex = 0;

	uint8_t c, c2, c3;

	int i;
	for (i = 0; i < utflen; i++) {
		c = readByte(stream);
		if ((c & 0x80) != 0)
			break;
		data[dataIndex++] = (char16_t) (c & 0x7F);
	}

	while (i < utflen) {
		c = readByte(stream);
		if ((c & 0x80) == 0) {
			// single byte
			data[dataIndex++] = (char16_t) (c & 0x7F);
			i++;
		} else if ((c & 0xE0) == 0xC0) {
			// double byte
			i += 2;
			if (i > utflen) {
				throw new encoding_error(
						"string terminates at partial character");
			}
			c2 = readByte(stream);
			if ((c2 & 0xC0) != 0x80) {
				throw new encoding_error("invalid utf-8 byte chain");
			}
			data[dataIndex++] = (char16_t) (((c & 0x1F) << 0x6) | (c2 & 0x3F));
		} else if ((c & 0xF0) == 0xE0) {
			// triple byte
			i += 3;
			if (i > utflen) {
				throw new encoding_error(
						"string terminates at partial character");
			}
			c2 = readByte(stream);
			c3 = readByte(stream);
			if ((c2 & 0xC0) != 0x80 || (c3 & 0xC0) != 0x80) {
				throw new encoding_error("invalid utf-8 byte chain");
			}
			data[dataIndex++] = (char16_t) (((c & 0x0F) << 0xC)
					| ((c2 & 0x3F) << 0x6) | (c3 & 0x3F));
		} else {
			throw new encoding_error("not a java-utf-8 string");
		}
	}

	u16string str(data, dataIndex);

	delete data;

	return str;
}

}

}
