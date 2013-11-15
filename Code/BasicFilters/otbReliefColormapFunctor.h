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
#ifndef __otbReliefColormapFunctor_h
#define __otbReliefColormapFunctor_h

#include "itkColormapFunction.h"

namespace otb {

namespace Functor {

/**
 * \class ReliefColormapFunctor
 * \brief Function object which maps a scalar value into an RGB colormap value for relief representation.
 *
 * This code is working with the framework contributed in the Insight Journal paper:
 *
 * "Meeting Andy Warhol Somewhere Over the Rainbow: RGB Colormapping and ITK"
 * http://www.insight-journal.org/browse/publication/285
 * http://hdl.handle.net/1926/1452
 *
 */
template<class TScalar, class TRGBPixel>
class ITK_EXPORT ReliefColormapFunctor
  : public itk::Function::ColormapFunction<TScalar, TRGBPixel>
{
public:

  typedef ReliefColormapFunctor                             Self;
  typedef itk::Function::ColormapFunction<TScalar, TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::RGBPixelType RGBPixelType;
  typedef typename Superclass::ScalarType   ScalarType;
  typedef typename Superclass::RealType     RealType;

  virtual RGBPixelType operator ()(const TScalar&) const;

protected:
  ReliefColormapFunctor(){};
  ~ReliefColormapFunctor() {}

private:
  ReliefColormapFunctor(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end namespace functor

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReliefColormapFunctor.txx"
#endif

#endif
