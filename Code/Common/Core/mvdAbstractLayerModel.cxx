/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
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

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"
#include "Core/mvdTypes.h"


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

  const char * epsg = ogr_sr.GetAuthorityCode( "PROJCS" );

  if( epsg!=NULL && strcmp( epsg, "" )!=0 )
    return SRT_CARTO;

  epsg = ogr_sr.GetAuthorityCode( "GEOGCS" );

  if( epsg!=NULL && strcmp( epsg, "" )!=0 )
    return SRT_GEO;

  return SRT_UNKNOWN;
}

/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */
/*****************************************************************************/
AbstractLayerModel
::AbstractLayerModel( QObject* parent ) :
  AbstractModel( parent ),
  VisibleInterface()
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

  OGRSpatialReference ogr_sr( wkt.c_str() );

  const char * epsg = ogr_sr.GetAuthorityCode( "PROJCS" );

  if( epsg!=NULL && strcmp( epsg, "" )!=0 )
    return epsg;

  epsg = ogr_sr.GetAuthorityCode( "GEOGCS" );

  assert( epsg!=NULL && strcmp( epsg, "" )!=0 );

  return epsg;
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
