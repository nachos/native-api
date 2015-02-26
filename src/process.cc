#pragma comment(lib, "psapi.lib")

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

#include <uv.h>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <list>

#include <stdio.h>
#include <psapi.h>

using namespace v8;
using namespace node;

class Proc : ObjectWrap {
  private:
  public:
    Proc() {}
    ~Proc() {}
    static void Init(Handle<Object> exports) {
        NODE_SET_METHOD(exports, "getAllProcesses", GetAllProcesses);
    }

    static void GetAllProcesses(const FunctionCallbackInfo<Value>& args) {
        Isolate* isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);

        Local<Object> returnObj = Object::New(isolate);
        Local<Object> errorObj = Object::New(isolate);

        HANDLE hProcessSnap;
        PROCESSENTRY32 pe32;

        // Take a snapshot of all processes in the system.
        hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if(hProcessSnap == INVALID_HANDLE_VALUE)
        {
            errorObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "CreateToolhelp32Snapshot (of processes)"));
            args.GetReturnValue().Set(errorObj);
            return;
        }

        // Set the size of the structure before using it.
        pe32.dwSize = sizeof(PROCESSENTRY32);

        // Retrieve information about the first process,
        // and exit if unsuccessful
        if(!Process32First(hProcessSnap, &pe32))
        {
            errorObj->Set(String::NewFromUtf8(isolate, "error"), String::NewFromUtf8(isolate, "Process32First"));
            args.GetReturnValue().Set(errorObj);
            return;
        }

        // Now walk the snapshot of processes, and
        // display information about each process in turn
        std::list<Handle<Object>> processes;

        do
        {
            Local<Object> curr = Object::New(isolate);

            curr->Set(String::NewFromUtf8(isolate, "name"), String::NewFromUtf8(isolate, pe32.szExeFile));
            /*curr->Set(String::NewFromUtf8(isolate, "cntUsage"), Integer::New(isolate, pe32.cntUsage));
            curr->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Integer::New(isolate, pe32.th32ProcessID));
            curr->Set(String::NewFromUtf8(isolate, "th32DefaultHeapID"), Integer::New(isolate, pe32.th32DefaultHeapID));
            curr->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Integer::New(isolate, pe32.th32ModuleID));
            curr->Set(String::NewFromUtf8(isolate, "cntThreads"), Integer::New(isolate, pe32.cntThreads));
            curr->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Integer::New(isolate, pe32.th32ParentProcessID));
            curr->Set(String::NewFromUtf8(isolate, "pcPriClassBase"), Integer::New(isolate, pe32.pcPriClassBase));
            curr->Set(String::NewFromUtf8(isolate, "dwFlags"), Integer::New(isolate, pe32.dwFlags));*/

            processes.push_back(curr);
        } while( Process32Next( hProcessSnap, &pe32 ) );

        CloseHandle( hProcessSnap );

        Handle<Array> returnValue = Array::New(isolate, processes.size());

        int i = 0;
        for (std::list<Handle<Object>>::iterator it=processes.begin(); it != processes.end(); ++it) {
            returnValue->Set(i, *it);
            i++;
        }

        args.GetReturnValue().Set(returnValue);
    }
};

extern "C" {
	void init(Handle<Object> exports) {
        Proc::Init(exports);
	}
	NODE_MODULE(proc, init)
}