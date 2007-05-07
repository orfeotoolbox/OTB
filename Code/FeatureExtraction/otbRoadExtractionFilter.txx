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
#ifndef _otbRoadExtractionFilter_txx
#define _otbRoadExtractionFilter_txx

#include "otbRoadExtractionFilter.h"




namespace otb
{
namespace MorphologicalPyramid
{
/**
 * Constructor
 */
template <class TInputImage,class TOutputPath>
RoadExtractionFilter<TInputImage, TOutputPath>
::RoadExtractionFilter()
{
        this->SetNumberOfRequiredInputs(1);
        this->SetNumberOfRequiredOutputs(1);

        m_SpectralAngleDistanceImageFilter = SpectralAngleDistanceImageFilterType::New();
        m_GradientFilter = GradientFilterType::New();
        m_NeighborhoodScalarProductFilter = NeighborhoodScalarProductFilterType::New();
        m_RemoveIsolatedByDirectionFilter = RemoveIsolatedByDirectionFilterType::New();
        m_RemoveWrongDirectionFilter = RemoveWrongDirectionFilterType::New();
        m_NonMaxRemovalByDirectionFilter = NonMaxRemovalByDirectionFilterType::New();
        m_VectorizationPathListFilter = VectorizationPathListFilterType::New();
        m_FirstSimplifyPathFilter = SimplifyPathFilterType::New();
        m_SecondSimplifyPathFilter = SimplifyPathFilterType::New();
        m_BreakAngularPathListFilter = BreakAngularPathListFilterType::New();
        m_FirstRemoveTortuousPathFilter = RemoveTortuousPathFilterType::New();
        m_SecondRemoveTortuousPathFilter = RemoveTortuousPathFilterType::New();
        m_LinkPathFilter = LinkPathFilterType::New();


}

/**
 * Main computation method
 */
template <class TInputImage,class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // Input images pointers
  typename InputImageType::ConstPointer inputImage     = this->GetInput();
  typename OutputPathListType::Pointer outputPathList  = this->GetOutput();

  ///////////////////////////////////////
  //// Algorithm for extract roads //////
  ///////////////////////////////////////

  // Pipeline connection
  
  // 
  m_SpectralAngleDistanceImageFilter->SetInput(inputImage);
  m_SpectralAngleDistanceImageFilter->SetReferencePixel(m_ReferencePixel);
  
  m_GradientFilter->SetInput(m_SpectralAngleDistanceImageFilter->GetOutput());
  m_GradientFilter->SetSigma(m_Sigma);
  
  m_NeighborhoodScalarProductFilter->SetInput(m_GradientFilter->GetOutput());
  
  m_RemoveIsolatedByDirectionFilter->SetInput(m_NeighborhoodScalarProductFilter->GetOutput());
  m_RemoveIsolatedByDirectionFilter->SetInputDirection(m_NeighborhoodScalarProductFilter->GetOutputDirection());
  
  m_RemoveWrongDirectionFilter->SetInput(m_RemoveIsolatedByDirectionFilter->GetOutput());
  m_RemoveWrongDirectionFilter->SetInputDirection(m_NeighborhoodScalarProductFilter->GetOutputDirection());

  m_NonMaxRemovalByDirectionFilter->SetInput(m_RemoveWrongDirectionFilter->GetOutput());
  m_NonMaxRemovalByDirectionFilter->SetInputDirection(m_NeighborhoodScalarProductFilter->GetOutputDirection());

  m_VectorizationPathListFilter->SetInput(m_NonMaxRemovalByDirectionFilter->GetOutput());
  m_VectorizationPathListFilter->SetInputDirection(m_NeighborhoodScalarProductFilter->GetOutputDirection());
  m_VectorizationPathListFilter->SetAmplitudeThreshold(m_AmplitudeThreshold);
  
  m_FirstSimplifyPathFilter->SetInput(m_VectorizationPathListFilter->GetOutput());
  m_FirstSimplifyPathFilter->SetTolerance(m_Tolerance);
  
  m_BreakAngularPathListFilter->SetInput(m_FirstSimplifyPathFilter->GetOutput());
  m_BreakAngularPathListFilter->SetMaxAngle(m_MaxAngle);
  
  m_FirstRemoveTortuousPathFilter->SetInput(m_BreakAngularPathListFilter->GetOutput());
  m_FirstRemoveTortuousPathFilter->SetMeanDistanceThreshold(m_MeanDistanceThreshold);
  
  m_LinkPathFilter->SetInput(m_FirstRemoveTortuousPathFilter->GetOutput());
 
  m_SecondSimplifyPathFilter->SetInput(m_LinkPathFilter->GetOutput());
  m_SecondSimplifyPathFilter->SetTolerance(m_Tolerance);
 
  m_SecondRemoveTortuousPathFilter->SetInput(m_SecondSimplifyPathFilter->GetOutput());

  m_SecondRemoveTortuousPathFilter->GraftOutput(this->GetOutput());
  m_SecondRemoveTortuousPathFilter->Update();
  this->GraftOutput(m_SecondRemoveTortuousPathFilter->GetOutput());

}
/**
 * PrintSelf method
 */
template <class TInputImage,class TOutputPath>
void
RoadExtractionFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "m_ReferencePixel: "<< m_ReferencePixel << std::endl;
  os << indent << "m_Sigma: "<< m_Sigma << std::endl;
  os << indent << "m_AmplitudeThreshold: "<< m_AmplitudeThreshold << std::endl;
  os << indent << "m_Tolerance: "<< m_Tolerance << std::endl;
  os << indent << "m_MaxAngle: "<< m_MaxAngle << std::endl;
  os << indent << "m_MeanDistanceThreshold: "<< m_MeanDistanceThreshold << std::endl;

}
} // End namespace MorphologicalPyramid
} // End namespace otb
#endif
