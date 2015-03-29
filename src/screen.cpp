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

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData){
  std::list<Local<Object>> *screens = (std::list<Local<Object>>*)dwData;

  MONITORINFO target;
  target.cbSize = sizeof(MONITORINFO);
  GetMonitorInfo(hMonitor, &target);

  Local<Object> obj = NanNew<Object>();
  Local<Object> recBounds = NanNew<Object>();
  Local<Object> recWork = NanNew<Object>();

  recBounds->Set(NanNew<String>("left"), NanNew<Number>(target.rcMonitor.left));
  recBounds->Set(NanNew<String>("top"), NanNew<Number>(target.rcMonitor.top));
  recBounds->Set(NanNew<String>("right"), NanNew<Number>(target.rcMonitor.right));
  recBounds->Set(NanNew<String>("bottom"), NanNew<Number>(target.rcMonitor.bottom));

  recWork->Set(NanNew<String>("left"), NanNew<Number>(target.rcWork.left));
  recWork->Set(NanNew<String>("top"), NanNew<Number>(target.rcWork.top));
  recWork->Set(NanNew<String>("right"), NanNew<Number>(target.rcWork.right));
  recWork->Set(NanNew<String>("bottom"), NanNew<Number>(target.rcWork.bottom));

  obj->Set(NanNew<String>("handle"), NanNew<Number>((unsigned int)hMonitor));
  obj->Set(NanNew<String>("bounds"), recBounds);
  obj->Set(NanNew<String>("work"), recWork);
  obj->Set(NanNew<String>("primary"), target.dwFlags == MONITORINFOF_PRIMARY ? NanTrue() : NanFalse());

  (*screens).push_back(obj);
  return TRUE;
}

NAN_METHOD(GetAllScreens) {
  NanScope();

  std::list<Local<Object>> screens;

  EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&screens);

  Local<Array> returnValue = NanNew<Array>();

  int i = 0;
  for (std::list<Local<Object>>::iterator it=screens.begin(); it != screens.end(); ++it) {
      returnValue->Set(i, *it);
      i++;
  }

  NanReturnValue(returnValue);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getAllScreens"),
    NanNew<FunctionTemplate>(GetAllScreens)->GetFunction());
}

NODE_MODULE(screen, init)
