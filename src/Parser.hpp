#pragma once

#include <nan.h>
#include <myhtml/myhtml.h>

#define MyENCODING_AUTO ((myencoding_t) 1)

namespace HTML5 {
	typedef struct ParserOptions {
		long threads					= 2;
		bool async						= false;
		bool ignore_whitespace			= false;
		bool ignore_doctype				= false;
		bool scripts					= false;
		myencoding_t encoding			= MyENCODING_AUTO;
		myencoding_t default_encoding	= MyENCODING_DEFAULT;
		bool encoding_use_meta			= true;
		bool encoding_use_bom			= true;
		long encoding_prescan_limit		= 1024;
	} _ParserOptions;
	
	class Parser: public Nan::ObjectWrap {
		public:
			Parser(v8::MaybeLocal<v8::Object> options);
			
			ParserOptions options;
			myhtml_t *myhtml = nullptr;
			uv_mutex_t myhtml_mutex;
			
			static void Init(v8::Local<v8::Object> exports);
		private:
			~Parser();
			
			static Nan::Persistent<v8::Function> constructor;
			
			static NAN_METHOD(JsNew);
			static NAN_METHOD(JsParse);
			static NAN_METHOD(JsParseSync);
			static NAN_METHOD(JsParseChunkStart);
			static NAN_METHOD(JsParseChunk);
			static NAN_METHOD(JsParseChunkSync);
			static NAN_METHOD(JsParseChunkEnd);
			static NAN_METHOD(JsParseChunkEndSync);
			
			myhtml_tree_t *Parse(const Nan::Utf8String &html, ParserOptions *options);
			std::tuple<myencoding_t, const char *, size_t> DetectEncoding(const Nan::Utf8String &html, ParserOptions *options);
			void ApplyTreeOptions(myhtml_tree_t *tree, ParserOptions *options);
			
			void TreeWaitForDone(myhtml_tree_t *tree);
			bool TreeIsDone(myhtml_tree_t *tree);
			
			void NodeWaitForDone(myhtml_tree_node_t *tree, bool recursive);
			bool NodeIsDone(myhtml_tree_node_t *tree, bool recursive);
			
			// Options from JS
			static bool GetBoolOption(v8::Local<v8::Object> object, const char *property, bool default_value);
			static long GetLongOption(v8::Local<v8::Object> object, const char *property, long default_value);
			static myencoding_t GetEncodingOption(v8::Local<v8::Object> object, const char *property, myencoding_t default_value);
			static void GetOptionsFromJS(ParserOptions *dst_options, const ParserOptions *default_options, v8::Local<v8::Object> object);
			static bool CheckOptions(ParserOptions *options, const char *func);
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
