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
#ifndef __otbBandName_h
#define __otbBandName_h

namespace otb
{
namespace BandName
{

/**
* Provides a way to identify bands when passing the parameters
* to the radiometric functors.*
*/
enum BandName {BLUE, GREEN, RED, NIR, MIR};

// Note for landsat equivalence
// http://landsat.gsfc.nasa.gov/news/news-archive/sci_0017.html
// TM1 = Coastal
// TM2 = Blue
// TM3 = Green
// TM4 = Red
// TM5 = NIR
// TM6 = SWIR1 = MIR (SPOT5)
// TM7 = SWIR2
// TM8 = Panchro
// TM9 = Cirrus
//
}

}

#endif
