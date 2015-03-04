#pragma comment(lib, "psapi.lib")

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

#include <windows.h>

#include <string>
#include <stdio.h>

using namespace v8;
using namespace node;


class File : ObjectWrap {
  private:
  public:
    File() {}
    ~File() {}
    static void Init(Handle<Object> exports) {
        NODE_SET_METHOD(exports, "getFileStats", GetFileStats);
    }

    static void GetFileStats(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);

        Local<Object> returnObj = Object::New(isolate);

        String::Utf8Value filePathV8(args[0]->ToString());
        const char* filePath = std::string(*filePathV8).c_str();

        int fileAttributes = GetFileAttributes(filePath);
        if (fileAttributes == INVALID_FILE_ATTRIBUTES && GetLastError()== ERROR_FILE_NOT_FOUND) {
          returnObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "file not found"));
        } else if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
          returnObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "unknown error"));
        }

        bool isNormal = false, isHidden = false, isReadOnly = false, isSystem = false, isDirectory = false;
        if ((fileAttributes & FILE_ATTRIBUTE_NORMAL) != 0){
          isNormal = true;
        }

        if ((fileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0){
          isHidden = true;
        }

        if ((fileAttributes & FILE_ATTRIBUTE_READONLY) != 0){
          isReadOnly = true;
        }

        if ((fileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0){
          isSystem = true;
        }

        if ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0){
          isDirectory = true;
        }

        returnObj->Set(String::NewFromUtf8(isolate, "isNormal"), Boolean::New(isolate, isNormal));
        returnObj->Set(String::NewFromUtf8(isolate, "isHidden"), Boolean::New(isolate, isHidden));
        returnObj->Set(String::NewFromUtf8(isolate, "isSystem"), Boolean::New(isolate, isSystem));
        returnObj->Set(String::NewFromUtf8(isolate, "isReadOnly"), Boolean::New(isolate, isReadOnly));
        returnObj->Set(String::NewFromUtf8(isolate, "isDirectory"), Boolean::New(isolate, isDirectory));

        args.GetReturnValue().Set(returnObj);
    }
};

extern "C" {
	void init(Handle<Object> exports) {
        File::Init(exports);
	}
	NODE_MODULE(file, init)
}