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
#ifndef __otbNetworkedQuadTreeTransform_txx
#define __otbNetworkedQuadTreeTransform_txx

#include "otbNetworkedQuadTreeTransform.h"
#include "otbMacro.h"

namespace otb
{
  
  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::NetworkedQuadTreeTransform() : Superclass(SpaceDimension,ParametersDimension)
  {
    m_NetworkedQuadTreeTransform =  new OssimNetworkedQuadTreeTransformType();
  }
  
  
  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::~NetworkedQuadTreeTransform()
  {
    delete m_NetworkedQuadTreeTransform;
  }
  
  
  
  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      typename NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>::OutputPointType
          NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::TransformPoint(const InputPointType & point) const
  {
    OutputPointType outputPoint;
    
    switch(DirectionOfMapping)
    {
      case INVERSE:
      {
        otbMsgDevMacro(<< "Cartographic coordinates: (" << point[0] << "," << point[1] << ")");
	  
	  //from "itk::point" to "ossim::ossimDpt" 
        ossimDpt ossimDPoint(point[0], point[1]);
	  
	  //map projection
        ossimGpt ossimGPoint;
//         ossimGPoint=m_NetworkedQuadTreeTransform->inverse(ossimDPoint);
        m_NetworkedQuadTreeTransform->lineSampleToWorld(ossimDPoint, ossimGPoint);
//	 	otbGenericMsgDebugMacro(<< "Inverse : " << std::endl << m_NetworkedQuadTreeTransform->print(std::cout));
		
        outputPoint[0]=ossimGPoint.lon;
        outputPoint[1]=ossimGPoint.lat;
        otbMsgDevMacro(<< "Geographic coordinates (long/lat) : (" << outputPoint[0] << "," << outputPoint[1] << ")");
        break;
      }
      case FORWARD:
      {
        otbMsgDevMacro(<< "Geographic coordinates (long/lat) : (" << point[1] << "," << point[0] << ")");
		//from "itk::point" to "ossim::ossimGpt"
        ossimGpt ossimGPoint(point[1], point[0]);
	  
	  //map projection
        ossimDpt ossimDPoint;
//         ossimDPoint=m_NetworkedQuadTreeTransform->forward(ossimGPoint);
        m_NetworkedQuadTreeTransform->worldToLineSample(ossimGPoint, ossimDPoint);
//	 	otbGenericMsgDebugMacro(<< "Forward : ========================= " << std::endl << m_NetworkedQuadTreeTransform->print(std::cout));
        outputPoint[0]=ossimDPoint.x;
        outputPoint[1]=ossimDPoint.y;
		
        otbMsgDevMacro(<< "Cartographic coordinates: (" << outputPoint[0] << "," << outputPoint[1] << ")");

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
  
  
  
  ///\return The geographic point corresponding to (0,0)
  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
      typename NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>::InputPointType 
          NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::Origin()
  {
    ossimGpt ossimOrigin=m_NetworkedQuadTreeTransform->origin();
    InputPointType otbOrigin;
    otbOrigin[0]= ossimOrigin.lat;
    otbOrigin[1]= ossimOrigin.lon;
    
    return otbOrigin;
  }
  


  
	
  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
      void 
          NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::PrintMap() const
  {
    std::cout << m_NetworkedQuadTreeTransform->print(std::cout);
  }

  template<InverseOrForwardTransformationEnum Transform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
      void NetworkedQuadTreeTransform<Transform, TScalarType, NInputDimensions, NOutputDimensions>
  ::SetLevel(unsigned char level)
  {
    m_NetworkedQuadTreeTransform->setDepth(level);
  }
	
} // namespace otb

#endif
