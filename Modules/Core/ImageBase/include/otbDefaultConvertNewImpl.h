/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef otbDefaultConvertNewImp_h
#define otbDefaultConvertNewImp_h

#include "itkDefaultConvertPixelTraits.h"

// #include "itkOffset.h"
// #include "itkVector.h"
// #include "itkMatrix.h"
// #include "itkVariableLengthVector.h"
// #include "itkVariableSizeMatrix.h"

namespace otb 
{


template < typename PixelType>
class DefaultConvertPixelTraitsTest 
: public itk::DefaultConvertPixelTraits < PixelType >
{
public:
  typedef itk::DefaultConvertPixelTraits < PixelType > SuperClass;
  using SuperClass::ComponentType;

  using SuperClass::SetNthComponent;

  static void SetNthComponent(int , PixelType & pixel, const PixelType & v)
    {
      pixel = v;
    }  
};

// We might not need this specialization if the compileur allow
// us to declare to function with same signature in some case :
// see "SetNthComponent"
#define OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(type)                      \
  template< >                                                              \
  class DefaultConvertPixelTraitsTest < type >                                 \
  : public itk::DefaultConvertPixelTraits < type >                         \
  {                                                                        \
public:                                                                    \
  typedef itk::DefaultConvertPixelTraits < type > SuperClass;              \
    using typename SuperClass::ComponentType;                              \
  };

OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(float)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(double)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(long double)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(int)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(short)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned int)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(signed char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned short)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(long long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned long long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(bool)

#undef OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL

//
//  Default traits for the Offset<> pixel type
//

// template<unsigned int VDimension>
// class DefaultConvertPixelTraitsTest < itk::Offset< VDimension > > : 
// public itk::DefaultConvertPixelTraits< itk::Offset< VDimension > >
// {
// public:
//   typedef itk::DefaultConvertPixelTraits < itk::Offset< VDimension > > SuperClass;
//   using typename SuperClass::ComponentType;

//   using SuperClass::SetNthComponent;

//   static void SetNthComponent(int , TargetType & pixel, const TargetType& v)
//     {
//       pixel = v;
//     }
// };

//
//  Default traits for the pixel types deriving from FixedArray<>
//

/*
define OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(type)                                        \
template < typename TComponentType , unsigned VDimension >                                   \
class DefaultConvertPixelTraitsTest < type < TComponentType , VDimension > >                 \
: public itk::DefaultConvertPixelTraits < type < TComponentType , VDimension > >             \
{                                                                                            \
public:                                                                                      \
  typedef itk::DefaultConvertPixelTraits < type < TComponentType , VDimension > > SuperClass;\
  typedef typename SuperClass::TargetType TargetType;                                        \
  static void SetNthComponent(int , TargetType & pixel, const TargetType & v)                \
    {                                                                                        \
      pixel = v;                                                                             \
    }                                                                                        \
}                                                                                            \
*/
// OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::FixedArray);
// OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::Vector);
// OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::CovariantVector);
// OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::Point);

// #undef OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE

//
//  Default traits for pixel types deriving from VariableLengthVector<>
//

// template < typename T >
// class DefaultConvertPixelTraitsTest < itk::VariableLengthVector < T > >
// : public itk::DefaultConvertPixelTraits < itk::VariableLengthVector< T > >
// {
// public:
//   typedef itk::DefaultConvertPixelTraits< itk::VariableLengthVector < T > > SuperClass;
//   using typename SuperClass::TargetType;
//   using typename SuperClass::ComponentType;
// };

//
//  Default traits for the pixel types deriving from Matrix<>
//

// template<typename VComponent, unsigned VRows, unsigned VCols >
// class DefaultConvertPixelTraitsTest < itk::Matrix< VComponent, VRows, VCols > >
// : public itk::DefaultConvertPixelTraits < itk::Matrix< VComponent, VRows, VCols > >
// {
// public:
//   typedef itk::DefaultConvertPixelTraits < itk::Matrix< VComponent, VRows, VCols > > SuperClass;
//   using typename SuperClass::TargetType;
//   using typename SuperClass::ComponentType;


//   using SuperClass::SetNthComponent;

//   static void SetNthComponent(int , TargetType & pixel, const TargetType& v)
//     {
//       pixel = v;
//     }
// };

//
//  Default traits for pixel types deriving from VariableSizeMatrix<>
//

// template < typename T >
// class DefaultConvertPixelTraitsTest< itk::VariableSizeMatrix< T > >
// : public itk::DefaultConvertPixelTraits< itk::VariableSizeMatrix< T > >
// {
//   public:
//   typedef itk::DefaultConvertPixelTraits< itk::VariableSizeMatrix < T > > SuperClass;
//   using typename SuperClass::TargetType;
//   using typename SuperClass::ComponentType;
// };

//
//  Default traits for pixel types deriving from std::complex<>
//

template < typename T >
class DefaultConvertPixelTraitsTest < ::std::complex < T > > 
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
