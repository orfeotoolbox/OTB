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
#ifndef __otbInverseSensorModel_txx
#define __otbInverseSensorModel_txx


#include "otbInverseSensorModel.h"
#include "itkExceptionObject.h"
#include "otbMacro.h"

namespace otb
{

  template < class TScalarType,
	     unsigned int NInputDimensions,
	     unsigned int NOutputDimensions,
	     unsigned int NParametersDimensions >
  InverseSensorModel< TScalarType,
                      NInputDimensions,
                      NOutputDimensions,
                      NParametersDimensions>
  ::InverseSensorModel()
  {
  }

  template < class TScalarType,
	     unsigned int NInputDimensions,
	     unsigned int NOutputDimensions,
	     unsigned int NParametersDimensions >
  InverseSensorModel< TScalarType,
                      NInputDimensions,
                      NOutputDimensions,
                      NParametersDimensions>
  ::~InverseSensorModel()
  {
  }



  template < class TScalarType,
	     unsigned int NInputDimensions,
	     unsigned int NOutputDimensions,
	     unsigned int NParametersDimensions >
  typename InverseSensorModel< TScalarType,NInputDimensions,NOutputDimensions,NParametersDimensions>::OutputPointType
  InverseSensorModel< TScalarType,
                      NInputDimensions,
                      NOutputDimensions,
                      NParametersDimensions>
  ::TransformPoint(const InputPointType &point) const
  {
    otbMsgDevMacro(<< "Geographic point lon/lat : (" << point[0] << "," <<	point[1] << ")");

    // Transformation of "itk::point" in "ossim::ossimGpt"
    ossimGpt ossimGPoint(point[1], point[0]);

    if (this->m_UseDEM)
    {
			otbMsgDevMacro(<< "USING DEM ! ") ;
			otbMsgDevMacro(<< "Point : (" << point[1] << "," << point[0] << ")");
			double height = this->m_DEMHandler->GetHeightAboveMSL(point);
			otbMsgDevMacro(<< "height : " << height) ;
			ossimGPoint.height(height);
    }
    else
    {
      if (this->m_AverageElevation != -10000)
      {
        ossimGPoint.height(this->m_AverageElevation);
      }
    }


    ossimDpt ossimDPoint;

    if( this->m_Model == NULL)
    {
  		itkExceptionMacro(<<"TransformPoint(): Invalid Model pointer m_Model == NULL !");
    }

    this->m_Model->worldToLineSample(ossimGPoint, ossimDPoint); //"worldToLineSample" appelle la m�thode "lineSampleHeightToWorld" pour prendre en compte l'�l�vation.

    OutputPointType outputPoint;



    outputPoint[0]=ossimDPoint.x;
    outputPoint[1]=ossimDPoint.y;

    otbMsgDevMacro(<< "Point in sensor geometry: (" << outputPoint[0] << "," <<	outputPoint[1] << ")");

    return outputPoint;
  }

  template < class TScalarType,
	     unsigned int NInputDimensions,
	     unsigned int NOutputDimensions,
	     unsigned int NParametersDimensions >
  void
  InverseSensorModel< TScalarType,
                      NInputDimensions,
                      NOutputDimensions,
                      NParametersDimensions>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

} // namespace otb

#endif

