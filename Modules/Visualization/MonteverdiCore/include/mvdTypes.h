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


#ifndef mvdTypes_h
#define mvdTypes_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

#include "OTBMonteverdiCoreExport.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtWidgets>

//
// System includes (sorted by alphabetic order)
#include <exception>
#include <string>

//
// ITK includes (sorted by alphabetic order)
#include <itkArray.h>
#include <itkImageRegion.h>

//
// OTB includes (sorted by alphabetic order)
#include <otbCast.h>
#include <otbImage.h>
#include <otbImageFileReader.h>
#include <otbImageFileWriter.h>
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
/* Enumerations                                                                */

/*******************************************************************************/
/* Type definitions of scalar values.                                          */

/**
 */
typedef unsigned int CountType;

/**
 */
typedef Monteverdi_FLOATING_TYPE RealType;

/**
 */
typedef RealType ComponentType;

/**
 */
typedef itk::VariableLengthVector< RealType > VectorPixelType;

/*******************************************************************************/
/* Type definitions of STL realization.                                        */

/**
 */
typedef std::vector< std::string > StringVector;

/**
 */
typedef std::vector< int > IntVector;

/**
 */
typedef std::vector< unsigned int > UIntVector;

/**
 */
typedef std::list< std::string > StringList;

/**
 */
typedef std::vector< double > DoubleVector;

/*******************************************************************************/
/* Type definitions of ITK realization.                                        */

/**
 */
typedef itk::Array< double > ParametersType;

/*******************************************************************************/
/* Type definitions depending on dimension.                                    */

/**
 */
typedef itk::ImageRegion< Monteverdi_DIMENSION > ImageRegionType;

/**
 */
typedef itk::Index< Monteverdi_DIMENSION > IndexType;

/**
 */
typedef itk::Size< Monteverdi_DIMENSION > SizeType;

/**
 */
typedef itk::ImageBase< Monteverdi_DIMENSION > ImageBaseType;

/*******************************************************************************/
/* Type definitions for scalar/mono-band images.                               */

/**
 */
typedef
otb::Image< ComponentType, Monteverdi_DIMENSION >
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
otb::VectorImage< ComponentType, Monteverdi_DIMENSION >
VectorImageType;

/**
 */
typedef otb::ImageFileReader< VectorImageType > VectorImageFileReaderType;

/**
 */
typedef otb::ImageFileWriter< VectorImageType > VectorImageFileWriterType;

/**
 */
typedef VectorImageType::SpacingType SpacingType;

/**
 */
typedef VectorImageType::SizeType SizeType;

/**
 */
typedef VectorImageType::PointType PointType;

/*******************************************************************************/
/* Type definitions for application defaults.                                  */

/**
 */
typedef mvd::VectorImageType DefaultImageType;

/**
 */
typedef mvd::VectorImageFileReaderType DefaultImageFileReaderType;

/**
 */
struct PixelInfo
{
  typedef std::vector< PixelInfo > Vector;

  PixelInfo() :
    m_Key(),
    m_Point(),
    m_Index(),
    m_Pixel(),
    m_Resolution (1),
    m_HasPoint( false ),
    m_HasIndex( false ),
    m_HasPixel( false ),
    m_HasResolution( false )
  {
  }

  std::string m_Key;

  DefaultImageType::PointType m_Point;
  DefaultImageType::IndexType m_Index;
  DefaultImageType::PixelType m_Pixel;
  unsigned int m_Resolution;

  bool m_HasPoint: 1;
  bool m_HasIndex: 1;
  bool m_HasPixel: 1;
  bool m_HasResolution: 1;
};

/*******************************************************************************/
/* Type definitions for wrapped applications                                   */

/**
 */
typedef std::map< std::string, std::string > ApplicationDocNameToNameMap;

/**
 */
typedef std::map< std::string, StringVector > ApplicationsTagContainer;

/*******************************************************************************/
/* Type definitions for dataset properties                                     */
/**
 */
typedef qlonglong SqlId;
/**
 */
// typedef SqlId DatasetId;
typedef QString DatasetHash;
/**
 */
typedef std::pair< std::string, std::string > PropertyType;

/**
 */
typedef std::vector< PropertyType > PropertiesVector;

/**
 */
typedef
std::pair< std::string, std::vector< PropertyType > > PropertiesVectorByCategory;

/**
 */
typedef std::map< std::string, PropertiesVector > PropertiesContainer;

/*******************************************************************************/
/* Type definitions for Dataset list                                           */

/**
 */
typedef QPair< QString, QString > StringPairType;

/**
 */
typedef QList< StringPairType > StringPairListType;

/*******************************************************************************/
template< typename T >
inline
T
QObjectCast( QObject * object, const QString & string = QString() )
{
  T model = qobject_cast< T >( object );

  if( model!=object )
    {
    if( !string.isNull() )
      qWarning() << string;

    throw std::bad_cast();
    }

  return model;
}

/*******************************************************************************/
} // end namespace 'mvd'

#endif // mvdTypes_h
