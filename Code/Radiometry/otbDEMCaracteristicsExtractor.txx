/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbDEMCaracteristicsExtractor_txx
#define __otbDEMCaracteristicsExtractor_txx

#include "otbDEMCaracteristicsExtractor.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "otbMath.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
DEMCaracteristicsExtractor<TInputImage, TOutputImage>
::DEMCaracteristicsExtractor()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(3);

  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());
  this->SetNthOutput(2, OutputImageType::New());
  this->SetNthOutput(3, OutputImageType::New());

  m_SolarAngle = 0;
  m_SolarAzimut = 0;
  m_ViewAngle = 0;
  m_ViewAzimut = 0;
}

template <class TInputImage, class TOutputImage>
DEMCaracteristicsExtractor<TInputImage, TOutputImage>
::~DEMCaracteristicsExtractor()
{
}

/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage>
void
DEMCaracteristicsExtractor<TInputImage, TOutputImage>
::GenerateData()
{
  // Input and output pointer definition
  typename InputImageType::Pointer  inputPtr  = const_cast<InputImageType *>(this->GetInput());
  typename OutputImageType::Pointer SlopOutputPtr = this->GetSlopOutput();
  typename OutputImageType::Pointer AspectOutputPtr = this->GetAspectOutput();
  typename OutputImageType::Pointer IncidenceOutputPtr = this->GetIncidenceOutput();
  typename OutputImageType::Pointer ExitanceOutputPtr = this->GetExitanceOutput();

  // Gradient Magnitude Image Filter used to compute the slope.
  typename GradientMagnitudeFilterType::Pointer GradientMagnitudeFilter = GradientMagnitudeFilterType::New();
  // Gradient Recursive Gaussian Image Filter used to compute the aspect.
  typename GradientRecursiveGaussianImageFilterType::Pointer GradientRecursiveGaussianFilter =
    GradientRecursiveGaussianImageFilterType::New();
  // Atan used to compute the slop
  typename AtanFilterType::Pointer AtanFilter   = AtanFilterType::New();
  // Atan2 Image Filter used to compute the aspect.
  typename Atan2FilterType::Pointer AspectFilter = Atan2FilterType::New();
  // Inverse cosinus Image filter used to compute the incidence image
  typename AcosImageFilterType::Pointer IncidenceFilter = AcosImageFilterType::New();
  // Inverse cosinus Image filter used to compute the exitance image
  typename AcosImageFilterType::Pointer ExitanceFilter  = AcosImageFilterType::New();

  // Degrees To Radian _-> Radian To Degree coefficient
  double rad2degCoef;
  rad2degCoef = 180 / CONST_PI;

  // Slop calculation
  GradientMagnitudeFilter->SetInput(inputPtr);
  AtanFilter->SetInput(GradientMagnitudeFilter->GetOutput());
  // Transform values from radian to degrees.
  typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter = MultiplyByScalarImageFilterType::New();
  //rad2DegFilter->SetInput( GradientMagnitudeFilter->GetOutput() );
  rad2DegFilter->SetInput(AtanFilter->GetOutput());
  rad2DegFilter->SetCoef(rad2degCoef);
  rad2DegFilter->GraftOutput(SlopOutputPtr);
  rad2DegFilter->Update();
  this->GraftNthOutput(0, rad2DegFilter->GetOutput());

  // Aspect calcultation
  GradientRecursiveGaussianFilter->SetInput(inputPtr);
  GradientRecursiveGaussianFilter->Update();

  // // Extract the X and the Y gradient
  typename AdaptorType::Pointer XAdaptator = AdaptorType::New();
  typename AdaptorType::Pointer YAdaptator = AdaptorType::New();
  XAdaptator->SetImage(GradientRecursiveGaussianFilter->GetOutput());
  YAdaptator->SetImage(GradientRecursiveGaussianFilter->GetOutput());
  XAdaptator->SelectNthElement(0);
  YAdaptator->SelectNthElement(1);
  // // Compute Arctan
  AspectFilter->SetInput1(XAdaptator);
  AspectFilter->SetInput2(YAdaptator);
  // // Transform values from radian to degres.
  typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter1 = MultiplyByScalarImageFilterType::New();
  rad2DegFilter1->SetInput(AspectFilter->GetOutput());
  rad2DegFilter1->SetCoef(rad2degCoef);
  rad2DegFilter1->GraftOutput(AspectOutputPtr);
  rad2DegFilter1->Update();
  this->GraftNthOutput(1, rad2DegFilter1->GetOutput());

  // Angle calculation :
  // sin(slop)
  typename SinImageFilterType::Pointer sinS = SinImageFilterType::New();
  sinS->SetInput(GradientMagnitudeFilter->GetOutput());
  // cos (slop)
  typename CosImageFilterType::Pointer cosS = CosImageFilterType::New();
  cosS->SetInput(GradientMagnitudeFilter->GetOutput());
  // -aspect
  typename MultiplyByScalarImageFilterType::Pointer oppositeFilter = MultiplyByScalarImageFilterType::New();
  oppositeFilter->SetInput(AspectFilter->GetOutput());
  oppositeFilter->SetCoef(-1);

  // Incidence calculation
  typename ShiftScaleImageFilterType::Pointer addAzimut = ShiftScaleImageFilterType::New();
  addAzimut->SetScale(1.);
  addAzimut->SetShift(m_SolarAzimut / rad2degCoef);
  addAzimut->SetInput(oppositeFilter->GetOutput());

  typename CosImageFilterType::Pointer cosAAzimut = CosImageFilterType::New();
  cosAAzimut->SetInput(addAzimut->GetOutput());

  typename MultiplyByScalarImageFilterType::Pointer sinSsinSolarAngleFilter = MultiplyByScalarImageFilterType::New();
  sinSsinSolarAngleFilter->SetCoef(vcl_sin(m_SolarAngle / rad2degCoef));
  sinSsinSolarAngleFilter->SetInput(sinS->GetOutput());

  typename MultiplyImageFilterType::Pointer cosAAzimuthsinSsinAngle =  MultiplyImageFilterType::New();
  cosAAzimuthsinSsinAngle->SetInput1(sinSsinSolarAngleFilter->GetOutput());
  cosAAzimuthsinSsinAngle->SetInput2(cosAAzimut->GetOutput());

  typename MultiplyByScalarImageFilterType::Pointer cosScosSolarAngleFilter = MultiplyByScalarImageFilterType::New();
  cosScosSolarAngleFilter->SetCoef(vcl_cos(m_SolarAngle / rad2degCoef));
  cosScosSolarAngleFilter->SetInput(cosS->GetOutput());

  typename AddImageFilterType::Pointer cosIncidence = AddImageFilterType::New();
  cosIncidence->SetInput1(cosAAzimuthsinSsinAngle->GetOutput());
  cosIncidence->SetInput2(cosScosSolarAngleFilter->GetOutput());

  IncidenceFilter->SetInput(cosIncidence->GetOutput());

  // // Change radians in degres
  typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter2 = MultiplyByScalarImageFilterType::New();
  rad2DegFilter2->SetInput(IncidenceFilter->GetOutput());
  rad2DegFilter2->SetCoef(rad2degCoef);
  // // Link to the output
  rad2DegFilter2->GraftOutput(IncidenceOutputPtr);
  rad2DegFilter2->Update();
  this->GraftNthOutput(2, rad2DegFilter2->GetOutput());

  // Exitance calculation
  typename ShiftScaleImageFilterType::Pointer addAzimut2 = ShiftScaleImageFilterType::New();
  addAzimut2->SetScale(1.);
  addAzimut2->SetShift(m_ViewAzimut / rad2degCoef);
  addAzimut2->SetInput(oppositeFilter->GetOutput());

  typename CosImageFilterType::Pointer cosAAzimut2 = CosImageFilterType::New();
  cosAAzimut2->SetInput(addAzimut2->GetOutput());

  typename MultiplyByScalarImageFilterType::Pointer sinSsinSolarAngleFilter2 = MultiplyByScalarImageFilterType::New();
  sinSsinSolarAngleFilter2->SetCoef(vcl_sin(m_ViewAngle / rad2degCoef));
  sinSsinSolarAngleFilter2->SetInput(sinS->GetOutput());

  typename MultiplyImageFilterType::Pointer cosAAzimuthsinSsinAngle2 =  MultiplyImageFilterType::New();
  cosAAzimuthsinSsinAngle2->SetInput1(sinSsinSolarAngleFilter2->GetOutput());
  cosAAzimuthsinSsinAngle2->SetInput2(cosAAzimut2->GetOutput());

  typename MultiplyByScalarImageFilterType::Pointer cosScosSolarAngleFilter2 = MultiplyByScalarImageFilterType::New();
  cosScosSolarAngleFilter2->SetCoef(vcl_cos(m_ViewAngle / rad2degCoef));
  cosScosSolarAngleFilter2->SetInput(cosS->GetOutput());

  typename AddImageFilterType::Pointer cosIncidence2 = AddImageFilterType::New();
  cosIncidence2->SetInput1(cosAAzimuthsinSsinAngle2->GetOutput());
  cosIncidence2->SetInput2(cosScosSolarAngleFilter2->GetOutput());

  ExitanceFilter->SetInput(cosIncidence2->GetOutput());

  // // Change radians in degres
  typename MultiplyByScalarImageFilterType::Pointer rad2DegFilter3 = MultiplyByScalarImageFilterType::New();
  rad2DegFilter3->SetInput(ExitanceFilter->GetOutput());
  rad2DegFilter3->SetCoef(rad2degCoef);
  // // Link to the output
  rad2DegFilter3->GraftOutput(ExitanceOutputPtr);
  rad2DegFilter3->Update();
  this->GraftNthOutput(3, rad2DegFilter3->GetOutput());

}

/**PrintSelf method */
template <class TInputImage, class TOutputImage>
void
DEMCaracteristicsExtractor<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Solar Angle: " << m_SolarAngle << std::endl;
  os << indent << "Solar Azimut: " << m_SolarAzimut << std::endl;
  os << indent << "View Angle: " << m_ViewAngle << std::endl;
  os << indent << "View Azimut: " << m_ViewAzimut << std::endl;
}

} // end namespace otb

#endif
