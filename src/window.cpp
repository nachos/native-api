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

NAN_METHOD(Minimize) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  ShowWindow(hwnd, 6);
}

NAN_METHOD(Maximize) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  ShowWindow(hwnd, 3);
}

NAN_METHOD(IsForeground) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  NanReturnValue(GetForegroundWindow() == hwnd ? NanTrue() : NanFalse());
}

NAN_METHOD(IsMinimized) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  NanReturnValue(IsIconic(hwnd) ? NanTrue() : NanFalse());
}

NAN_METHOD(SetTopMost) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE);
}

NAN_METHOD(Close) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  ShowWindow(hwnd, 0);
}

NAN_METHOD(SetToForeground) {
  NanScope();

  HWND hwnd = (HWND)args[0]->Uint32Value();

  ShowWindow(hwnd, 4);
  SetForegroundWindow(hwnd);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getAllWindows"),
    NanNew<FunctionTemplate>(GetAllWindows)->GetFunction());

  exports->Set(NanNew<String>("getWindowTitle"),
    NanNew<FunctionTemplate>(GetWindowTitle)->GetFunction());

  exports->Set(NanNew<String>("setToForeground"),
    NanNew<FunctionTemplate>(SetToForeground)->GetFunction());

  exports->Set(NanNew<String>("minimize"),
    NanNew<FunctionTemplate>(Minimize)->GetFunction());

  exports->Set(NanNew<String>("maximize"),
      NanNew<FunctionTemplate>(Maximize)->GetFunction());

  exports->Set(NanNew<String>("isForeground"),
      NanNew<FunctionTemplate>(IsForeground)->GetFunction());

  exports->Set(NanNew<String>("isMinimized"),
      NanNew<FunctionTemplate>(IsMinimized)->GetFunction());

  exports->Set(NanNew<String>("setTopMost"),
        NanNew<FunctionTemplate>(SetTopMost)->GetFunction());

  exports->Set(NanNew<String>("close"),
          NanNew<FunctionTemplate>(Close)->GetFunction());
}

NODE_MODULE(window, init)
