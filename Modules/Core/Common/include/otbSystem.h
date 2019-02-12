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

#ifndef otbSystem_h
#define otbSystem_h

#include <string>
#include <vector>

#include "itksys/SystemTools.hxx"
#include "itkMacro.h"

#include "OTBCommonExport.h"

namespace otb
{

/** \class System
 * \brief System operations.
 *
 * System operations, filename or strings manipulations, etc.
 *
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT System
{
public:

  /** Standard class typedefs. */
  typedef System Self;

  /** Get the root name */
  static std::string GetRootName(const std::string& filename);

  /** Get list of file find in a directory */
  static std::vector<std::string> Readdir(const std::string& pszPath);

  /** Parse the hdf subset name */
  static bool ParseHdfSubsetName(const std::string& id, std::string& key, std::string& name);

  /** Parse a filename with dataset information */
  static bool ParseHdfFileName(const std::string& id, std::string& file, unsigned int& datasetNum);

  /** Parse a filename with additional information */
  static bool ParseFileNameForAdditionalInfo(const std::string& id, std::string& file, unsigned int& addNum);

  /** Returns true if the file descriptor fd is interactive (i.e. like isatty on unix) */
  static bool IsInteractive(int fd);
};

} // namespace otb

#endif
