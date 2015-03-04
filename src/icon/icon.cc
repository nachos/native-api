#pragma comment(lib, "psapi.lib")

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

#include <uv.h>

#include <windows.h>

#include <stdio.h>
#include <psapi.h>

using namespace v8;
using namespace node;

class Icon : ObjectWrap {
  private:
  public:
    Icon() {}
    ~Icon() {}
    static void Init(Handle<Object> exports) {
        NODE_SET_METHOD(exports, "getIcon", GetIcon);
    }

    static void GetIcon(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);

        if(hProcessSnap == INVALID_HANDLE_VALUE)
        {
            Local<Object> errorObj = Object::New(isolate);
            errorObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "An error has occured."));
            args.GetReturnValue().Set(errorObj);
            return;
        }

        Local<Object> returnObj = Object::New(isolate);

        returnObj->Set(String::NewFromUtf8(isolate, "smallIcon"), String::NewFromUtf8(isolate, "small"));
        returnObj->Set(String::NewFromUtf8(isolate, "largeIcon"), String::NewFromUtf8(isolate, "large"));

        args.GetReturnValue().Set(returnObj);
    }
};

extern "C" {
	void init(Handle<Object> exports) {
        Icon::Init(exports);
	}
	NODE_MODULE(icon, init)
}