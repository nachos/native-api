#pragma comment(lib, "psapi.lib")

#include <nan.h>

#include <windows.h>
#include <winbase.h>
#include <tlhelp32.h>

#include <tchar.h>
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
    current->Set(NanNew<String>("processID"), NanNew<Number>((unsigned int)pe32.th32ProcessID));
    current->Set(NanNew<String>("parentProcessID"), NanNew<Number>((unsigned int)pe32.th32ParentProcessID));
    //current->Set(NanNew<String>("moduleID"), NanNew<Number>((unsigned int)pe32.th32ModuleID));

    /*curr->Set(String::NewFromUtf8(isolate, "cntUsage"), Integer::New(isolate, pe32.cntUsage));
    curr->Set(String::NewFromUtf8(isolate, "th32DefaultHeapID"), Integer::New(isolate, pe32.th32DefaultHeapID));
    curr->Set(String::NewFromUtf8(isolate, "th32ModuleID"), Integer::New(isolate, pe32.th32ModuleID));
    curr->Set(String::NewFromUtf8(isolate, "cntThreads"), Integer::New(isolate, pe32.cntThreads));
    curr->Set(String::NewFromUtf8(isolate, "th32ParentProcessID"), Integer::New(isolate, pe32.th32ParentProcessID));
    curr->Set(String::NewFromUtf8(isolate, "pcPriClassBase"), Integer::New(isolate, pe32.pcPriClassBase));
    curr->Set(String::NewFromUtf8(isolate, "dwFlags"), Integer::New(isolate, pe32.dwFlags));*/

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
    char processFile[1024];
    GetModuleFileNameEx(hProcess, NULL, processFile, 1024);

    current->Set(NanNew<String>("processFile"), NanNew<String>(processFile));

    CloseHandle(hProcess);

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
