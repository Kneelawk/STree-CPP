#include <cstring>
#include "utils/encoding_error.h"

encoding_error::encoding_error(const char *message) {
	this->message = new char[std::strlen(message)];
	std::strcpy(this->message, message);
}

encoding_error::~encoding_error() {
	delete message;
}

const char *encoding_error::what() noexcept {
	return message;
}
