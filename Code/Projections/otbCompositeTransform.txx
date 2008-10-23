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
#ifndef __otbCompositeTransform_txx
#define __otbCompositeTransform_txx

#include "otbCompositeTransform.h"
#include "otbMapProjections.h"

namespace otb
{
  
  template<class TFirstTransform, 
           class TSecondTransform, 
           class TScalarType, 
           unsigned int NInputDimensions, 
           unsigned int NOutputDimensions>
  CompositeTransform<TFirstTransform, 
                     TSecondTransform, 
                     TScalarType,  
                     NInputDimensions, 
                     NOutputDimensions>
  ::CompositeTransform() : Superclass(SpaceDimension,ParametersDimension)
  {
    m_FirstTransform = FirstTransformType::New();
    m_SecondTransform = SecondTransformType::New();
  }
  
  template<class TFirstTransform, 
           class TSecondTransform, 
           class TScalarType, 
           unsigned int NInputDimensions, 
           unsigned int NOutputDimensions>
  CompositeTransform<TFirstTransform, 
                     TSecondTransform, 
                     TScalarType, 
                     NInputDimensions, 
                     NOutputDimensions>
  ::~CompositeTransform()
  {
  }
  
  
  template<class TFirstTransform, 
           class TSecondTransform, 
           class TScalarType, 
           unsigned int NInputDimensions, 
           unsigned int NOutputDimensions> 
  typename CompositeTransform<TFirstTransform, 
                              TSecondTransform, 
                              TScalarType,
                              NInputDimensions, 
                              NOutputDimensions>::SecondTransformOutputPointType 
  CompositeTransform<TFirstTransform, 
                     TSecondTransform, 
                     TScalarType, 
                     NInputDimensions, 
                     NOutputDimensions>
  ::TransformPoint(const FirstTransformInputPointType &point1) const
  {
    FirstTransformOutputPointType pointTmp;
    SecondTransformOutputPointType point2;	
    
    pointTmp=m_FirstTransform->TransformPoint(point1); 
    
    otbMsgDevMacro(<< "Geographic point is ("<<  pointTmp[0]<< ","<<  pointTmp[1]<< ")"); 	
    
    point2=m_SecondTransform->TransformPoint(pointTmp); 
    
    otbMsgDevMacro(<< "Coresponding coordinates in sensor geometry are: " << std::endl
		   << point2[0] << ","<< point2[1] );
    
    return point2;
  }
  
  /*template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
    typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputVectorType 
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
    ::TransformVector(const InputVectorType &vector1) const
    { 
    InputVectorType vectorTmp;
    OutputVectorType vector2;	
    
    vectorTmp=m_FirstTransform->TransformVector(vector1); 
    vector2=m_SecondTransform->TransformVector(vectorTmp); 
    
    return vector2;
    }
    
    template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
    typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputVnlVectorType 
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
    ::TransformVector(const InputVnlVectorType &vnlVector1) const
    { 
    InputVnlVectorType vnlVectorTmp;
    OutputVnlVectorType vnlVector2;	
    
    vnlVectorTmp=m_FirstTransform->TransformVector(vnlVector1); 
    vnlVector2=m_SecondTransform->TransformVector(vnlVectorTmp); 
    
    return vnlVector2;
    }
    
    template<class TFirstTransform, class TSecondTransform, class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions> 
    typename CompositeTransform<TFirstTransform, TSecondTransform, TScalarType, NInputDimensions, NOutputDimensions>::OutputCovariantVectorType 
    CompositeTransform<TFirstTransform, TSecondTransform, TScalarType,NInputDimensions, NOutputDimensions>
    ::TransformCovariantVector(const InputCovariantVectorType &covariantVector1) const
    { 
    InputCovariantVectorType covariantVectorTmp;
    OutputCovariantVectorType covariantVector2;	
    
    covariantVectorTmp=m_FirstTransform->TransformCovariantVector(covariantVector1); 
    covariantVector2=m_SecondTransform->TransformCovariantVector(covariantVectorTmp); 
    
    return covariantVector2;
    }*/
  
  
  
  
} // namespace otb

#endif

