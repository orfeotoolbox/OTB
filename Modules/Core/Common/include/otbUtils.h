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
#ifndef __otbUtils_h
#define __otbUtils_h

#include "itkMacro.h"

namespace otb
{

/** \class Utils
 * \brief Utils operations.
 *
 * Some usefull utilities in use across the library
 *
 *
 * \ingroup OTBCommon
 */
namespace Utils
{
  /** Check that the values for lon, lat are reasonable */
  bool IsLonLatValid(double lon, double lat);

  template <class T>
    inline std::string ConvertToString (const T& v)
  {
    std::stringstream s;
    s << v;
    return s.str();
  }

}

} // namespace otb

#endif
