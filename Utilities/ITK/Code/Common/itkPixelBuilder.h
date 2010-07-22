/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPixelBuilder.h,v $
  Language:  C++
  Date:      $Date: 2010-04-05 18:05:48 $
  Version:   $Revision: 1.65 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPixelBuilder_h
#define __itkPixelBuilder_h

#include "itkVariableLengthVector.h"

namespace itk
{

/** \class PixelBuilder
 *
 * \ingroup DataRepresentation
 */
template <typename T>
class PixelBuilder
{
public:
  // Default implementation
  static void Zero( T & pixel,unsigned int)
  {
    pixel = itk::NumericTraits<T>::Zero;
  }

  static void RefZero( T & pixel,const T &)
  {
    pixel = itk::NumericTraits<T>::Zero;
  }
};

// Partial specialisation for VariableLengthVector
template <typename T>
class PixelBuilder< VariableLengthVector<T> >
{
public:
  static void Zero(VariableLengthVector<T>& pixel, unsigned int nbComp)
  {
    pixel.SetSize(nbComp,true);
    pixel.Fill(itk::NumericTraits<T>::Zero);
  }

  static void RefZero(VariableLengthVector<T>& pixel, const VariableLengthVector<T>& refPixel )
  {
    pixel.SetSize(refPixel.Size(),true);
    pixel.Fill(itk::NumericTraits<T>::Zero);
  }
};
} // End namespace itk

#endif // __itkPixelBuilder_h
