// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// The file contains the implementation of the File methods specific to
// the win32 platform.

// TODO: likely there are better ways to accomplish Delete and
// CreateNewTempFile.

#include "kml/base/file.h"
#include <windows.h>
#include <tchar.h>
#include <xstring>
#include <algorithm>

namespace kmlbase {

// Internal to the win32 file class. We need a conversion from string to
// LPCWSTR.
static std::wstring Str2Wstr(const string& str) {
  std::wstring wstr(str.length(), L'');
  std::copy(str.begin(), str.end(), wstr.begin());
  return wstr;
}

// Internal to the win32 file class. We need a conversion from std::wstring to
// string.
string Wstr2Str(const std::wstring& wstr) {
  size_t s = wstr.size();
  string str(static_cast<int>(s+1), 0);
  WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(s), &str[0],
                      static_cast<int>(s), NULL, NULL);
  return str;
}

bool File::Exists(const string& full_path) {
  if (full_path.empty()) {
    return false;
  }
  std::wstring wstr = Str2Wstr(full_path);
  DWORD attrs = ::GetFileAttributes(wstr.c_str());
  return (attrs != INVALID_FILE_ATTRIBUTES) &&
    ((attrs & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

bool File::Delete(const string& filepath) {
  if (filepath.empty()) {
    return false;
  }
  std::wstring wstr = Str2Wstr(filepath);
  return ::DeleteFile(wstr.c_str()) ? true : false;
}

static const unsigned int BUFSIZE = 1024;
DWORD dwBufSize = BUFSIZE;
DWORD dwRetVal;
TCHAR lpPathBuffer[BUFSIZE];
UINT uRetVal;
TCHAR szTempName[BUFSIZE];

// http://msdn.microsoft.com/en-us/library/aa363875(VS.85).aspx
bool File::CreateNewTempFile(string* path) {
  if (!path) {
    return false;
  }
  // Get the temp path.
  dwRetVal = ::GetTempPath(dwBufSize, lpPathBuffer);
  if (dwRetVal > dwBufSize || (dwRetVal == 0)) {
    return false;
  }
  // Create a temporary file.
  uRetVal = ::GetTempFileName(lpPathBuffer, TEXT("libkml"), 0, szTempName);
  if (uRetVal == 0) {
    return false;
  }
  string str = Wstr2Str(szTempName);
  path->assign(str.c_str(), strlen(str.c_str()));
  return true;
}

}  // end namespace kmlbase
