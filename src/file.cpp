#include <nan.h>

#include <windows.h>

using namespace v8;
using namespace node;

NAN_METHOD(GetFileStats) {
  NanScope();

  Local<Object> returnObj = NanNew<Object>();

  String::Utf8Value filePathV8(args[0]->ToString());
  const char* filePath = std::string(*filePathV8).c_str();

  int fileAttributes = GetFileAttributes(filePath);
  int lastError = GetLastError();

  returnObj->Set(NanNew<String>("errorCode"), NanNew<Number>(lastError));

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

    returnObj->Set(NanNew<String>("isNormal"), NanNew<Boolean>(isNormal));
    returnObj->Set(NanNew<String>("isArchive"), NanNew<Boolean>(isArchive));
    returnObj->Set(NanNew<String>("isHidden"), NanNew<Boolean>(isHidden));
    returnObj->Set(NanNew<String>("isSystem"), NanNew<Boolean>(isSystem));
    returnObj->Set(NanNew<String>("isReadOnly"), NanNew<Boolean>(isReadOnly));
    returnObj->Set(NanNew<String>("isDirectory"), NanNew<Boolean>(isDirectory));
  }

  NanReturnValue(returnObj);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getFileStats"),
    NanNew<FunctionTemplate>(GetFileStats)->GetFunction());
}

NODE_MODULE(file, init)
