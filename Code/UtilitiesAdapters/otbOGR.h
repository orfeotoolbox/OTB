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
#ifndef __otbOGR_h
#define __otbOGR_h

namespace otb {
  /**\defgroup Geometry Geometry
   * \ingroup Thematic
   * Classes and functions aimed toward the manipulation of set of geometric
   * objects.
   * This module mainly provides an encapsulation of <a
   * href="http://www.gdal.org/ogr/">OGR API</a>.
   * @{
   */
  namespace ogr { }
  //@}
} // namespace otb

#include "otbOGRDriversInit.h"
#include "otbOGRDataSourceWrapper.h"

/**\ingroup Geometry
 * \file    otbOGR.h
 * How to include all include files dedicated to OGR encapsulation in OTB...
 */


#endif // __otbOGR_h
