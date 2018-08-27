#include "Utils.hpp"

#include <cstdarg>

using namespace HTML5;

void Utils::ThrowError(const char *msg, ...) {
	char buffer[4096];
	
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg, args);
	va_end(args);
	
	Nan::ThrowError(buffer);
}

void Utils::ThrowTypeError(const char *msg, ...) {
	char buffer[4096];
	
	va_list args;
	va_start(args, msg);
	vsnprintf(buffer, sizeof(buffer), msg, args);
	va_end(args);
	
	Nan::ThrowTypeError(buffer);
}

const char *Utils::GetModestError(mystatus_t status) {
	switch (status) {
		#include "gen/modest_errors.c"
	}
	
	return "UNKNOWN";
}
