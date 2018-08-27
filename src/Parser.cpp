#include "Utils.hpp"
#include "Parser.hpp"

using namespace HTML5;

/*
 * Async parser worker
 * */
void ParserWorker::Execute() {
	uv_mutex_lock(&parser->myhtml_mutex);
	
	// parser->myhtml;
	
	
	
	uv_mutex_unlock(&parser->myhtml_mutex);
}

void ParserWorker::HandleOKCallback() {
	Nan::HandleScope scope;
	
	v8::Local<v8::Value> argv[] = {Nan::Null(), Nan::Null()};
	callback->Call(2, argv, async_resource);
}

void ParserWorker::HandleErrorCallback() {
	Nan::HandleScope scope;
	
	v8::Local<v8::Value> argv[] = {Nan::New(this->ErrorMessage()).ToLocalChecked(), Nan::Null()};
	callback->Call(2, argv, async_resource);
}

/*
 * Parser class
 * */

Nan::Persistent<v8::Function> Parser::constructor;

Parser::Parser(v8::MaybeLocal<v8::Object> src_options) {
	fprintf(stderr, "Init Parser\n");
	
	mystatus_t status;
	uv_mutex_init(&myhtml_mutex);
	
	if (!src_options.IsEmpty()) {
		GetOptionsFromJS(&options, &options, src_options.ToLocalChecked());
		
		if (!CheckOptions(&options, "new Parser([options])"))
			return;
	}
	
	myhtml = myhtml_create();
	if (options.threads <= 0) {
		status = myhtml_init(myhtml, MyHTML_OPTIONS_PARSE_MODE_SINGLE, 1, 0);
	} else if (options.threads == 1) {
		status = myhtml_init(myhtml, MyHTML_OPTIONS_PARSE_MODE_ALL_IN_ONE, 1, 0);
	} else {
		status = myhtml_init(myhtml, MyHTML_OPTIONS_PARSE_MODE_SEPARATELY, options.threads, 0);
	}
	
	if (status) {
		Utils::ThrowError("myhtml_init failed: %d (%s)", status, Utils::GetModestError(status));
		return;
	}
}

Parser::~Parser() {
	fprintf(stderr, "Destroy Parser\n");
	
	if (myhtml)
		myhtml_destroy(myhtml);
	uv_mutex_destroy(&myhtml_mutex);
}

// Init Parser class
void Parser::Init(v8::Local<v8::Object> exports) {
	Nan::HandleScope scope;
	
	// Constructor
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	// Prototype
	Nan::SetPrototypeMethod(tpl, "parseCallback", Parse);
	Nan::SetPrototypeMethod(tpl, "parseSync", ParseSync);
	
	Nan::SetPrototypeMethod(tpl, "ParseChunkStart", ParseChunkStart);
	
	Nan::SetPrototypeMethod(tpl, "ParseChunkCallback", ParseChunk);
	Nan::SetPrototypeMethod(tpl, "ParseChunkSync", ParseChunkSync);
	
	Nan::SetPrototypeMethod(tpl, "ParseChunkEndCallback", ParseChunkEnd);
	Nan::SetPrototypeMethod(tpl, "ParseChunkEndSync", ParseChunkEndSync);
	
	// Add to exports
	constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("Parser").ToLocalChecked(), tpl->GetFunction());
}

// New instance of Parser
NAN_METHOD(Parser::New) {
	if (info.IsConstructCall()) {
		if (info.Length() > 1) {
			Utils::ThrowTypeError("Parser([options]): Invalid arguments count.");
			return;
		}
		
		if (info.Length() > 0 && !info[0]->IsObject()) {
			Utils::ThrowTypeError("Parser([options]): options must be a object.");
			return;
		}
		
		Parser *parser = new Parser(Nan::To<v8::Object>(info[0]));
		parser->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	} else {
		Utils::ThrowTypeError("Constructor requires 'new'");
	}
}

// Parse full HTML (async)
NAN_METHOD(Parser::Parse) {
	Parser *self = ObjectWrap::Unwrap<Parser>(info.Holder());
	
	Nan::Callback *callback = new Nan::Callback(Nan::To<v8::Function>(info[1]).ToLocalChecked());
	const v8::Local<v8::Value> &value = info[0];
	
	ParserWorker *parser = new ParserWorker(callback, self);
	parser->SaveToPersistent(0u, value);
	AsyncQueueWorker(parser);
}

// Parse full HTML (sync)
NAN_METHOD(Parser::ParseSync) {
	
}

// Start chunked parsing (only sync)
NAN_METHOD(Parser::ParseChunkStart) {
	
}

// Parse one chunk of html (async)
NAN_METHOD(Parser::ParseChunk) {
	
}

// Parse one chunk of html (sync)
NAN_METHOD(Parser::ParseChunkSync) {
	
}

// Stop chunked parsing and return tree (async)
NAN_METHOD(Parser::ParseChunkEnd) {
	
}

// Stop chunked parsing and return tree (sync)
NAN_METHOD(Parser::ParseChunkEndSync) {
	
}

/*
 * Parser options from JS
 * */
