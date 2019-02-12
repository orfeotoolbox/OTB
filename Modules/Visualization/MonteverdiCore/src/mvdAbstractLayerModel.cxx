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

#include "mvdAbstractLayerModel.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)
#include "ogr_spatialref.h"

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include "otbGeoInformationConversion.h"
//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAlgorithm.h"
#include "mvdTypes.h"


namespace mvd
{
/*
  TRANSLATOR mvd::AbstractLayerModel

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/


/*****************************************************************************/
/* CONSTANTS                                                                 */

namespace
{
char const * const STR_SENSOR = QT_TRANSLATE_NOOP( "mvd::AbstractLayerModel", "Sensor" );
char const * const STR_UNKNOWN = QT_TRANSLATE_NOOP( "mvd::AbstractLayerModel", "Unknown" );
char const * const STR_NOEPSG = QT_TRANSLATE_NOOP( "mvd::AbstractLayerModel", "No EPSG" );
} // end of anonymous namespace.


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */
/*****************************************************************************/
SpatialReferenceType
GetSpatialReferenceType( const std::string & filename )
{
  DefaultImageFileReaderType::Pointer reader( DefaultImageFileReaderType::New() );
  assert( !reader.IsNull() );

  reader->SetFileName( filename );
  reader->UpdateOutputInformation();

  DefaultImageType * image = reader->GetOutput();
  assert( image!=NULL );

  return GetSpatialReferenceType(
    image->GetProjectionRef(),
    image->GetImageKeywordlist().GetSize()>0
  );
}

/*****************************************************************************/
SpatialReferenceType
GetSpatialReferenceType( const std::string & wkt, bool hasKwl )
{
  if( wkt.empty() )
    return
      hasKwl
      ? SRT_SENSOR
      : SRT_UNKNOWN;

  OGRSpatialReference ogr_sr( wkt.c_str() );

  if(ogr_sr.IsGeographic())
    return SRT_GEO;

  if(ogr_sr.IsProjected())
    return SRT_CARTO;

  return SRT_UNKNOWN;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
AbstractLayerModel
::AbstractLayerModel( QObject* p ) :
  AbstractModel( p ),
  VisibleInterface(),
  m_Name(QString())
{
}

/*******************************************************************************/
AbstractLayerModel
::~AbstractLayerModel()
{
}

/*******************************************************************************/
SpatialReferenceType
AbstractLayerModel
::GetSpatialReferenceType() const
{
  return mvd::GetSpatialReferenceType( GetWkt(), HasKwl() );
}

/*******************************************************************************/
std::string
AbstractLayerModel
::GetWkt() const
{
  return virtual_GetWkt();
}

/*******************************************************************************/
std::string
AbstractLayerModel
::GetAuthorityCode( bool isEnhanced ) const
{
  std::string wkt( GetWkt() );

  if( wkt.empty() )
    return
      !isEnhanced
      ? std::string()
      : ( HasKwl()
	  ? ToStdString( tr( STR_SENSOR ) )
	  : ToStdString( tr( STR_UNKNOWN ) ) );

  int code = otb::GeoInformationConversion::ToEPSG(wkt);
  if(code < 0)
    return ToStdString( tr( STR_UNKNOWN ) );
  else if(code == 0)
    return ToStdString( tr( STR_NOEPSG ) );

  return boost::lexical_cast<std::string>(code);
}

/*******************************************************************************/
bool
AbstractLayerModel
::HasKwl() const
{
  return virtual_HasKwl();
}

/*****************************************************************************/
void
AbstractLayerModel
::ToWgs84( const PointType & p, PointType & wgs84, double & alt) const
{
  virtual_ToWgs84( p, wgs84, alt );
}

/*****************************************************************************/
void
AbstractLayerModel
::SetName(const QString & name)
{
  if (name != m_Name)
    {
    m_Name = name;
    emit NameChanged();
    }
}

/*****************************************************************************/
const QString &
AbstractLayerModel
::GetName() const
{
  return m_Name;
}

/*******************************************************************************/
bool
AbstractLayerModel
::virtual_HasKwl() const
{
  return false;
}

/*******************************************************************************/
void
AbstractLayerModel
::virtual_SignalVisibilityChanged( bool isVisible )
{
  emit VisibilityChanged( isVisible );
  emit VisibilityChanged( this, isVisible );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

} // end namespace 'mvd'
