#include <nan.h>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <list>

#include <stdio.h>
#include <psapi.h>
#include <string>

#include <vector>

using namespace v8;
using namespace node;

BOOL IsTaskbarWindow(HWND hwnd) {
  if (IsWindowVisible(hwnd) && GetParent(hwnd) == 0) {
    bool bNoOwner = (GetWindow(hwnd, GW_OWNER) == 0);
    int lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    if (!bNoOwner) return FALSE;
    if ((((lExStyle & WS_EX_TOOLWINDOW) == 0)))
    {
        return TRUE;
    }
  }

  return FALSE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  if (!IsTaskbarWindow(hwnd)) {
    return TRUE;
  }

  std::list<Local<Object>> *windows = (std::list<Local<Object>>*)lParam;
  Local<Object> obj = NanNew<Object>();

  DWORD processID;
  char title[1024];
  DWORD threadID = GetWindowThreadProcessId(hwnd, &processID);
  GetWindowText(hwnd, title, sizeof(title));

  obj->Set(NanNew<String>("threatID"), NanNew<Number>((unsigned int)threadID));
  obj->Set(NanNew<String>("processID"), NanNew<Number>((unsigned int)processID));
  obj->Set(NanNew<String>("handle"), NanNew<Number>((unsigned int)hwnd));
  obj->Set(NanNew<String>("title"), NanNew<String>(title));

  (*windows).push_back(obj);
  return TRUE;
}

NAN_METHOD(GetAllWindows) {
  NanScope();

  std::list<Local<Object>> windows;

  EnumWindows(EnumWindowsProc, (LPARAM)&windows);

  Local<Array> returnValue = NanNew<Array>();

  int i = 0;
  for (std::list<Local<Object>>::iterator it=windows.begin(); it != windows.end(); ++it) {
      returnValue->Set(i, *it);
      i++;
  }

  NanReturnValue(returnValue);
}

NAN_METHOD(GetWindowTitle) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  char title[1024];
  GetWindowText(hwnd, title, sizeof(title));

  NanReturnValue(NanNew<String>(title));
  // If hebrew is not working
  //NanReturnValue(NanNew(*NanAsciiString(title)));
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getAllWindows"),
    NanNew<FunctionTemplate>(GetAllWindows)->GetFunction());

  exports->Set(NanNew<String>("getWindowTitle"),
    NanNew<FunctionTemplate>(GetWindowTitle)->GetFunction());
}

NODE_MODULE(window, init)