bool Parser::GetBoolOption(v8::Local<v8::Object> object, const char *property, bool default_value) {
	auto maybe_value = Nan::Get(object, Nan::New(property).ToLocalChecked());
	if (!maybe_value.IsEmpty() && !maybe_value.ToLocalChecked()->IsUndefined())
		return Nan::To<bool>(maybe_value.ToLocalChecked()).FromMaybe(default_value);
	return default_value;
}

long Parser::GetLongOption(v8::Local<v8::Object> object, const char *property, long default_value) {
	v8::MaybeLocal<v8::Value> maybe_value = Nan::Get(object, Nan::New(property).ToLocalChecked());
	if (!maybe_value.IsEmpty() && !maybe_value.ToLocalChecked()->IsUndefined())
		return Nan::To<int32_t>(maybe_value.ToLocalChecked()).FromMaybe(default_value);
	return default_value;
}

myencoding_t Parser::GetEncodingOption(v8::Local<v8::Object> object, const char *property, myencoding_t default_value) {
	v8::MaybeLocal<v8::Value> maybe_value = Nan::Get(object, Nan::New(property).ToLocalChecked());
	if (!maybe_value.IsEmpty()) {
		v8::Local<v8::Value> value = maybe_value.ToLocalChecked();
		
		// default
		if (value->IsUndefined()) {
			return default_value;
		}
		// encoding id
		else if (value->IsNumber() || value->IsBoolean() || value->IsNumber() || value->IsInt32() || value->IsUint32()) {
			myencoding_t enc_id = (myencoding_t) Nan::To<int32_t>(maybe_value.ToLocalChecked()).FromMaybe(MyENCODING_NOT_DETERMINED);
			
			if (enc_id == MyENCODING_AUTO || enc_id == MyENCODING_DEFAULT || enc_id == MyENCODING_NOT_DETERMINED)
				return enc_id;
			
			if (!myencoding_name_by_id(enc_id, NULL))
				return MyENCODING_NOT_DETERMINED;
			
			return enc_id;
		}
		// encoding name or id
		else {
			Nan::Utf8String str_value(maybe_value.ToLocalChecked());
			if (str_value.length()) {
				const char *char_value = *str_value;
				myencoding_t enc_id;
				if (isdigit(char_value[0])) { // may be encoding id
					enc_id = (myencoding_t) Nan::To<int32_t>(maybe_value.ToLocalChecked()).FromMaybe(MyENCODING_NOT_DETERMINED);
					
					if (enc_id == MyENCODING_AUTO || enc_id == MyENCODING_DEFAULT || enc_id == MyENCODING_NOT_DETERMINED)
						return enc_id;
					
					if (!myencoding_name_by_id(enc_id, NULL))
						return MyENCODING_NOT_DETERMINED;
					
					return enc_id;
				} else { // encoding name
					if (myencoding_by_name(char_value, str_value.length(), &enc_id))
						return enc_id;
					
					if (str_value.length() == 4 && strcasecmp(char_value, "auto") == 0)
						return MyENCODING_AUTO;
					
					if (str_value.length() == 7 && strcasecmp(char_value, "default") == 0)
						return MyENCODING_DEFAULT;
				}
			}
			return MyENCODING_NOT_DETERMINED;
		}
	}
	return default_value;
}

bool Parser::CheckOptions(ParserOptions *options, const char *func) {
	if (options->encoding == MyENCODING_NOT_DETERMINED) {
		Utils::ThrowTypeError("%s: Invalid encoding value");
		return false;
	}
	
	if (options->default_encoding == MyENCODING_NOT_DETERMINED || options->default_encoding == MyENCODING_AUTO) {
		Utils::ThrowTypeError("%s: Invalid defaultEncoding value");
		return false;
	}
	
	if (options->threads < 0) {
		Utils::ThrowTypeError("%s: Invalid threads count");
		return false;
	}
	
	if (options->encoding_prescan_limit < 0) {
		Utils::ThrowTypeError("%s: Invalid encodingPrescanLimit value");
		return false;
	}
	
	return true;
}

void Parser::GetOptionsFromJS(ParserOptions *dst_options, const ParserOptions *default_options, v8::Local<v8::Object> object) {
	dst_options->threads				= GetLongOption(object, "threads", default_options->threads);
	dst_options->async					= GetBoolOption(object, "async", default_options->async);
	dst_options->ignore_whitespace		= GetBoolOption(object, "ignoreWhitespace", default_options->ignore_whitespace);
	dst_options->ignore_doctype			= GetBoolOption(object, "ignoreDoctype", default_options->ignore_doctype);
	dst_options->scripts				= GetBoolOption(object, "scripts", default_options->scripts);
	dst_options->encoding				= GetEncodingOption(object, "encoding", default_options->encoding);
	dst_options->default_encoding		= GetEncodingOption(object, "defaultEncoding", default_options->default_encoding);
	dst_options->encoding_use_meta		= GetBoolOption(object, "encodingUseMeta", default_options->encoding_use_meta);
	dst_options->encoding_use_bom		= GetBoolOption(object, "encodingUseBom", default_options->encoding_use_bom);
	dst_options->encoding_prescan_limit	= GetLongOption(object, "encodingPrescanLimit", default_options->encoding_prescan_limit);
	
	#ifdef MyCORE_BUILD_WITHOUT_THREADS
		dst_options->threads = 0;
	#endif
}
