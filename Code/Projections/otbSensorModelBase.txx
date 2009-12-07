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
#ifndef __otbSensorModelBase_txx
#define __otbSensorModelBase_txx

#include "otbSensorModelBase.h"

#include "otbMacro.h"
#include "itkExceptionObject.h"
#include "projection/ossimSensorModelFactory.h"
#include "projection/ossimProjectionFactoryRegistry.h"
#include "base/ossimKeywordlist.h"
#include "base/ossimRefPtr.h"
#include "imaging/ossimImageHandlerRegistry.h"
#include "imaging/ossimImageHandler.h"
#include "ossim/ossimPluginProjectionFactory.h"

namespace otb
{

template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::SensorModelBase(): Superclass(OutputSpaceDimension, 0)
{
  m_Model = NULL;
  m_DEMHandler = DEMHandlerType::New();
  m_UseDEM = false;
  m_DEMIsLoaded = false;
  m_AverageElevation = 0.0;

}


template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::~SensorModelBase()
{
  if ( m_Model != NULL)
  {
    delete m_Model;
    m_Model = NULL;
  }
}


/// Get the Geometry Keyword list
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
ImageKeywordlist
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::GetImageGeometryKeywordlist(void)const
{
  return m_ImageKeywordlist;
}

/// Get the Geometry Keyword list
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
ossimKeywordlist
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::GetOssimKeywordlist(void)
{
  ossimKeywordlist geom;
  m_ImageKeywordlist.convertToOSSIMKeywordlist(geom);

  return geom;
}

/// Get the ossim model
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
ossimProjection*
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::GetOssimModel(void)
{

  return m_Model;
}

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::SetImageGeometry(const ImageKeywordlist & image_kwl)
{
  m_ImageKeywordlist = image_kwl;
  CreateProjection(m_ImageKeywordlist);
}

/** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::SetImageGeometry(const ossimKeywordlist & geom_kwl)
{
  m_ImageKeywordlist.Clear();
  m_ImageKeywordlist.SetKeywordlist(geom_kwl);
  CreateProjection(m_ImageKeywordlist);
}


/** Instatiate the sensor model from metadata. */
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::CreateProjection(const ImageKeywordlist & image_kwl)
{
  ossimKeywordlist geom;

  image_kwl.convertToOSSIMKeywordlist(geom);
  otbMsgDevMacro(<< "CreateProjection()");
  otbMsgDevMacro(<< "* type: " << geom.find("type"));

  m_Model = ossimSensorModelFactory::instance()->createProjection(geom);
  if ( m_Model == NULL)
  {
    m_Model = ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(geom);
  }
  if ( m_Model == NULL)
  {
    itkExceptionMacro(<< "Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!");
  }
}

/**
 * PrintSelf method
 */
template < class TScalarType,
unsigned int NInputDimensions,
unsigned int NOutputDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Model: " << m_Model << std::endl;
  os << indent << "Keywordlist: " << m_ImageKeywordlist << std::endl;
}

} // namespace otb

#endif

