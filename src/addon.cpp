#include <nan.h>
#include "Parser.hpp"

NAN_MODULE_INIT(InitAll) {
	HTML5::Parser::Init(target);
}

NODE_MODULE(addon, InitAll)
