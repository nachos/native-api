#pragma comment(lib, "psapi.lib")

#include <nan.h>

#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

#include <uv.h>

#include <windows.h>

#include <stdio.h>
#include <psapi.h>

#include <stdlib.h>

#include <string>


using namespace v8;
using namespace node;
using namespace std;

static char* Bitmap2Bytes(HBITMAP hBitmap, int& len)
{
 BITMAP bmpObj;
 HDC hDCScreen;
 int iRet;
 DWORD dwBmpSize;

 BITMAPFILEHEADER    bmfHeader;
 LPBITMAPINFO        lpbi;
 const DWORD dwSizeOfBmfHeader = sizeof(BITMAPFILEHEADER);
 DWORD dwSizeOfBmInfo = sizeof(BITMAPINFO);

 hDCScreen = GetDC(NULL);
 GetObject(hBitmap, sizeof(BITMAP), &bmpObj);

 HGLOBAL hDIB = GlobalAlloc(GHND, dwSizeOfBmInfo + 8);
 lpbi = (LPBITMAPINFO)GlobalLock(hDIB);
 lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

 // Gets the "bits" from the bitmap and copies them into a buffer
 // which is pointed to by lpbi
 iRet = GetDIBits(hDCScreen, hBitmap, 0,
 (UINT)bmpObj.bmHeight,
 NULL,
 lpbi, DIB_RGB_COLORS);
 //ASSERT(iRet > 0);

 // only 16 and 32 bit images are supported.
 //ASSERT(lpbi->bmiHeader.biBitCount == 16 || lpbi->bmiHeader.biBitCount == 32);
 if(lpbi->bmiHeader.biCompression == BI_BITFIELDS)
 dwSizeOfBmInfo += 8;

 dwBmpSize = lpbi->bmiHeader.biSizeImage;
 HGLOBAL hDIBBmData = GlobalAlloc(GHND, dwBmpSize);
 char* lpbitmap = (char*)GlobalLock(hDIBBmData);

 iRet = GetDIBits(hDCScreen, hBitmap, 0,
 (UINT)bmpObj.bmHeight,
 lpbitmap,
 lpbi, DIB_RGB_COLORS);
 //ASSERT(iRet > 0);

 DWORD dwSizeofDIB   =    dwBmpSize + dwSizeOfBmfHeader + dwSizeOfBmInfo;
 bmfHeader.bfOffBits = (DWORD)dwSizeOfBmfHeader + (DWORD)dwSizeOfBmInfo;
 bmfHeader.bfSize = dwSizeofDIB;
 bmfHeader.bfType = 0x4D42; //BM
 bmfHeader.bfReserved1 = bmfHeader.bfReserved2 = 0;

 char* arrData = new char[dwSizeofDIB];
 memcpy(arrData, &bmfHeader, dwSizeOfBmfHeader);
 memcpy(arrData + dwSizeOfBmfHeader, lpbi, dwSizeOfBmInfo);
 memcpy(arrData + dwSizeOfBmfHeader + dwSizeOfBmInfo, lpbitmap, dwBmpSize);

 GlobalUnlock(hDIB);
 GlobalUnlock(hDIBBmData);
 GlobalFree(hDIB);
 GlobalFree(hDIBBmData);
 ReleaseDC(NULL, hDCScreen);

 len = dwSizeofDIB;
 return arrData;
}

HBITMAP icon_to_bitmap(HICON hIcon) {
  HDC hDC = GetDC(NULL);
  HDC hMemDC = CreateCompatibleDC(hDC);
  HBITMAP hMemBmp = CreateCompatibleBitmap(hDC, 32, 32);
  HBITMAP hResultBmp = NULL;
  HGDIOBJ hOrgBMP = SelectObject(hMemDC, hMemBmp);

  DrawIconEx(hMemDC, 0, 0, hIcon, 32, 32, 0, NULL, DI_NORMAL);

  hResultBmp = hMemBmp;
  hMemBmp = NULL;

  SelectObject(hMemDC, hOrgBMP);
  DeleteDC(hMemDC);
  ReleaseDC(NULL, hDC);
  DestroyIcon(hIcon);
  return hResultBmp;
}

void CharToByte(char* chars, byte* bytes, unsigned int count){
    for(unsigned int i = 0; i < count; i++)
    	bytes[i] = (byte)chars[i];
}

NAN_METHOD(GetIcon) {
  NanScope();

  char* filepath = *NanAsciiString(args[0]);

  WORD a = 0; // icon index
  WORD b; // returned icon id
  HICON hIcon = ExtractAssociatedIconEx(NULL, filepath, &a, &b);

  //HBITMAP hBitmap = icon_to_bitmap(ico);
  ICONINFO iconinfo;
  GetIconInfo(hIcon, &iconinfo);
  HBITMAP hBitmap = iconinfo.hbmColor;

  int len;
  char* res = Bitmap2Bytes(hBitmap, len);

  Local<Array> returnValue = NanNew<Array>();

  for (int i = 0; i < len; i++) {
    returnValue->Set(i, NanNew(res[i]));
  }

  Local<Object> obj = NanNew<Object>();
  obj->Set(NanNew<String>("handle"), returnValue);

  DestroyIcon(hIcon);

  NanReturnValue(obj);
}

void init(Handle<Object> exports) {
  exports->Set(NanNew<String>("getIcon"),
    NanNew<FunctionTemplate>(GetIcon)->GetFunction());
}

NODE_MODULE(icon, init);