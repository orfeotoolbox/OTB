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
#include <otbVectorImage.h>

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}

//
// Type definitions.
namespace mvd
{

/*******************************************************************************/

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

/**
 */
typedef itk::ImageRegion< Monteverdi2_DIMENSION > ImageRegionType;
/**
 */
typedef itk::Index< Monteverdi2_DIMENSION > IndexType;
/**
 */
typedef itk::Array< double > ParametersType;

/*******************************************************************************/

/**
 */
typedef
otb::Image< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION >
ScalarImageType;
/**
 */
typedef
otb::ImageFileReader< ScalarImageType > ScalarImageFileReaderType;

/*******************************************************************************/

/**
 */
typedef
otb::VectorImage< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION >
VectorImageType;
/**
 */
typedef otb::ImageFileReader< VectorImageType > VectorImageFileReaderType;

/*******************************************************************************/

/**
 */
typedef itk::RGBAPixel< unsigned char > RGBAPixelType;
/**
 */
typedef otb::Image< RGBAPixelType, Monteverdi2_DIMENSION > RGBAImageType;

/*******************************************************************************/

/**
 */
typedef mvd::VectorImageType DefaultImageType;
/**
 */
typedef mvd::VectorImageFileReaderType DefaultImageFileReaderType;

/*******************************************************************************/

} // end namespace 'mvd'

#endif // __mvdTypes_h
