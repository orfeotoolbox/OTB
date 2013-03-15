/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDefaultConvertPixelTraits.h,v $
  Language:  C++
  Date:      $Date: 2008-01-21 18:30:58 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbDefaultConvertPixelTraits_h
#define __otbDefaultConvertPixelTraits_h

#include "itkOffset.h"
#include "itkCovariantVector.h"
#include "itkVector.h"
#include "itkPoint.h"

namespace otb
{
  
/** \class DefaultConvertPixelTraits
 *  \brief Traits class used to by ConvertPixels to convert blocks of pixels.
 *
 *  TOutputPixelType is the destination type. The input type is inferred
 *  by the templated static function Convert.
 *
 *  This implementation does a simple assignment operator, so if you are
 *  going from a higher bit representation to a lower bit one (int to
 *  char), you may want to specialize and add some sort of transfer function.
 */
template<typename PixelType>
class DefaultConvertPixelTraits
{
public:
  /** Determine the pixel data type. */
  typedef typename PixelType::ComponentType ComponentType;

  /** Return the number of components per pixel. */
  static unsigned int GetNumberOfComponents() 
  { return PixelType::GetNumberOfComponents();}
  
  /** Return the nth component of the pixel. */
  static ComponentType GetNthComponent(int c, const PixelType& pixel) 
  { return pixel.GetNthComponent(c); }

  /** Set the nth component of the pixel. */
  static void SetNthComponent(int c, PixelType& pixel, const ComponentType& v) 
  { pixel.SetNthComponent(c, v); }
  static void SetNthComponent(int c, PixelType & pixel, const PixelType& v)
  { pixel = v; }

  /** Return a single scalar value from this pixel. */
  static ComponentType GetScalarValue(const PixelType& pixel)
  { return pixel.GetScalarValue(); }

};

#define OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(type)                    \
template<>                                                               \
class DefaultConvertPixelTraits<type>                                    \
{                                                                        \
public:                                                                  \
  typedef type ComponentType;                                            \
  static unsigned int GetNumberOfComponents()                            \
    {                                                                    \
      return 1;                                                          \
    }                                                                    \
  static void SetNthComponent(int , type& pixel, const ComponentType& v) \
    {                                                                    \
      pixel = v;                                                         \
    }                                                                    \
  static type GetScalarValue(const type& pixel)                          \
    {                                                                    \
      return pixel;                                                      \
    }                                                                    \
};

OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(float)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(double)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(int)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(short)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned int)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(signed char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned char)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned short)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(unsigned long)
OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL(bool)

#undef OTB_DEFAULTCONVERTTRAITS_NATIVE_SPECIAL

//
//  Default traits for the Offset<> pixel type
//

#define OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(dimension)                  \
template<>                                                               \
class DefaultConvertPixelTraits< Offset<dimension> >                     \
{                                                                        \
public:                                                                  \
  typedef Offset<dimension>  TargetType;                                 \
  typedef TargetType::OffsetValueType  ComponentType;                    \
  static unsigned int GetNumberOfComponents()                            \
    {                                                                    \
      return dimension;                                                  \
    }                                                                    \
  static void SetNthComponent(int i, TargetType & pixel, const ComponentType& v)   \
    {                                                                    \
      pixel[i] = v;                                                      \
    }                                                                    \
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v)   \
    {                                                                    \
      pixel = v;                                                      \
    }                                                                    \
  static ComponentType GetScalarValue(const TargetType& pixel)           \
    {                                                                    \
      return pixel[0];                                                   \
    }                                                                    \
};                                                                       \


// Define traits for Offset<> from dimensions 1 to 5
  OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(1)
  OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(2)
  OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(3)
  OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(4)
  OTB_DEFAULTCONVERTTRAITS_OFFSET_TYPE(5)

//
//  Default traits for the pixel types deriving from FixedArray<>
//

#define OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(type,componenttype, dimension) \
template<>                                                               \
class DefaultConvertPixelTraits< type< componenttype, dimension> >       \
{                                                                        \
public:                                                                  \
  typedef type< componenttype, dimension >  TargetType;                  \
  typedef componenttype                     ComponentType;               \
  static unsigned int GetNumberOfComponents()                            \
    {                                                                    \
      return dimension;                                                  \
    }                                                                    \
  static void SetNthComponent(int i, TargetType & pixel, const ComponentType& v)   \
    {                                                                    \
      pixel[i] = v;                                                      \
    }                                                                    \
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v)   \
    {                                                                    \
      pixel = v;                                                      \
    }                                                                    \
  static ComponentType GetScalarValue(const TargetType& pixel)           \
    {                                                                    \
      return pixel[0];                                                   \
    }                                                                    \
};                                                                       \

