/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbSystem.h"
#include <string> // strdup
#include <cstdlib>

#if (defined(WIN32) || defined(WIN32CE)) && !defined(__CYGWIN__) && !defined(__MINGW32__)

/*=====================================================================
                   WIN32 / MSVC++ implementation
 *====================================================================*/
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#ifndef WIN32CE
#  include <io.h>
#else
#  include <wce_io.h>
#endif
#else
/*=====================================================================
                      POSIX (Unix) implementation
 *====================================================================*/
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#endif

namespace otb
{

//GetRootName from uiig library.
std::string
System::GetRootName(const std::string& filename)
{
  const std::string fileExt = itksys::SystemTools::GetFilenameLastExtension(filename);

  // Create a base filename
  // i.e Image.ent --> Image
  if (fileExt.length() > 0)
    {
    const std::string::size_type it = filename.find_last_of(fileExt);
    std::string                  baseName(filename, 0, it - fileExt.length() + 1);
    return (baseName);
    }
  //Default to return same as input when the extension is nothing (Analyze)
  return (filename);
}


#if (defined(WIN32) || defined(WIN32CE)) && !defined(__CYGWIN__) && !defined(__MINGW32__)

/*=====================================================================
                   WIN32 / MSVC++ implementation
 *====================================================================*/

std::vector<std::string> System::Readdir(const std::string&  pszPath)
{
  WIN32_FIND_DATA          c_file;
  HANDLE                   hFile = INVALID_HANDLE_VALUE;
  std::vector<std::string> listFileFind;
  std::string              pszFileSpec;
  std::string              path(pszPath);

  if (pszPath.empty() == true) path = ".";

  pszFileSpec = path + "\\*.*";

  if ((hFile = FindFirstFile(pszFileSpec.c_str(), &c_file)) != INVALID_HANDLE_VALUE)
    {
    do
      {
      listFileFind.push_back(c_file.cFileName);
      }
    while (FindNextFile(hFile, &c_file) != 0);

    FindClose(hFile);
    }

  return listFileFind;
}

#else

/*=====================================================================
                      POSIX (Unix) implementation
 *====================================================================*/

/**
 * Read names in a directory.
 *
 * This function abstracts access to directory contains.  It returns a
 * list of strings containing the names of files, and directories in this
 * directory.  The resulting string list becomes the responsibility of the
 * application and should be freed with CSLDestroy() when no longer needed.
 *
 * Note that no error is issued via CPLError() if the directory path is
 * invalid, though NULL is returned.
 *
 * @param pszPath the relative, or absolute path of a directory to read.
 * @return The list of entries in the directory, or NULL if the directory
 * doesn't exist.
 */

std::vector<std::string> System::Readdir(const std::string& pszPath)
{
  DIR *                    hDir;
  std::vector<std::string> listFileFind;
  struct dirent *          psDirEntry;
  std::string              path(pszPath);

  if (pszPath.empty() == true) path = ".";

  if ((hDir = opendir(path.c_str())) != nullptr)
    {
    while ((psDirEntry = readdir(hDir)) != nullptr)
      {
      listFileFind.push_back(psDirEntry->d_name);
      }

    closedir(hDir);
    }
  return listFileFind;
}

#endif


/** From an hdf subset name such as:
 * SUBDATASET_7_NAME=HDF4_EOS:EOS_GRID:"file/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf":MODIS_Grid_16DAY...
 * it will set key to SUBDATASET_7_NAME
 * and name to HDF4_EOS:EOS_GRID:"file/MOD13Q1.A2010001.h17v05.005.2010028003734.hdf":MODIS_Grid_16DAY...
 */
bool System::ParseHdfSubsetName(const std::string& id, std::string& key, std::string& name)
{
  std::size_t pos = id.find("=");
  if (pos == std::string::npos) return false;
  key = id.substr(0, pos);
  name = id.substr(pos+1, id.size() - pos - 1);
  return true;
}

/** Parse and hdf filename indicating the dataset number
 * if id is filelocation/file.hdf:10
 * it will return filelocation/file.hdf in file
 * and 10 in datasetNum
 */
bool System::ParseHdfFileName(const std::string& id, std::string& file, unsigned int& datasetNum)
{
  std::size_t pos = id.rfind(":");
  if (pos == std::string::npos) return false;
  std::string datasetNumString(id.substr(pos+1, id.size() - pos - 1));
  datasetNum = atoi(datasetNumString.c_str());
  // a value of 0 could be the sign that the conversion failed, we need to check that the character was really 0
  if (datasetNum == 0)
    {
      if (datasetNumString.compare("0") != 0)
        {
        return false;
        }
    }
  file = id.substr(0, pos);
  return true;
}

/** Parse a filename indicating an additional information
 * if id is filelocation/file.***:10
 * it will return filelocation/file.*** in file
 * and 10 in addNum additional
 */
bool System::ParseFileNameForAdditionalInfo(const std::string& id, std::string& file, unsigned int& addNum)
{
  std::size_t pos = id.rfind(":");
  if (pos == std::string::npos) return false;

  std::string addNumString(id.substr(pos+1, id.size() - pos - 1));
  addNum = atoi(addNumString.c_str());
  // a value of 0 could be the sign that the conversion failed, we need to check that the character was really 0
  if (addNum == 0)
    {
      if (addNumString.compare("0") != 0)
        {
        return false;
        }
    }
  file = id.substr(0, pos);
  return true;
}

bool System::IsInteractive(int fd)
{
#if (defined(WIN32) || defined(WIN32CE)) && !defined(__CYGWIN__) && !defined(__MINGW32__)
  // Windows implementation
  HANDLE hcon;

  /* get OS handle of the file descriptor */
  hcon = (HANDLE) _get_osfhandle(fd);
  if (hcon == INVALID_HANDLE_VALUE)
    return false;

  /* check if its a device (i.e. console, printer, serial port) */
  if (GetFileType(hcon) != FILE_TYPE_CHAR)
    return false;

  /* check if its a handle to a console output screen buffer */
  CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
  DWORD mode;
  if (!fd)
    {
    if (!GetConsoleMode(hcon, &mode))
      return false;
    }
  else if (!GetConsoleScreenBufferInfo(hcon, &screenBufferInfo))
    return false;

  return true;
#else
  // Unix implementation
  return isatty(fd);
#endif
}

}
