#pragma comment(lib, "psapi.lib")

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

#include <windows.h>

#include <string>
#include <stdio.h>

using namespace v8;
using namespace node;

void GetFileStats(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = Isolate::GetCurrent();
  HandleScope scope(isolate);

  Local<Object> returnObj = Object::New(isolate);

  String::Utf8Value filePathV8(args[0]->ToString());
  const char* filePath = std::string(*filePathV8).c_str();

  int fileAttributes = GetFileAttributes(filePath);
  int lastError = GetLastError();

  returnObj->Set(String::NewFromUtf8(isolate, "errorCode"), Number::New(isolate, lastError));

  if (lastError == 0) {
    bool isNormal = false, isHidden = false, isReadOnly = false, isSystem = false, isDirectory = false, isArchive = false;

    if ((fileAttributes & FILE_ATTRIBUTE_NORMAL) != 0){
      isNormal = true;
    }

    if ((fileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0){
      isArchive = true;
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
    returnObj->Set(String::NewFromUtf8(isolate, "isArchive"), Boolean::New(isolate, isArchive));
    returnObj->Set(String::NewFromUtf8(isolate, "isHidden"), Boolean::New(isolate, isHidden));
    returnObj->Set(String::NewFromUtf8(isolate, "isSystem"), Boolean::New(isolate, isSystem));
    returnObj->Set(String::NewFromUtf8(isolate, "isReadOnly"), Boolean::New(isolate, isReadOnly));
    returnObj->Set(String::NewFromUtf8(isolate, "isDirectory"), Boolean::New(isolate, isDirectory));
  }

  args.GetReturnValue().Set(returnObj);
}

void init(Handle<Object> exports) {
  NODE_SET_METHOD(exports, "getFileStats", GetFileStats);
}

NODE_MODULE(file, init)