//
//
// Define traits for Classed deriving from FixedArray from dimensions 1 to 6
// These classes include: Vector, CovariantVector and Point.
//
//
#define OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, Type) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,1) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,2) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,3) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,4) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,5) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE(ArrayType,Type,6)

#define OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_TYPES_MACRO(ArrayType) \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, char); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, signed char); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, unsigned char); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, short); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, unsigned short); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, int); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, unsigned int); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, long); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, unsigned long); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, float); \
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_MACRO(ArrayType, double);

  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_TYPES_MACRO(Vector);
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_TYPES_MACRO(CovariantVector);
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_TYPES_MACRO(Point);
  OTB_DEFAULTCONVERTTRAITS_FIXEDARRAY_TYPE_ALL_TYPES_MACRO(FixedArray);

//
//  End of Traits for the classes deriving from FixedArray.
//
//


//
//  Default traits for the pixel types deriving from Matrix<>
//

#define OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(type,componenttype,rows,cols) \
template<>                                                               \
class DefaultConvertPixelTraits< type< componenttype, rows, cols > >     \
{                                                                        \
public:                                                                  \
  typedef type< componenttype, rows, cols >  TargetType;                 \
  typedef componenttype                     ComponentType;               \
  static unsigned int GetNumberOfComponents()                            \
    {                                                                    \
      return rows * cols;                                                \
    }                                                                    \
  static void SetNthComponent(int i, TargetType & pixel, const ComponentType& v)   \
    {                                                                    \
      const unsigned int row = i / cols;                                 \
      const unsigned int col = i % cols;                                 \
      pixel[row][col] = v;                                               \
    }                                                                    \
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v)   \
    {                                                                    \
      pixel = v;                                               \
    }                                                                    \
  static ComponentType GetScalarValue(const TargetType& pixel)           \
    {                                                                    \
      return pixel[0][0];                                                \
    }                                                                    \
};                                                                       \

//
//
// Define traits for Classed deriving from Matrix from dimensions 1 to 6
//
//
#define OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, Type) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,1,1) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,2,2) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,3,3) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,4,4) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,5,5) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE(ArrayType,Type,6,6)

#define OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_TYPES_MACRO(ArrayType) \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, char); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, signed char); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, unsigned char); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, short); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, unsigned short); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, int); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, unsigned int); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, long); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, unsigned long); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, float); \
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_MACRO(ArrayType, double);

//
// Add here other classes that derive from Matrix or that have the same API
//
  OTB_DEFAULTCONVERTTRAITS_MATRIX_TYPE_ALL_TYPES_MACRO(Matrix);

//
//  End of Traits for the classes deriving from Matrix.
//
//


//
//  Default traits for the pixel types deriving from std::complex<>
//

#define OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE( componenttype ) \
template<>                                                               \
class DefaultConvertPixelTraits< ::std::complex< componenttype > >       \
{                                                                        \
public:                                                                  \
  typedef ::std::complex< componenttype>  TargetType;                    \
  typedef componenttype                     ComponentType;               \
  static unsigned int GetNumberOfComponents()                            \
    {                                                                    \
    return 2;                                                  \
    }                                                                    \
  static void SetNthComponent(int i, TargetType & pixel, const ComponentType& v)   \
    {                                                                    \
    if( i == 0 )                                                            \
      {                                                                  \
      pixel = TargetType( v, pixel.imag() );                           \
      }                                                                  \
    else                                                                 \
      {                                                                  \
      pixel = TargetType( pixel.real(), v );                           \
      }                                                                  \
    }                                                                    \
  static void SetNthComponent(int , TargetType & pixel, const TargetType& v) \
    { \
      pixel = v; \
    } \
  static ComponentType GetScalarValue(const TargetType& pixel)           \
    {                                                                    \
    return std::norm(pixel);                                             \
    }                                                                    \
};                                                                       \

OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(float);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(double);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(signed int);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(unsigned int);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(short int);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(signed char);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(unsigned char);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(signed long);
OTB_DEFAULTCONVERTTRAITS_COMPLEX_TYPE(unsigned long);

//
//  End of Traits for the classes deriving from std::complex.
//
//
  
} // end namespace otb
#endif
