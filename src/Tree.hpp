#pragma once

#include <nan.h>
#include <myhtml/myhtml.h>

#include "Parser.hpp"

namespace HTML5 {
	class Tree: public Nan::ObjectWrap {
		public:
			Tree(myhtml_tree_t *tree);
			
			myhtml_tree_t *myhtml_tree = nullptr;
			
			static void Init(v8::Local<v8::Object> exports);
			static v8::Local<v8::Object> FromMyhtml(myhtml_tree_t *tree, Parser *parser);
		private:
			~Tree();
			
			static Nan::Persistent<v8::Function> constructor;
			
			static NAN_METHOD(JsNew);
	};
};
