/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbDefaultConvertPixelTraits_h
#define otbDefaultConvertPixelTraits_h

#include "itkDefaultConvertPixelTraits.h"
#include "OTBImageBaseExport.h"

namespace otb 
{


template < typename PixelType>
class OTBImageBase_EXPORT_TEMPLATE DefaultConvertPixelTraits 
: public itk::DefaultConvertPixelTraits < PixelType >
{
public:
  typedef itk::DefaultConvertPixelTraits < PixelType > SuperClass;
  using typename SuperClass::ComponentType;

  using SuperClass::SetNthComponent;

  static void SetNthComponent(int , PixelType & pixel, const PixelType & v)
    {
      pixel = v;
    }  
};

template < typename T >
class DefaultConvertPixelTraits < ::std::complex < T > > 
: public itk::DefaultConvertPixelTraits < ::std::complex < T > >
{
public:

  typedef itk::DefaultConvertPixelTraits < ::std::complex < T > > SuperClass;
  using typename SuperClass::TargetType ;
  using typename SuperClass::ComponentType ;

  using SuperClass::SetNthComponent ;

  static void SetNthComponent(int , TargetType & pixel, const TargetType & v)
    {
      pixel = v;
    }

  static TargetType GetNthComponent ( int , const TargetType & pixel )
    {
    return pixel;
    }

  static ComponentType GetScalarValue(const TargetType& pixel)
    {
       /*
       * This seems to be dead code, since the complex to scalar
       * conversion is done by ConvertPixelBuffer
       *
       * Historically, it was returning std::norm, which causes
       * compilation error on MacOSX 10.9.
       * Now returns the equivalent implementation of std::norm.
       */
      return static_cast<ComponentType>( pixel.real()*pixel.real()
                                         + pixel.imag()*pixel.imag() );
    }
};

} // end namespace
#endif
