#pragma once

#include <nan.h>

#include <myhtml/myhtml.h>
#include <mycss/mycss.h>
#include <modest/finder/finder.h>

namespace HTML5 {
	class Utils {
		public:
			static void ThrowError(const char *msg, ...);
			static void ThrowTypeError(const char *msg, ...);
			static const char *GetModestError(mystatus_t status);
	};
};
