#ifndef _ENCODING_ERROR_H_
#define _ENCODING_ERROR_H_

#include <exception>

class encoding_error : public std::exception {
public:
	encoding_error(const char *message);
	virtual ~encoding_error();

	virtual const char *what() noexcept;

private:
	char *message;
};

#endif
