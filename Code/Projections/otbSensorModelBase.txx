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


/**SensorModelBase.cxx
*  Purpose:
*  Cette classe permet de projeter un point au sol en un point sur un point sur l'image capteur.
*  (lat,lon) -> (i,j) ou (lat,lon,h) -> (i,j)
**/
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
{ /************************************/
  /*          Constructeurs           */
  /************************************/
  
// Constructeur par défault
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::SensorModelBase(): Superclass(OutputSpaceDimension, ParametersDimension)
{
        m_Model = NULL;
}

// Destructeur 
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

/******************************************/
/*        Déclaration des méthodes:       */
/******************************************/

/// Méthode GetGeometryKeywordlist
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
ImageKeywordlist &
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::GetImageGeometryKeywordlist(void)const
{
        return m_ImageKeywordlist;
}
/// Méthode GetGeometryKeywordlist
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

/* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::SetImageGeometry(ImageKeywordlist image_kwl)
{
        m_ImageKeywordlist = image_kwl;
        CreateProjection(m_ImageKeywordlist);
}

/* Set the Imagekeywordlist and affect the ossim projection ( m_Model) */
template < class TScalarType,
           unsigned int NInputDimensions,
           unsigned int NOutputDimensions,
           unsigned int NParametersDimensions >
void
SensorModelBase< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>
::SetImageGeometry(ossimKeywordlist & geom_kwl)
{
        m_ImageKeywordlist.Clear();
        m_ImageKeywordlist.SetKeywordlist(geom_kwl);
        CreateProjection(m_ImageKeywordlist);
}


/// Méthode SetGeometry :Créer et instancier le modèle de capteur grâce aux metadata.
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

}//fin namespace

#endif

