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

namespace otb
{

  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::SensorModelBase(): Superclass(OutputSpaceDimension, ParametersDimension)
  {
    m_Model = NULL;
    m_DEMHandler = DEMHandlerType::New();
    m_UseDEM = false;
    m_DEMIsLoaded = false;
    m_AverageElevation = -10000;

  }


  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::~SensorModelBase()
  {
    if( m_Model != NULL)
    {
      delete m_Model;
      m_Model = NULL;
    }
  }


  /// Get the Geometry Keyword list
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      ImageKeywordlist
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::GetImageGeometryKeywordlist(void)const
  {
    return m_ImageKeywordlist;
  }

  /// Get the Geometry Keyword list
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      ossimKeywordlist
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::GetOssimKeywordlist(void)
  {
    ossimKeywordlist geom;
    m_ImageKeywordlist.convertToOSSIMKeywordlist(geom);

    return geom;
  }

  /// Get the ossim model
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      ossimProjection*
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::GetOssimModel(void)
  {

    return m_Model;
  }

  /** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      void
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::SetImageGeometry(const ImageKeywordlist image_kwl)
  {
    m_ImageKeywordlist = image_kwl;
    CreateProjection(m_ImageKeywordlist);
  }

  /** Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      void
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::SetImageGeometry(const ossimKeywordlist & geom_kwl)
  {
    m_ImageKeywordlist.Clear();
    m_ImageKeywordlist.SetKeywordlist(geom_kwl);
    CreateProjection(m_ImageKeywordlist);
  }


  /** Instatiate the sensor model from metadata. */
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      void
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::CreateProjection(const ImageKeywordlist & image_kwl)
  {
    ossimKeywordlist geom;

    otbMsgDevMacro(<<"CreateProjection(): ossimKeywordlist: "<<geom);
    image_kwl.convertToOSSIMKeywordlist(geom);
    m_Model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
    if( m_Model == NULL)
    {
      itkExceptionMacro(<<"Invalid Model pointer m_Model == NULL !\n The ossim keywordlist is bad!");
    }
  }

  /**
   * PrintSelf method
   */
  template < class TScalarType,
  unsigned int NInputDimensions,
  unsigned int NOutputDimensions,
  unsigned int NParametersDimensions >
      void
      SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "Model: " << m_Model << std::endl;
    os << indent << "Keywordlist: " << m_ImageKeywordlist << std::endl;
  }

} // namespace otb

#endif

