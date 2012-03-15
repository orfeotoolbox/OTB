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
#ifndef __otbTinyXML_h
#define __otbTinyXML_h

/**
 * TinyXML header uses the TINYXML_USE_STL macro
 * This must be defined before including the tinyxml header.
 *
 * Using the upstream headers, it depends on how tinyxml is compiled,
 * which is detected by a try_compile test
 * Note that Debian patches the tinyxml.h to unconditionnally define TINYXML_USE_STL
 */
#include "otbConfigure.h"
#include "tinyxml.h"

#endif
