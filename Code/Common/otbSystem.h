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
#ifndef _otbSystem_h
#define _otbSystem_h

#include <string>
#include <vector>

namespace otb
{


/** \class System
 * \brief System operations.
 *
 * System operations, filename manipulations, etc.
 *
 */
class /*ITK_EXPORT*/ System 
{
public:

  /** Standard class typedefs. */
  typedef System  Self;

  /** Get the extension of the file name */
  static std::string GetExtension( const std::string& filename );
  /** Get the root name */
  static std::string GetRootName( const std::string& filename );

  /** Get list of file find in a directory */
  static std::vector<std::string> Readdir(std::string pszPath);
};

} // namespace otb


#endif
