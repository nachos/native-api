#include <nan.h>

#include <windows.h>
#include <tlhelp32.h>

#include <psapi.h>

using namespace v8;
using namespace node;

NAN_METHOD(GetAllProcesses) {
  NanScope();

  Local<Object> returnObj = NanNew<Object>();

  returnObj->Set(NanNew<String>("errorCode"), NanNew<Number>(0));


  HANDLE hProcessSnap;
  PROCESSENTRY32 pe32;

  // Take a snapshot of all processes in the system.
  hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  int lastError = GetLastError();

  if(lastError != 0)
  {
      returnObj->Set(NanNew<String>("errorCode"), NanNew<Number>(lastError));
      NanReturnValue(returnObj);
  }

  // Set the size of the structure before using it.
  pe32.dwSize = sizeof(PROCESSENTRY32);

  // Retrieve information about the first process,
  // and exit if unsuccessful
  if(!Process32First(hProcessSnap, &pe32))
  {
    returnObj->Set(NanNew<String>("errorCode"), NanNew<Number>(GetLastError()));
    NanReturnValue(returnObj);
  }

  int i = 0;

  Local<Array> processes = NanNew<Array>();

  do
  {
    Local<Object> current = NanNew<Object>();

    current->Set(NanNew<String>("name"), NanNew<String>(pe32.szExeFile));
    /*curr->Set(String::NewFromUtf8(isolate, "cntUsage"), Integer::New(isolate, pe32.cntUsage));
    curr->Set(String::NewFromUtf8(isolate, "th32ProcessID"), Integer::New(isolate, pe32.th32ProcessID));
    curr->Set(String::NewFromUtf8(isolate, "th32DefaultHeapID"), Integer::New(isolate, pe32.th32DefaultHeapID));
    curr->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Integer::New(isolate, pe32.th32ModuleID));
    curr->Set(String::NewFromUtf8(isolate, "cntThreads"), Integer::New(isolate, pe32.cntThreads));
    curr->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Integer::New(isolate, pe32.th32ParentProcessID));
    curr->Set(String::NewFromUtf8(isolate, "pcPriClassBase"), Integer::New(isolate, pe32.pcPriClassBase));
    curr->Set(String::NewFromUtf8(isolate, "dwFlags"), Integer::New(isolate, pe32.dwFlags));*/

    processes->Set(NanNew<Number>(i++), current);
  } while(Process32Next(hProcessSnap, &pe32));

  returnObj->Set(NanNew<String>("processes"), processes);

  CloseHandle(hProcessSnap);

  NanReturnValue(returnObj);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getAllProcesses"),
    NanNew<FunctionTemplate>(GetAllProcesses)->GetFunction());
}
NODE_MODULE(proc, init)
