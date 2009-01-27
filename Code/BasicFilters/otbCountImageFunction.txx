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
#ifndef __otbCountImageFunction_txx
#define __otbCountImageFunction_txx

#include "otbCountImageFunction.h"

#include "itkProcessObject.h"
#include "itkNumericTraits.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TDetector ,class TCount >
CountImageFunction<TInputImage,TDetector , TCount>
::CountImageFunction()
{
  m_NeighborhoodRadius = 1;
  m_Detector = DetectorType::New();
}


template <class TInputImage, class TDetector ,class TCount >
void
CountImageFunction< TInputImage, TDetector , TCount >
::SetInputImage(const InputImageType * ptr)
{
  Superclass::SetInputImage(ptr);
  m_Detector->SetInput(ptr);
}

/**
 *
 */
template <class TInputImage, class TDetector ,class TCount >
void
CountImageFunction< TInputImage, TDetector , TCount >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
  os << indent << "NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;
}


/**
 *
 */
template <class TInputImage, class TDetector ,class TCount >
typename CountImageFunction< TInputImage, TDetector , TCount >
::RealType
CountImageFunction<TInputImage,TDetector , TCount>
::EvaluateAtIndex(const IndexType& index) const 
{
  m_Detector->Update();
  CountType  countDensity;
  return countDensity(m_Detector->GetOutput(),m_NeighborhoodRadius,index );
}
/**
 * SetDetector method
 */
template <class TInputImage, class TDetector ,class TCount >
void
CountImageFunction<TInputImage,TDetector , TCount>
::SetDetector( DetectorType* detector)
{
  m_Detector = detector;
  if(this->GetInputImage())
    {
      m_Detector->SetInput(this->GetInputImage());
    }
}

/**
 * GetDetector method
 */
template <class TInputImage, class TDetector ,class TCount >
typename CountImageFunction< TInputImage, TDetector , TCount >
::DetectorType *
CountImageFunction<TInputImage,TDetector , TCount>
::GetDetector() 
{
  return m_Detector;
}
} // end namespace otb

#endif
