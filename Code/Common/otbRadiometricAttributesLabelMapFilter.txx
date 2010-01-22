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
#ifndef __otbRadiometricAttributesLabelMapFilter_txx
#define __otbRadiometricAttributesLabelMapFilter_txx

#include "otbRadiometricAttributesLabelMapFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TImage, class TFeatureImage>
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::RadiometricAttributesLabelMapFilter() : m_RedChannelIndex(2), m_GreenChannelIndex(1), m_BlueChannelIndex(0), m_NIRChannelIndex(3), m_ReducedAttributeSet(true)
{
  this->SetNumberOfRequiredInputs(2);
}

template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::GenerateData()
{
  GEMIFilterPointerType gemi = GEMIFilterType::New();
  gemi->SetRedIndex(m_RedChannelIndex+1);
  gemi->SetNIRIndex(m_NIRChannelIndex+1);
  gemi->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats1 = StatisticsLabelMapFilterType::New();
  stats1->SetInput(this->GetInput());
  stats1->SetFeatureImage(gemi->GetOutput());
  stats1->SetFeatureName("Gemi");
  stats1->SetReducedAttributeSet(m_ReducedAttributeSet);

  NDVIFilterPointerType ndvi = NDVIFilterType::New();
  ndvi->SetRedIndex(m_RedChannelIndex+1);
  ndvi->SetNIRIndex(m_NIRChannelIndex+1);
  ndvi->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats2 = StatisticsLabelMapFilterType::New();
  stats2->SetInput(stats1->GetOutput());
  stats2->SetFeatureImage(ndvi->GetOutput());
  stats2->SetFeatureName("Ndvi");
  stats2->SetReducedAttributeSet(m_ReducedAttributeSet);

  IRFilterPointerType ir = IRFilterType::New();
  ir->SetGreenIndex(m_GreenChannelIndex+1);
  ir->SetRedIndex(m_RedChannelIndex+1);
  ir->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats3 = StatisticsLabelMapFilterType::New();
  stats3->SetInput(stats2->GetOutput());
  stats3->SetFeatureImage(ir->GetOutput());
  stats3->SetFeatureName("Redness");
  stats3->SetReducedAttributeSet(m_ReducedAttributeSet);

  ICFilterPointerType ic = ICFilterType::New();
  ic->SetGreenIndex(m_GreenChannelIndex+1);
  ic->SetRedIndex(m_RedChannelIndex+1);
  ic->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats4 = StatisticsLabelMapFilterType::New();
  stats4->SetInput(stats3->GetOutput());
  stats4->SetFeatureImage(ic->GetOutput());
  stats4->SetFeatureName("Color");
  stats4->SetReducedAttributeSet(m_ReducedAttributeSet);

  IBFilterPointerType ib = IBFilterType::New();
  ib->SetGreenIndex(m_GreenChannelIndex+1);
  ib->SetRedIndex(m_RedChannelIndex+1);
  ib->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats5 = StatisticsLabelMapFilterType::New();
  stats5->SetInput(stats4->GetOutput());
  stats5->SetFeatureImage(ib->GetOutput());
  stats5->SetFeatureName("Brightness");
  stats5->SetReducedAttributeSet(m_ReducedAttributeSet);

  NDWI2FilterPointerType ndwi2 = NDWI2FilterType::New();
  ndwi2->GetFunctor().SetGIndex(m_GreenChannelIndex+1);
  ndwi2->GetFunctor().SetNIRIndex(m_NIRChannelIndex+1);
  ndwi2->SetInput(this->GetFeatureImage());
  
  StatisticsLabelMapFilterPointerType stats6 = StatisticsLabelMapFilterType::New();
  stats6->SetInput(stats5->GetOutput());
  stats6->SetFeatureImage(ndwi2->GetOutput());
  stats6->SetFeatureName("Ndwi2");
  stats6->SetReducedAttributeSet(m_ReducedAttributeSet);

  IntensityFilterPointerType intensity = IntensityFilterType::New();
  intensity->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats7 = StatisticsLabelMapFilterType::New();
  stats7->SetInput(stats6->GetOutput());
  stats7->SetFeatureImage(intensity->GetOutput());
  stats7->SetFeatureName("Intensity");
  stats7->SetReducedAttributeSet(m_ReducedAttributeSet);

  ChannelFilterPointerType red = ChannelFilterType::New();
  red->SetChannel(m_RedChannelIndex+1);
  red->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats8 = StatisticsLabelMapFilterType::New();
  stats8->SetInput(stats7->GetOutput());
  stats8->SetFeatureImage(red->GetOutput());
  stats8->SetFeatureName("Red");
  stats8->SetReducedAttributeSet(m_ReducedAttributeSet);

  ChannelFilterPointerType green = ChannelFilterType::New();
  green->SetChannel(m_GreenChannelIndex+1);
  green->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats9 = StatisticsLabelMapFilterType::New();
  stats9->SetInput(stats8->GetOutput());
  stats9->SetFeatureImage(green->GetOutput());
  stats9->SetFeatureName("Green");
  stats9->SetReducedAttributeSet(m_ReducedAttributeSet);

  ChannelFilterPointerType blue = ChannelFilterType::New();
  blue->SetChannel(m_BlueChannelIndex+1);
  blue->SetInput(this->GetFeatureImage());
  
  StatisticsLabelMapFilterPointerType stats10 = StatisticsLabelMapFilterType::New();
  stats10->SetInput(stats9->GetOutput());
  stats10->SetFeatureImage(blue->GetOutput());
  stats10->SetFeatureName("Blue");
  stats10->SetReducedAttributeSet(m_ReducedAttributeSet);

  ChannelFilterPointerType nir = ChannelFilterType::New();
  nir->SetChannel(m_NIRChannelIndex+1);
  nir->SetInput(this->GetFeatureImage());

  StatisticsLabelMapFilterPointerType stats11 = StatisticsLabelMapFilterType::New();
  stats11->SetInput(stats10->GetOutput());
  stats11->SetFeatureImage(nir->GetOutput());
  stats11->SetFeatureName("Nir");
  stats11->SetReducedAttributeSet(m_ReducedAttributeSet);

  stats11->GraftOutput(this->GetOutput());
  stats11->Update();
  this->GraftOutput(stats11->GetOutput());
}

template <class TImage, class TFeatureImage>
void
RadiometricAttributesLabelMapFilter<TImage, TFeatureImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}// end namespace itk
#endif
