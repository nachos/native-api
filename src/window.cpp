#include <nan.h>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>

#include <list>

#include <stdio.h>
#include <stdlib.h>
#include <psapi.h>
#include <string>

#include <sstream>

#pragma data_seg(".SHARED")
HHOOK hHook = 0;
HWND _hwnd = 0;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.SHARED,RWS")

//#include <windowsx.h>

#include <vector>

#include <iostream>
#include <fstream>
using namespace std;

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

  obj->Set(NanNew<String>("threadID"), NanNew<Number>((unsigned int)threadID));
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

NAN_METHOD(GetCurrentWindow) {
  NanScope();

  Local<Object> returnValue = NanNew<Object>();

  returnValue->Set(NanNew<String>("processID"), NanNew<Number>((unsigned int)  GetCurrentProcessId()));

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

LRESULT CALLBACK DisableZWindowProc(int nCode, WPARAM wParam, LPARAM lParam) {
  if (nCode >= 0) {
    // lParam is the pointer to the struct containing the data needed, so cast and assign it to kdbStruct.
    PCWPSTRUCT cwpStruct = (PCWPSTRUCT)lParam;
    // MessageBox(NULL, "He;p", "key pressed", MB_ICONINFORMATION);
    // a key (non-system) is pressed.

    if (cwpStruct->message == WM_SETFOCUS) {
         fstream  myfile;
                myfile.open ("c:\\example.txt", fstream::app|fstream::out);
                myfile << "Hello";
                myfile << "\n";
                        myfile.close();

          SetWindowPos(_hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
         return (FALSE);
    }

    /*if (cwpStruct->message == WM_WINDOWPOSCHANGING) {
        // F1 is pressed!

        LPWINDOWPOS lpWindowPos = (LPWINDOWPOS)cwpStruct->lParam;

        //fstream  myfile;
        //myfile.open ("c:\\example.txt", fstream::app|fstream::out);
        //myfile << lpWindowPos->flags << "\n";
        //myfile << "\n";


        //lpWindowPos->flags |= SWP_NOZORDER;
        lpWindowPos->flags &= (~SWP_NOZORDER);
        lpWindowPos->hwndInsertAfter = HWND_BOTTOM;
        //SetWindowPos(_hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
        //myfile << lpWindowPos->flags;

        //myfile << "\n";
        //myfile << "\n";

        //myfile.close();

      //MessageBox(NULL, "Helllllo", "key pressed", MB_ICONINFORMATION);
      return 0;
    }*/
  }



  // call the next hook in the hook chain. This is nessecary or your hook chain will break and the hook stops
  return CallNextHookEx(hHook, nCode, wParam, lParam);
}

HMODULE GetCurrentModule()
{ // NB: XP+ solution!
  HMODULE hModule = NULL;
  GetModuleHandleEx(
    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
    (LPCTSTR)GetCurrentModule,
    &hModule);

  return hModule;
}

NAN_METHOD(DisableZIndexChange) {
  NanScope();

  Local<Object> returnObj = NanNew<Object>();

  HWND hwnd = (HWND)args[0]->Uint32Value();
  _hwnd = hwnd;

  // SetWindowPos(_hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

  DWORD processID;
  DWORD windowThread = GetWindowThreadProcessId(hwnd, &processID);

  HINSTANCE hInstance = GetCurrentModule();

  // hHook = SetWindowsHookEx(WH_CALLWNDPROC, DisableZWindowProc, hInstance, windowThread);

  //HWND hWndProgMan = FindWindow("Progman", "Program Manager");
  //SetParent(hwnd, hWndProgMan);

  fstream  myfile;
  myfile.open ("c:\\example.txt", fstream::app|fstream::out);

   LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

   myfile << "First Get: " << style << "\n";

   SetLastError(0);

    LONG_PTR result = SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_NOACTIVATE);
    int lastError = GetLastError();

    myfile << "Set Returns: " << result << "\n";
    myfile << "Error code: " << lastError << "\n";

    LONG_PTR style2 = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    myfile << "Second Get: " << style2 << "\n";

    myfile << "\n";
    myfile.close();

  /*if (lastError != 0) {

    LPWSTR lpMsg = NULL;
    FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, lastError, 0, (LPWSTR)&lpMsg, 0, NULL);
    char msg[1024];
    wcstombs(msg, lpMsg, 1024);

    returnObj->Set(NanNew<String>("error"), NanNew<String>(msg));
    returnObj->Set(NanNew<String>("errorCode"), NanNew<Number>(lastError));
  } else {
    SetWindowPos(hwnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    returnObj->Set(NanNew<String>("style"), NanNew<Number>((unsigned int)style));
    returnObj->Set(NanNew<String>("result"), NanNew<Number>((unsigned int)(result)));
  }

  NanReturnValue(returnObj);*/
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

  exports->Set(NanNew<String>("getCurrentWindow"),
    NanNew<FunctionTemplate>(GetCurrentWindow)->GetFunction());

  exports->Set(NanNew<String>("disableZIndexChange"),
    NanNew<FunctionTemplate>(DisableZIndexChange)->GetFunction());
}

NODE_MODULE(window, init)
