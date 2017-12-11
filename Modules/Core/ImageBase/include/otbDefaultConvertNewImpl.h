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
#include "itkRGBPixel.h"

// #include "itkOffset.h"
// #include "itkVector.h"
// #include "itkMatrix.h"
// #include "itkVariableLengthVector.h"
// #include "itkVariableSizeMatrix.h"

namespace otb 
{
template < typename PixelType>
class DefaultConvertPixelTraitsTest : public itk::DefaultConvertPixelTraits < PixelType >
{
public:
  typedef PixelType TargetType;
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v)
    {
      pixel = v;
    }  
};

template < typename T >
class DefaultConvertPixelTraitsTest < std::complex < T > > : public itk::DefaultConvertPixelTraits < std::complex < T > >
{
public:
  typedef itk::DefaultConvertPixelTraits < std::complex < T > > SuperClass;
  typedef typename SuperClass::TargetType TargetType;
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v)
    {
      pixel = v;
    }  
  static TargetType GetNthComponent ( int , const TargetType & pixel )
    {
    return pixel;
    }
};

// template <>                                   
// class DefaultConvertPixelTraitsTest < itk::RGBPixel < short int > >                 
// : public itk::DefaultConvertPixelTraits < itk::FixedArray < short int > >             
// {                                                                                            
// public:                                                                                      
//   typedef itk::DefaultConvertPixelTraits < itk::RGBPixel < short int > > SuperClass;
//   typedef itk::RGBPixel < short int > TargetType;                                        
//   static void SetNthComponent(int , TargetType & pixel, const TargetType & v)                
//     {                                                                                        
//       pixel = v;                                                                             
//     }                                                                                        
// };

#define OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(type)                                       \
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

OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::Vector);
OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::CovariantVector);
OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::Point);
OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(itk::FixedArray);

} // end namespace
#endif
