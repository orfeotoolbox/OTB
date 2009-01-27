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

  m_HasBeenGenerated = false;
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
::EvaluateAtIndex(const IndexType& index)
{
  // generate data again
  if(!m_HasBeenGenerated)
    {
      m_Detector->SetInput(this->GetInputImage());
      m_Detector->Update();
      m_HasBeenGenerated = true;

    }
  // Call the const implementation
  return this->EvaluateAtIndex(index);
}

template <class TInputImage, class TDetector ,class TCount >
typename CountImageFunction< TInputImage, TDetector , TCount >
::RealType
CountImageFunction<TInputImage,TDetector , TCount>
::EvaluateAtIndex(const IndexType& index) const
{
  CountType  countDensity;
  return countDensity(m_Detector->GetOutput(),this->GetNeighborhoodRadius(),index );
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

// /**
//  * Modified
//  */
template <class TInputImage, class TDetector ,class TCount >
void
CountImageFunction<TInputImage,TDetector , TCount>
::Modified()
{
  m_HasBeenGenerated = false ;
  Superclass::Modified();
  m_Detector->Modified();
}




} // end namespace otb

#endif
