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

#include "itksys/SystemTools.hxx"
#include "itkMacro.h"

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

  /** Get the root name */
  static std::string GetRootName(const std::string& filename);

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
