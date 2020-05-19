/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <iostream>

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage : " << argv[0] << "  library_file" << std::endl;
    return 1;
  }
  const char* input = argv[1];
#if defined(_WIN32) && !defined(__CYGWIN__)
  HMODULE library = LoadLibrary(input);
  if (library == NULL)
  {
    std::cout << "Failed to load " << input << std::endl;
    LPVOID lpMsgBuf = NULL;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                  (LPTSTR)&lpMsgBuf, 0, NULL);
    if (lpMsgBuf)
    {
      std::cout << (char*)lpMsgBuf << std::endl;
      LocalFree(lpMsgBuf);
    }
  }
  else
  {
    FreeLibrary(library);
  }
#else
  void* library = dlopen(input, RTLD_LAZY);
  if (library == NULL)
  {
    std::cout << "Failed to load " << input << std::endl;
    std::cout << dlerror() << std::endl;
  }
  else
  {
    dlclose(library);
  }
#endif
  return 0;
}
