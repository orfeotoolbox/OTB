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
#ifndef __otbGenericRoadExtractionFilter_txx
#define __otbGenericRoadExtractionFilter_txx

#include "otbGenericRoadExtractionFilter.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputPath>
GenericRoadExtractionFilter<TInputImage, TOutputPath>
::GenericRoadExtractionFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  m_SquareRootImageFilter = SquareRootImageFilterType::New();
  m_GradientFilter = GradientFilterType::New();
  m_NeighborhoodScalarProductFilter = NeighborhoodScalarProductFilterType::New();
  m_RemoveIsolatedByDirectionFilter = RemoveIsolatedByDirectionFilterType::New();
  m_RemoveWrongDirectionFilter = RemoveWrongDirectionFilterType::New();
  m_NonMaxRemovalByDirectionFilter = NonMaxRemovalByDirectionFilterType::New();
  m_VectorizationPathListFilter = VectorizationPathListFilterType::New();
  m_FirstSimplifyPathListFilter = SimplifyPathListFilterType::New();
  m_SecondSimplifyPathListFilter = SimplifyPathListFilterType::New();
  m_BreakAngularPathListFilter = BreakAngularPathListFilterType::New();
  m_FirstRemoveTortuousPathListFilter = RemoveTortuousPathListFilterType::New();
  m_SecondRemoveTortuousPathListFilter = RemoveTortuousPathListFilterType::New();
  m_LinkPathListFilter = LinkPathListFilterType::New();
  m_LikelihoodPathListFilter = LikelihoodPathListFilterType::New();

  /** Amplitude threshold to start following a path (use by the VectorizationPathListFilter)*/
  m_AmplitudeThreshold = static_cast<AmplitudeThresholdType>(0.00005);
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by the SimplifyPathFilter)*/
  m_Tolerance = static_cast<ToleranceType>(1.);
  /** Max angle (use bye the BreakAngularPathListFilter)*/
  m_MaxAngle = static_cast<MaxAngleType>(CONST_PI_8);
  /** Tolerance for segment consistency (tolerance in terms of distance) (use by RemoveTortuousPathFilter)*/
  m_FirstMeanDistanceThreshold = static_cast<MeanDistanceThresholdType>(1.);
  m_SecondMeanDistanceThreshold = static_cast<MeanDistanceThresholdType>(10.);
  /** The angular threshold (use by LinkPathFilter) */
  m_AngularThreshold = static_cast<LinkRealType>(CONST_PI_8);

  /** The distance threshold (use by LinkPathFilter) */
  m_DistanceThreshold = 25.;

  /** Alpha value */
  /** Use to calculate the sigma value use by the GradientRecursiveGaussianImageFilter */
  m_Alpha = 1.0;

  /** Resolution of the image */
  m_Resolution = 1.;
}
/**
 * Prepare main computation method
 */
template <class TInputImage, class TOutputPath>
void
GenericRoadExtractionFilter<TInputImage, TOutputPath>
::BeforeGenerateData()
{
  /** Calculation of resolution value */
  typename InputImageType::SpacingType spacing = this->GetInput()->GetSpacing();
  // Getting x Spacing for the resolution
  m_Resolution = static_cast<double>(spacing[0]);
  if (m_Resolution == 0.)
    {
    itkWarningMacro(<< "The image spacing is zero. So the resolution used in the filter is forced to 1.");
    m_Resolution = 1.;
    }

}

/**
 * Main computation method
 */
template <class TInputImage, class TOutputPath>
void
GenericRoadExtractionFilter<TInputImage, TOutputPath>
::GenerateData()
{
  // // Input images pointers
  typename InputImageType::ConstPointer inputImage     = this->GetInput();
  typename OutputPathListType::Pointer  outputPathList  = this->GetOutput();

  ///////////////////////////////////////
  //// Algorithm for road extraction ////
  ///////////////////////////////////////

  //

  m_SquareRootImageFilter->SetInput(inputImage);

  m_GradientFilter->SetInput(m_SquareRootImageFilter->GetOutput());
  /** Sigma calculated with the alpha and image resolution parameters */
  m_GradientFilter->SetSigma(static_cast<SigmaType>(m_Alpha * (1.2 / m_Resolution + 1.)));
  m_GradientFilter->SetUseImageDirection(false);

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

  m_FirstSimplifyPathListFilter->SetInput(m_VectorizationPathListFilter->GetOutput());
  m_FirstSimplifyPathListFilter->GetFunctor().SetTolerance(m_Tolerance);

  m_BreakAngularPathListFilter->SetInput(m_FirstSimplifyPathListFilter->GetOutput());
  m_BreakAngularPathListFilter->SetMaxAngle(m_MaxAngle);

  m_FirstRemoveTortuousPathListFilter->SetInput(m_BreakAngularPathListFilter->GetOutput());
  m_FirstRemoveTortuousPathListFilter->GetFunctor().SetThreshold(m_FirstMeanDistanceThreshold);

  m_LinkPathListFilter->SetInput(m_FirstRemoveTortuousPathListFilter->GetOutput());
  m_LinkPathListFilter->SetAngularThreshold(m_AngularThreshold);
  m_LinkPathListFilter->SetDistanceThreshold(static_cast<LinkRealType>(m_DistanceThreshold / m_Resolution));

  m_SecondSimplifyPathListFilter->SetInput(m_LinkPathListFilter->GetOutput());
  m_SecondSimplifyPathListFilter->GetFunctor().SetTolerance(m_Tolerance);

  m_SecondRemoveTortuousPathListFilter->SetInput(m_SecondSimplifyPathListFilter->GetOutput());
  m_SecondRemoveTortuousPathListFilter->GetFunctor().SetThreshold(m_SecondMeanDistanceThreshold);

  m_LikelihoodPathListFilter->SetInput(m_SecondRemoveTortuousPathListFilter->GetOutput());
  m_LikelihoodPathListFilter->SetInputImage(m_NonMaxRemovalByDirectionFilter->GetOutput());

  // Graft output seems to be broken for PolylineParametricPath
  // So we use update, and copy the path to the output path list.
  // m_LikelihoodPathListFilter->GraftOutput(this->GetOutput());
  m_LikelihoodPathListFilter->Update();
  // outputPathList =  m_LikelihoodPathListFilter->GetOutput();
  for (typename LikelihoodPathListFilterType::PathListType::ConstIterator it
         = m_LikelihoodPathListFilter->GetOutput()->Begin();
       it != m_LikelihoodPathListFilter->GetOutput()->End();
       ++it)
    {
    outputPathList->PushBack(it.Get());
    }
}
/**
 * PrintSelf method
 */
template <class TInputImage, class TOutputPath>
void
GenericRoadExtractionFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "m_Alpha:" << m_Alpha << std::endl;
  os << indent << "m_Resolution:" << m_Resolution << std::endl;
  os << indent << "m_AmplitudeThreshold: " << m_AmplitudeThreshold << std::endl;
  os << indent << "m_Tolerance: " << m_Tolerance << std::endl;
  os << indent << "m_MaxAngle: " << m_MaxAngle << std::endl;
  os << indent << "m_FirstMeanDistanceThreshold: " << m_FirstMeanDistanceThreshold << std::endl;
  os << indent << "m_SecondMeanDistanceThreshold: " << m_SecondMeanDistanceThreshold << std::endl;
  os << indent << "m_DistanceThreshold: " << m_DistanceThreshold << std::endl;
  os << indent << "m_AngularThreshold: " << m_AngularThreshold << std::endl;

}
} // End namespace otb
#endif
