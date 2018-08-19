#pragma once

#include <nan.h>

namespace HTML5 {
	class Parser: public Nan::ObjectWrap {
		public:
			static void Init(v8::Local<v8::Object> exports);
		
		private:
			static Nan::Persistent<v8::Function> constructor;
			static void New(const Nan::FunctionCallbackInfo<v8::Value> &info);
			static void Parse(const Nan::FunctionCallbackInfo<v8::Value> &info);
	};
	
	class ParserWorker: public Nan::AsyncWorker {
		public:
			ParserWorker(Nan::Callback *callback, Parser *parser) :
				Nan::AsyncWorker(callback), parser(parser) { }
			void Execute();
			void HandleOKCallback();
			void HandleErrorCallback();
		private:
			Parser *parser;
	};
};
