/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbReliefColormapFunctor_h
#define otbReliefColormapFunctor_h

#include "itkColormapFunction.h"

namespace otb
{

namespace Functor
{

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
 *
 * \ingroup OTBColorMap
 */
template <class TScalar, class TRGBPixel>
class ITK_EXPORT ReliefColormapFunctor : public itk::Function::ColormapFunction<TScalar, TRGBPixel>
{
public:
  typedef ReliefColormapFunctor Self;
  typedef itk::Function::ColormapFunction<TScalar, TRGBPixel> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  typedef typename Superclass::RGBPixelType RGBPixelType;
  typedef typename Superclass::ScalarType   ScalarType;
  typedef typename Superclass::RealType     RealType;

  RGBPixelType operator()(const TScalar&) const override;

protected:
  ReliefColormapFunctor(){};
  ~ReliefColormapFunctor() override
  {
  }

private:
  ReliefColormapFunctor(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace functor

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbReliefColormapFunctor.hxx"
#endif

#endif
