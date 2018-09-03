#include "Utils.hpp"
#include "Tree.hpp"

// sooqa, why tak slozhno?

using namespace HTML5;

Nan::Persistent<v8::Function> Tree::constructor;

Tree::Tree(myhtml_tree_t *tree) : myhtml_tree(tree) {
	fprintf(stderr, "Init Tree\n");
}

Tree::~Tree() {
	fprintf(stderr, "Destroy Tree\n");
}

// Create Tree from modest `myhtml_tree_t`
v8::Local<v8::Object> Tree::FromMyhtml(myhtml_tree_t *myhtml_tree, Parser *parser) {
	Nan::EscapableHandleScope scope;
	
	// Reuse exists instance
	if (myhtml_tree->context)
		return scope.Escape(static_cast<Tree *>(myhtml_tree->context)->handle());
	
	fprintf(stderr, "tree=%p, parser=%p\n", myhtml_tree, parser);
	
	// Create new instance
	v8::Local<v8::Value> argv[2] = {Nan::New<v8::External>(myhtml_tree), Nan::New<v8::External>(parser)};
    v8::Local<v8::Object> obj = Nan::NewInstance(Nan::New<v8::Function>(constructor), 2, argv).ToLocalChecked();
    
    return scope.Escape(obj);
}

// Init Tree class
void Tree::Init(v8::Local<v8::Object> exports) {
	Nan::HandleScope scope;
	
	// Constructor
	v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(JsNew);
	tpl->SetClassName(Nan::New("Tree").ToLocalChecked());
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	// Add to exports
	constructor.Reset(tpl->GetFunction());
	exports->Set(Nan::New("Tree").ToLocalChecked(), tpl->GetFunction());
}

// New instance of Tree
NAN_METHOD(Tree::JsNew) {
	if (info.Length() == 2 && info[0]->IsExternal() && info[1]->IsExternal()) {
		myhtml_tree_t *tree = static_cast<myhtml_tree_t *>(info[0].As<v8::External>()->Value());
		Parser *parser = static_cast<Parser *>(info[1].As<v8::External>()->Value());
		
		fprintf(stderr, "call JsNew tree=%p, parser=%p\n", tree, parser);
	} else {
		Utils::ThrowTypeError("No direct call. Use Parser.createTree().");
	}
}
