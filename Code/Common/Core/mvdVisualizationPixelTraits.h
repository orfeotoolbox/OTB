/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbVisualizationPixelTraits_h
#define __otbVisualizationPixelTraits_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "itkVariableLengthVector.h"

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)


namespace mvd
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
