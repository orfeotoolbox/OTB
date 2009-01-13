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
#ifndef __otbGenericMapProjection_txx
#define __otbGenericMapProjection_txx

#include "otbGenericMapProjection.h"
#include "otbMacro.h"
#include "projection/ossimMapProjectionFactory.h"
#include "projection/ossimMapProjection.h"

namespace otb
{

  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::GenericMapProjection() : Superclass(SpaceDimension,ParametersDimension)
  {
    m_MapProjection = NULL;
    m_ProjectionRefWkt.clear();
    reinstanciateProjection=true;
  }




  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
  GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::~GenericMapProjection()
  {
    if (m_MapProjection != NULL)
    {
      delete m_MapProjection;
    }
  }




  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      typename GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>::OssimMapProjectionType*
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::GetMapProjection ()
  {
    itkDebugMacro("returning MapProjection address " << this->m_MapProjection );
    if ((reinstanciateProjection) || (m_MapProjection == NULL))
    {
      this->InstanciateProjection();
    }

    return this->m_MapProjection;
  }


  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
     const typename GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>::OssimMapProjectionType*
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::GetMapProjection () const
  {
    itkDebugMacro("returning MapProjection address " << this->m_MapProjection );
    if ((reinstanciateProjection) || (m_MapProjection == NULL))
    {
      itkExceptionMacro(<<"m_MapProjection not up-to-date, call InstanciateProjection() first");
    }

    return this->m_MapProjection;
  }



  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      std::string
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::GetWkt()
  {
    ossimKeywordlist kwl;
    this->GetMapProjection()->saveState(kwl);
    ossimOgcWktTranslator wktTranslator;
    std::string wkt;
    wkt = wktTranslator.fromOssimKwl(kwl);
    return wkt;
  }

  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      void
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::SetWkt(std::string projectionRefWkt)
  {
    this->m_ProjectionRefWkt=projectionRefWkt;
    reinstanciateProjection = true;
    this->InstanciateProjection();
    this->Modified();
  }

  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      bool
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::InstanciateProjection()
  {
    if ((this->reinstanciateProjection) || (m_MapProjection == NULL))
    {
      ossimKeywordlist kwl;
      ossimOgcWktTranslator wktTranslator;

      bool projectionInformationAvailable = wktTranslator.toOssimKwl(m_ProjectionRefWkt, kwl);

      if (!projectionInformationAvailable)
      {
        itkExceptionMacro(<<"Impossible to create the projection from string: "<< m_ProjectionRefWkt);
      }

      m_MapProjection = ossimMapProjectionFactory::instance()->createProjection(kwl);
      this->reinstanciateProjection = false;
      return true;
    }
    return false;
  }


  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      typename GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformPoint(const InputPointType & point) const
  {
    OutputPointType outputPoint;

//     otbMsgDevMacro(<< "DirectionOfMapping: " << DirectionOfMapping);
    switch(DirectionOfMapping)
    {
      case INVERSE:
      {
//     otbMsgDevMacro(<< "Cartographic coordinates: (" << point[0] << "," << point[1] << ")");

    //from "itk::point" to "ossim::ossimDpt"
        ossimDpt ossimDPoint(point[0], point[1]);

    //map projection
        ossimGpt ossimGPoint;
        ossimGPoint=this->GetMapProjection()->inverse(ossimDPoint);
        ossimGPoint.changeDatum(ossimDatumFactory::instance()->wgs84() );
//     otbGenericMsgDebugMacro(<< "Inverse : " << std::endl << m_MapProjection->print(std::cout));

        outputPoint[0]=ossimGPoint.lon;
        outputPoint[1]=ossimGPoint.lat;
//     otbMsgDevMacro(<< "Geographic coordinates (lon, lat) : (" << outputPoint[0] << "," << outputPoint[1] << ")");
        break;
      }
      case FORWARD:
      {
//     otbMsgDevMacro(<< "Geographic coordinates (lon, lat) : (" << point[1] << "," << point[0] << ")");
    //from "itk::point" to "ossim::ossimGpt"
        ossimGpt ossimGPoint(point[1], point[0]);

    //map projection
        ossimDpt ossimDPoint;
        ossimDPoint=this->GetMapProjection()->forward(ossimGPoint);
//     otbGenericMsgDebugMacro(<< "Forward : ========================= " << std::endl << m_MapProjection->print(std::cout));
        outputPoint[0]=ossimDPoint.x;
        outputPoint[1]=ossimDPoint.y;

//     otbMsgDevMacro(<< "Cartographic coordinates: (" << outputPoint[0] << "," << outputPoint[1] << ")");

        break;
      }
      default:
      {
        itkExceptionMacro(<<"Model is INVERSE or FORWARD only !!");
        break;
      }
    }

    return outputPoint;
  }


  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      void
          GenericMapProjection<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::PrintMap() const
  {
    std::cout << m_MapProjection->print(std::cout);
  }



} // namespace otb

#endif
