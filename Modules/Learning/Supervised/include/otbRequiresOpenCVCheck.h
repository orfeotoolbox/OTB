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
#ifndef __otbRequiresOpenCVCheck_h
#define __otbRequiresOpenCVCheck_h

#include "otbConfigure.h"
#ifndef OTB_USE_OPENCV
#  error "You need to enable OpenCV to compile this file"
#endif

#endif
