#include <nan.h>
#include "Parser.hpp"
#include "Tree.hpp"

NAN_MODULE_INIT(InitAll) {
	HTML5::Parser::Init(target);
	HTML5::Tree::Init(target);
}

NODE_MODULE(addon, InitAll)
