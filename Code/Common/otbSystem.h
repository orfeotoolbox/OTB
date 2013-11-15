/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSystem_h
#define __otbSystem_h

#include <string>
#include <vector>

#include "itkMacro.h"

#ifdef _WIN32
#define OTB_FILE_SEPARATOR '\\'
#else
#define OTB_FILE_SEPARATOR '/'
#endif

namespace otb
{

/** \class System
 * \brief System operations.
 *
 * System operations, filename or strings manipulations, etc.
 *
 */
class ITK_EXPORT System
{
public:

  /** Standard class typedefs. */
  typedef System Self;

  /** Return true if the parameter 'pszPath' specify a directory name) */
  static bool IsADirName(const std::string& pszPath);
  /** Return true if the parameter 'pszPath' specify a file name) */
  static bool IsAFileName(const std::string& pszPath);
  /** Get the extension of the file name */
  static std::string GetExtension(const std::string& filename);
  /** Get the root name */
  static std::string GetRootName(const std::string& filename);
  /** Get the Path of the file name */
  static std::string GetPathName(const std::string& filename);
  /** Get the short filename of the file name */
  static std::string GetShortFileName(const std::string& filename);
  /** Set to upper a string */
  static std::string SetToUpper(const std::string& str);
  /** Set to upper a string */
  static std::string SetToLower(const std::string& str);

  /** Get list of file find in a directory */
  static std::vector<std::string> Readdir(const std::string& pszPath);

  /** Parse the hdf subset name */
  static bool ParseHdfSubsetName(const std::string& id, std::string& key, std::string& name);

  /** Parse a filename with dataset information */
  static bool ParseHdfFileName(const std::string& id, std::string& file, unsigned int& datasetNum);

  /** Parse a filename with additional information */
  static bool ParseFileNameForAdditonalInfo(const std::string& id, std::string& file, unsigned int& addNum);
};

} // namespace otb

#endif
