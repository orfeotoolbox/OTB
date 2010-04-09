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

  /** Conversion methods from and to VariableLengthVector*/
  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in, TScalarTypeOutput& out)
  {
    out = in[0];
  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(TScalarTypeInput in, itk::VariableLengthVector<TScalarTypeOutput>& out)
  {
    out.SetSize(1);
    out[0] = in;
  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in,
                      itk::VariableLengthVector<TScalarTypeOutput>& out)
  {
    out = in;
  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in, itk::RGBPixel<TScalarTypeOutput>& out)
  {
    out[0] = in[0];
    if (in.Size() >= 2) out[1] = in[1]; else out[1] = 0;
    if (in.Size() >= 3) out[2] = in[2]; else out[2] = 0;

  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(itk::RGBPixel<TScalarTypeInput> in, itk::VariableLengthVector<TScalarTypeOutput>& out)
  {
    out.SetSize(3);
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(const itk::VariableLengthVector<TScalarTypeInput>& in, itk::RGBAPixel<TScalarTypeOutput>& out)
  {
    out[0] = in[0];
    if (in.Size() >= 2) out[1] = in[1]; else out[1] = 0;
    if (in.Size() >= 3) out[2] = in[2]; else out[2] = 0;

  }

  template<class TScalarTypeInput, class TScalarTypeOutput>
  static void Convert(itk::RGBAPixel<TScalarTypeInput> in, itk::VariableLengthVector<TScalarTypeOutput>& out)
  {
    out.SetSize(3);
    out[0] = in[0];
    out[1] = in[1];
    out[2] = in[2];
  }

  /** PixelSize method which return the size of a pixel (portion to be displayed) */
  template<class TScalarTypeInput>
  static unsigned int PixelSize(const itk::VariableLengthVector<TScalarTypeInput>& in)
  {
    return in.Size();
  }

  template<class TScalarTypeInput>
  static unsigned int PixelSize(const itk::RGBPixel<TScalarTypeInput>&)
  {
    return 3;
  }

  template<class TScalarTypeInput>
  static unsigned int PixelSize(const itk::RGBAPixel<TScalarTypeInput>&)
  {
    return 4;
  }

  template<class TScalarTypeInput>
  static unsigned int PixelSize(const TScalarTypeInput)
  {
    return 1;
  }

  //To be continued
};

}

#endif
