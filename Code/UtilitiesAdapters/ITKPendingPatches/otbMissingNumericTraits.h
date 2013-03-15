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
#ifndef __otbMissingNumericTraits_h
#define __otbMissingNumericTraits_h

#include "itkMacro.h"
#undef min
#undef max

#define itkNUMERIC_TRAITS_MIN_MAX_MACRO() \
  static ValueType min() { return vcl_numeric_limits<ValueType>::min(); } \
  static ValueType max() { return vcl_numeric_limits<ValueType>::max(); } \
  static ValueType min( ValueType ) { return vcl_numeric_limits<ValueType>::min(); } \
  static ValueType max( ValueType ) { return vcl_numeric_limits<ValueType>::max(); } \


#include "vcl_limits.h" // for vcl_numeric_limits
#include "itkNumericTraits.h"
#include <complex>

namespace itk
{


/** \class NumericTraits< std::complex<char> >
 * \brief Define traits for type std::complex<char>.
 * \ingroup DataRepresentation
 * \ingroup OTBCommon
 */
template<>
class NumericTraits< std::complex< char > >
{
public:
  typedef std::complex< char >   Self;
  // for backward compatibility
  typedef Self                   TheType;
  typedef char                   ValueType;
  typedef Self                   PrintType;
  typedef double                 AbsType;
  typedef Self                   AccumulateType;
  typedef std::complex< double > RealType;
  typedef double                 ScalarRealType;
  typedef std::complex< float >  FloatType;
  //typedef itk::FixedArray<char, 2>    MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() <= 0; }
  static bool IsNegative(Self val) { return val.real() < 0; }
  static bool IsNonnegative(Self val) { return val.real() >= 0; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<unsigned char> >
 * \brief Define traits for type std::complex<unsigned char>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< unsigned char > >
{
public:
  typedef std::complex< unsigned char >   Self;
  // for backward compatibility
  typedef Self                         TheType;
  typedef unsigned char                ValueType;
  typedef Self                         PrintType;
  typedef double                       AbsType;
  typedef Self                         AccumulateType;
  typedef std::complex< double >       RealType;
  typedef double                       ScalarRealType;
  typedef std::complex< float >        FloatType;
  //typedef itk::FixedArray<unsigned char, 2> MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() == 0; }
  static bool IsNegative(Self) { return false; }
  static bool IsNonnegative(Self) { return true; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<short> >
 * \brief Define traits for type std::complex<short>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< short > >
{
public:
  typedef std::complex< short >  Self;
  // for backward compatibility
  typedef Self                   TheType;
  typedef short                  ValueType;
  typedef Self                   PrintType;
  typedef double                 AbsType;
  typedef Self                   AccumulateType;
  typedef std::complex< double > RealType;
  typedef double                 ScalarRealType;
  typedef std::complex< float >  FloatType;
  //typedef itk::FixedArray<short, 2>   MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
 static Self max(Self) { return max(); }
 static Self NonpositiveMin()
 {
   return Self(-NumericTraits< float >::NonpositiveMin(), 0);
 }
  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() <= 0; }
  static bool IsNegative(Self val) { return val.real() < 0; }
  static bool IsNonnegative(Self val) { return val.real() >= 0; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<unsigned short> >
 * \brief Define traits for type std::complex<unsigned short>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< unsigned short > >
{
public:
  typedef std::complex< unsigned short >  Self;
  // for backward compatibility
  typedef Self                            TheType;
  typedef unsigned short                  ValueType;
  typedef Self                            PrintType;
  typedef double                          AbsType;
  typedef Self                            AccumulateType;
  typedef std::complex< double >          RealType;
  typedef double                          ScalarRealType;
  typedef std::complex< float >           FloatType;
  //typedef itk::FixedArray<unsigned short, 2>   MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() == 0; }
  static bool IsNegative(Self) { return false; }
  static bool IsNonnegative(Self) { return true; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
   if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<int> >
 * \brief Define traits for type std::complex<int>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< int > >
{
public:
  typedef std::complex< int >    Self;
  // for backward compatibility
  typedef Self                   TheType;
  typedef int                    ValueType;
  typedef Self                   PrintType;
  typedef double                 AbsType;
  typedef Self                   AccumulateType;
  typedef std::complex< double > RealType;
  typedef double                 ScalarRealType;
  typedef std::complex< float >  FloatType;
  //typedef itk::FixedArray<int, 2>     MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() <= 0; }
  static bool IsNegative(Self val) { return val.real() < 0; }
  static bool IsNonnegative(Self val) { return val.real() >= 0; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<unsigned int> >
 * \brief Define traits for type std::complex<unsigned int>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< unsigned int > >
{
public:
  typedef std::complex< unsigned int >  Self;
  // for backward compatibility
  typedef Self                          TheType;
  typedef unsigned int                  ValueType;
  typedef Self                          PrintType;
  typedef double                        AbsType;
  typedef Self                          AccumulateType;
  typedef std::complex< double >        RealType;
  typedef double                        ScalarRealType;
  typedef std::complex< float >         FloatType;
  //typedef itk::FixedArray<unsigned int, 2>   MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() == 0; }
  static bool IsNegative(Self) { return false; }
  static bool IsNonnegative(Self) { return true; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<long> >
 * \brief Define traits for type std::complex<long>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< long > >
{
public:
  typedef std::complex< long >    Self;
  // for backward compatibility
  typedef Self                   TheType;
  typedef long                   ValueType;
  typedef Self                   PrintType;
  typedef double                 AbsType;
  typedef Self                   AccumulateType;
  typedef std::complex< double > RealType;
  typedef double                 ScalarRealType;
  typedef std::complex< float >  FloatType;
  //typedef itk::FixedArray<long, 2>    MeasurementVectorType;
  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() <= 0; }
  static bool IsNegative(Self val) { return val.real() < 0; }
  static bool IsNonnegative(Self val) { return val.real() >= 0; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

/** \class NumericTraits< std::complex<unsigned long> >
 * \brief Define traits for type std::complex<unsigned long>.
 * \ingroup DataRepresentation
 * \ingroup ITKCommon
 */
template< >
class NumericTraits< std::complex< unsigned long > >
{
public:
  typedef std::complex< unsigned long >  Self;
  // for backward compatibility
  typedef Self                           TheType;
  typedef unsigned long                  ValueType;
  typedef Self                           PrintType;
  typedef double                         AbsType;
  typedef Self                           AccumulateType;
  typedef std::complex< double >         RealType;
  typedef double                         ScalarRealType;
  typedef std::complex< float >          FloatType;
  //typedef itk::FixedArray<unsigned long, 2>   MeasurementVectorType;

  static const Self ITKCommon_EXPORT Zero;
  static const Self ITKCommon_EXPORT One;

  static Self min() { return vcl_numeric_limits< Self >::min(); }
  static Self max() { return vcl_numeric_limits< Self >::max(); }
  static Self min(Self) { return min(); }
  static Self max(Self) { return max(); }
  static Self NonpositiveMin()
  {
    return Self(-NumericTraits< float >::NonpositiveMin(), 0);
  }

  static bool IsPositive(Self val) { return val.real() > 0; }
  static bool IsNonpositive(Self val) { return val.real() == 0; }
  static bool IsNegative(Self) { return false; }
  static bool IsNonnegative(Self) { return true; }
  static Self ZeroValue() { return Zero; }
  static Self OneValue() { return One; }
  static unsigned int GetLength(const Self &) { return 2; }
  static unsigned int GetLength() { return 2; }
  static Self NonpositiveMin(const Self &) { return NonpositiveMin(); }
  static Self ZeroValue(const Self &) { return ZeroValue(); }
  static Self OneValue(const Self &) { return OneValue(); }
  template<class TArray>
  static void AssignToArray( const Self & v, TArray & mv )
  {
    mv[0] = v.real();
    mv[1] = v.imag();
  }
  static void SetLength(Self &, const unsigned int s)
  {
    if ( s != 2 )
      {
      itkGenericExceptionMacro(<< "Cannot set the size of a complex to " << s);
      }
  }
};

} // namespace itk

#endif // __otbMissingNumericTraits_h
