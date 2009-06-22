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
#ifndef __otbVisualizationPixelTraits_h
#define __otbVisualizationPixelTraits_h

#include "itkVariableLengthVector.h"
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"

namespace otb
{
/** \class VisualizationPixelTraits
*   \brief TODO

*  \ingroup Visualization
 */

class VisualizationPixelTraits {
public:

  template< class TScalarTypeInput, class TScalarTypeOutput >
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in, TScalarTypeOutput& out)
  {
     out = in[0];
  }

  template< class TScalarTypeInput, class TScalarTypeOutput >
  static void Convert(TScalarTypeInput in, itk::VariableLengthVector<TScalarTypeOutput> &out)
  {
    out.SetSize(1);
    out[0]=in;
  }

  template< class TScalarTypeInput, class TScalarTypeOutput >
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in, itk::VariableLengthVector<TScalarTypeOutput>& out)
  {
    out = in;
  }


  //To be continued
};

}

#endif
