/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbGroundSpacingImageFunction_txx
#define __otbGroundSpacingImageFunction_txx

#include "otbGroundSpacingImageFunction.h"

#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TCoordRep>
GroundSpacingImageFunction<TInputImage, TCoordRep>
::GroundSpacingImageFunction()
{
  m_R = 6371000;
  m_deg2radCoef = CONST_PI / 180;
  m_Transform = TransformType::New();
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
void
GroundSpacingImageFunction<TInputImage, TCoordRep>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

/**
 *
 */
template <class TInputImage, class TCoordRep>
typename GroundSpacingImageFunction<TInputImage, TCoordRep>
::FloatType
GroundSpacingImageFunction<TInputImage, TCoordRep>
::EvaluateAtIndex(const IndexType& index) const
{
  if (!this->GetInputImage())
    {
    return (itk::NumericTraits<std::complex<float> >::max());
    }

//   if (!this->IsInsideBuffer(index))
//     {
//     return (itk::NumericTraits<std::complex<float> >::max());
//     }

  PointType point = this->GetPixelLocation(index);
  
  IndexType indexSrcX, indexSrcY;
  indexSrcX[0] =
    static_cast<IndexValueType>(vcl_fabs(static_cast<ValueType>(this->GetInputImage()->GetLargestPossibleRegion().GetSize()[0] -
                                                              index[0])));                                                                   // x position
  indexSrcX[1] = index[1];   // y position

  indexSrcY[0] = index[0];   // x position
  indexSrcY[1] =
    static_cast<IndexValueType>(vcl_fabs(static_cast<ValueType>(this->GetInputImage()->GetLargestPossibleRegion().GetSize()[1] -
                                                              index[1])));
  
  PointType pointSrcX = this->GetPixelLocation(indexSrcX);
  PointType pointSrcY = this->GetPixelLocation(indexSrcY);

  ValueType dLatX = (vcl_fabs(pointSrcX[1] - point[1])) * m_deg2radCoef;
  ValueType dLonX = (vcl_fabs(pointSrcX[0] - point[0])) * m_deg2radCoef;
  
  ValueType aX = vcl_sin(dLatX/2.) * vcl_sin(dLatX/2.) + vcl_cos( point[1] * m_deg2radCoef) * vcl_cos(pointSrcX[1] * m_deg2radCoef) * vcl_sin(dLonX/2.) * vcl_sin(dLonX/2.); 
  ValueType cX = 2. * vcl_atan2(vcl_sqrt(aX), vcl_sqrt(1.-aX)); 
  ValueType dX = m_R * cX;


  ValueType dLatY = (vcl_fabs(pointSrcY[1] - point[1])) * m_deg2radCoef;
  ValueType dLonY = (vcl_fabs(pointSrcY[0] - point[0])) * m_deg2radCoef;
  
  ValueType aY = vcl_sin(dLatY/2.) * vcl_sin(dLatY/2.) + vcl_cos( point[1] * m_deg2radCoef) * vcl_cos(pointSrcY[1] * m_deg2radCoef) * vcl_sin(dLonY/2.) * vcl_sin(dLonY/2.); 
  ValueType cY = 2. * vcl_atan2(vcl_sqrt(aY), vcl_sqrt(1.-aY)); 
  ValueType dY = m_R * cY;
  
  FloatType var (dX / (vcl_fabs(static_cast<ValueType>(indexSrcX[0] - index[0]))), dY / (vcl_fabs(static_cast<ValueType>(indexSrcY[1] - index[1]))));
  
  return var;
}

template <class TInputImage, class TCoordRep>
typename GroundSpacingImageFunction<TInputImage, TCoordRep>
::PointType
GroundSpacingImageFunction<TInputImage, TCoordRep>
::GetPixelLocation(const IndexType& index) const
{
  PointType inputPoint;
  inputPoint[0] = index[0];
  inputPoint[1] = index[1];
  return m_Transform->TransformPoint(inputPoint);
}

} // end namespace otb

#endif
