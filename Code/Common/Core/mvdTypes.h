/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdTypes_h
#define __mvdTypes_h

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
#include <itkArray.h>
#include <itkImageRegion.h>
#include <itkRGBAPixel.h>

//
// OTB includes (sorted by alphabetic order)
#include <otbImage.h>
#include <otbImageFileReader.h>
#include <otbImageFileWriter.h>
#include <otbVectorImage.h>

// TODO: Contribute to OTB!
namespace otb
{
/**
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< T2 >
DynamicCast( const itk::SmartPointer< T1 >& p1 )
{
  return
    typename itk::SmartPointer< T2 >(
      dynamic_cast< typename itk::SmartPointer< T2 >::ObjectType* >(
	p1.GetPointer()
      )
    );
}

/**
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< const T2 >
ConstCast( const itk::SmartPointer< T1 >& p1 )
{
  return
    typename itk::SmartPointer< const T2 >(
      const_cast< typename itk::SmartPointer< const T2 >::ObjectType* >(
	p1.GetPointer()
      )
    );
}

/**
 */
template< typename T2, typename T1 >
inline
itk::SmartPointer< T2 >
ConstCast( const itk::SmartPointer< const T1 >& p1 )
{
  return
    typename itk::SmartPointer< T2 >(
      const_cast< typename itk::SmartPointer< T2 >::ObjectType* >(
	p1.GetPointer()
      )
    );
}

} // end of namespace otb.

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}
/*******************************************************************************/
/* PROTOTYPE                                                                   */
/*******************************************************************************/
namespace mvd
{

namespace core
{

template< typename TValueType, unsigned int VLength >
class FixedArray :
    public itk::FixedArray< TValueType, VLength >
{
public:
  /** Standard Self typedef */
  typedef mvd::core::FixedArray< TValueType, VLength > Self;
  typedef itk::FixedArray< TValueType, VLength > Superclass;

  typedef itk::SmartPointer< Self > Pointer;
  typedef itk::SmartPointer< const Self > ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Runtime information support. */
  itkTypeMacro( mvd::FixedArray, itk::FixedArray );


public:
  FixedArray();
  void SetSize( unsigned int sz, bool destroyExistingData=true );
  void Fill( const Self& );
  void Fill( const TValueType& );
};

} // end of namespace 'core'.

} // end of namespace 'mvd'.

/*******************************************************************************/
/*
namespace itk
{
//
// Instantiate the macros to declare the NumericTraits for the
// mvd::FixedArray types.
//
#ifdef ITK_USE_NUMERIC_TRAITS_PARTIAL_SPECIALIZATION
itkNumericTraitsGenericArrayScalarsDimensionsMacro( mvd::FixedArray );
#else // ITK_USE_NUMERIC_TRAITS_PARTIAL_SPECIALIZATION
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, char );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, unsigned char );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, signed char );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, short );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, unsigned short );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, int );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, unsigned int );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, long );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, unsigned long );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, float );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, double );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, long double );
#ifdef ITK_TYPE_USE_LONG_LONG
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, long long );
itkNumericTraitsGenericArrayDimensionsMacro( mvd::FixedArray, unsigned long long );
#endif // ITK_TYPE_USE_LONG_LONG
#endif // ITK_USE_NUMERIC_TRAITS_PARTIAL_SPECIALIZATION
}
*/
/*******************************************************************************/
/* PROTOTYPE                                                                   */
/*******************************************************************************/

//
// Type definitions.
namespace mvd
{

namespace core
{

/*******************************************************************************/
/* Enumerations                                                                */

/**
 */
enum Bound
{
  BOUND_LOWER = 0,
  BOUND_UPPER,
  BOUND_COUNT
};

/**
 * Constant naming bound values.
 */
extern
const char*
BOUND_NAMES[ BOUND_COUNT ];

/*******************************************************************************/
/* Type definitions of scalar values.                                          */
/**
 */
typedef unsigned int CountType;

/*******************************************************************************/
/* Type definitions of STL realization.                                        */
/**
 */
typedef std::vector< std::string > StringVector;
/**
 */
typedef std::vector< unsigned int > UIntVector;
/**
 */
typedef std::list< std::string > StringList;

/*******************************************************************************/
/* Type definitions of ITK realization.                                        */
/**
 */
typedef itk::Array< double > ParametersType;

/*******************************************************************************/
/* Type definitions depending on dimension.                                    */
/**
 */
typedef itk::ImageRegion< Monteverdi2_DIMENSION > ImageRegionType;
/**
 */
typedef itk::Index< Monteverdi2_DIMENSION > IndexType;
/**
 */
typedef itk::Size< Monteverdi2_DIMENSION > SizeType;
/**
 */
typedef itk::ImageBase< Monteverdi2_DIMENSION > ImageBaseType;

/*******************************************************************************/
/* Type definitions for scalar/mono-band images.                               */
/**
 */
typedef
otb::Image< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION >
ScalarImageType;
/**
 */
typedef
otb::ImageFileReader< ScalarImageType > ScalarImageFileReaderType;
/**
 */
typedef
otb::ImageFileWriter< ScalarImageType > ScalarImageFileWriterType;

/*******************************************************************************/
/* Type definitions for vector/multi-band images (generic internal data).      */
/**
 */
typedef
otb::VectorImage< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION >
VectorImageType;
/**
 */
typedef otb::ImageFileReader< VectorImageType > VectorImageFileReaderType;
/**
 */
typedef otb::ImageFileWriter< VectorImageType > VectorImageFileWriterType;
/**
 */
typedef VectorImageType::SpacingType  SpacingType;
/**
 */
typedef VectorImageType::PointType    PointType;

/*******************************************************************************/
/* Type definitions for RGBA images (display).                                 */
/**
 */
typedef itk::RGBAPixel< unsigned char > RGBAPixelType;
/**
 */
typedef otb::Image< RGBAPixelType, Monteverdi2_DIMENSION > RGBAImageType;

/*******************************************************************************/
/* Type definitions for application defaults.                                  */
/**
 */
typedef VectorImageType DefaultImageType;
/**
 */
typedef VectorImageFileReaderType DefaultImageFileReaderType;

/*******************************************************************************/
/** Type definitions for wrapped applications
 * 
 */
typedef std::map< std::string, StringVector >    ApplicationsTagContainer;

/*******************************************************************************/

} // end namespace 'core'

} // end namespace 'mvd'

#endif // __mvdTypes_h
