#include "Parser.hpp"

#include <myhtml/myhtml.h>

/*
 * Async parser worker
 * */
void HTML5::ParserWorker::Execute() {
	
}

void HTML5::ParserWorker::HandleOKCallback() {
	Nan::HandleScope scope;
	
	v8::Local<v8::Value> argv[] = {Nan::Null(), Nan::Null()};
	callback->Call(2, argv, async_resource);
}

void HTML5::ParserWorker::HandleErrorCallback() {
	Nan::HandleScope scope;
	
	v8::Local<v8::Value> argv[] = {Nan::New(this->ErrorMessage()).ToLocalChecked(), Nan::Null()};
	callback->Call(2, argv, async_resource);
}

/*
 * Parser class
 * */

Nan::Persistent<v8::Function> HTML5::Parser::constructor;

void HTML5::Parser::Init(v8::Local<v8::Object> exports) {
	Nan::HandleScope scope;
	
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
	tpl->SetClassName(Nan::New("Parser").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	// Prototype
	Nan::SetPrototypeMethod(tpl, "parse", Parse);
	
	constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("Parser").ToLocalChecked(), tpl->GetFunction());
}

void HTML5::Parser::New(const Nan::FunctionCallbackInfo<v8::Value> &info) {
	if (info.IsConstructCall()) {
		HTML5::Parser* obj = new HTML5::Parser();
		obj->Wrap(info.This());
		info.GetReturnValue().Set(info.This());
	} else {
		v8::Local<v8::Value> argv[0];
		v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
		info.GetReturnValue().Set(Nan::NewInstance(cons, 0, argv).ToLocalChecked());
	}
}

void HTML5::Parser::Parse(const Nan::FunctionCallbackInfo<v8::Value> &info) {
	HTML5::Parser *self = ObjectWrap::Unwrap<HTML5::Parser>(info.Holder());
	
	Nan::Callback *callback = new Nan::Callback(Nan::To<v8::Function>(info[1]).ToLocalChecked());
	const v8::Local<v8::Value> &value = info[0];
	
	HTML5::ParserWorker *parser = new HTML5::ParserWorker(callback, self);
	parser->SaveToPersistent(0u, value);
	AsyncQueueWorker(parser);
}
