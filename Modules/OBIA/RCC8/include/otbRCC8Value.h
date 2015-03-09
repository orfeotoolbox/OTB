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
#ifndef __otbRCC8Value_h
#define __otbRCC8Value_h

namespace otb
{
// Coding enum system to represent the RCC8 relation values.
typedef enum
  {
  OTB_RCC8_DC,
  OTB_RCC8_EC,
  OTB_RCC8_PO,
  OTB_RCC8_TPP,
  OTB_RCC8_TPPI,
  OTB_RCC8_NTPP,
  OTB_RCC8_NTPPI,
  OTB_RCC8_EQ
  }
RCC8Value;
} // End namespace otb
#endif
