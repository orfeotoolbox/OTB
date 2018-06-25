/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbImageSimulationMethod_hxx
#define otbImageSimulationMethod_hxx

#include "otbImageSimulationMethod.h"

namespace otb
{

template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::ImageSimulationMethod()
{
  //instantiation
  m_Spatialisation = SpatialisationType::New();
  m_LabelMapToSimulatedImageFilter = LabelMapToSimulatedImageFilterType::New();
  m_LabelMapToLabelImageFilter = LabelMapToLabelImageFilterType::New();

  //    this->SetNumberOfRequiredOutputs(2);
  //    this->SetNthOutput(0, OutputImageType::New());
  //    this->SetNthOutput(1, OutputLabelImageType::New());

  // TODO fix this. But for now this is mandatory because ProlateInterpolateImageFunction
  // only works with single channel images.
  //    m_FTMFilter = FTMFilterType::New();
  //    m_Interpolator = InterpolatorType::New();
  //    m_MultiToMonoChannelFilter = MultiToMonoChannelFilterType::New();
  m_ImageList = ImageListType::New();
  m_ImageListToVectorImageFilter = ImageListToVectorImageFilterType::New();

  //default value
  m_NumberOfComponentsPerPixel = 3;
  m_Radius = 3;
  m_SatRSRFilename = "";
  m_PathRoot = "";
  m_Mean = 0.0;
  m_Variance = 1e-8;
}

//The 3 commented methods are needed for a true composite filters.

// template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
// void
// ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
// ::SetInput(const InputVectorDataType* object)
// {
//    this->itk::ProcessObject::SetNthInput(0, const_cast<InputVectorDataType *>(object));
// }
//
// template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
// typename ImageSimulationMethod<TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>::InputVectorDataType *
// ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
// ::GetInput()
// {
//    return static_cast<InputVectorDataType *>(this->itk::ProcessObject::GetInput(0));
// }

/** Get output reflectance image */
// template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
// typename ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>::OutputImageType *
// ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
// ::GetOutputReflectanceImage()
// {
//    if(this->GetNumberOfOutputs() < 2)
//    {
//       //exit
//       return 0;
//    }
//    return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
// }
//
// /** Get output label image */
// template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
// typename ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>::OutputLabelImageType *
// ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
// ::GetOutputLabelImage()
// {
//    if(this->GetNumberOfOutputs() < 2)
//    {
//       //exit
//       return 0;
//    }
//    return static_cast<OutputLabelImageType *>(this->itk::ProcessObject::GetOutput(1));
// }

/** Get output reflectance image */
template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
typename ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>::OutputImageType *
ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::GetOutputReflectanceImage()
{
   return m_ImageListToVectorImageFilter->GetOutput();
}

/** Get output label image */
template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
typename ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>::OutputLabelImageType *
ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::GetOutputLabelImage()
{
   return m_LabelMapToLabelImageFilter->GetOutput();
}

template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
void
ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::GenerateOutputInformation()
{
   Superclass::GenerateOutputInformation();

   OutputImageType * outputReflImage = this->GetOutputReflectanceImage();

//    m_Spatialisation->SetInput(m_InputVectorData);
   m_Spatialisation->UpdateOutputInformation();

   outputReflImage->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
   outputReflImage->SetLargestPossibleRegion(m_Spatialisation->GetOutput()->GetLargestPossibleRegion());
   outputReflImage->SetOrigin(m_Spatialisation->GetOutput()->GetOrigin());

   OutputLabelImageType * outputLabelImage = this->GetOutputLabelImage();
   outputLabelImage->SetLargestPossibleRegion(m_Spatialisation->GetOutput()->GetLargestPossibleRegion());
   outputLabelImage->SetOrigin(m_Spatialisation->GetOutput()->GetOrigin());
}

template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
void
ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::UpdateData()
{

  //    m_Spatialisation->SetInput(m_InputVectorData);
  //    m_Spatialisation->SetInitialLabel(1);
  //    m_Spatialisation->SetBackgroundValue(0);

  m_LabelMapToSimulatedImageFilter->SetInput(m_Spatialisation->GetOutput());
  m_LabelMapToSimulatedImageFilter->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
  m_LabelMapToSimulatedImageFilter->SetSatRSRFilename(m_SatRSRFilename);
  m_LabelMapToSimulatedImageFilter->SetNumberOfThreads(1);
  m_LabelMapToSimulatedImageFilter->SetPathRoot(m_PathRoot);
  m_LabelMapToSimulatedImageFilter->SetMean(m_Mean);
  m_LabelMapToSimulatedImageFilter->SetVariance(m_Variance);

  m_LabelMapToLabelImageFilter->SetInput(m_Spatialisation->GetOutput());

  m_LabelMapToSimulatedImageFilter->UpdateOutputInformation();

  for (unsigned int i = 0; i < m_NumberOfComponentsPerPixel; ++i)
    {

    MultiToMonoChannelFilterPointer multiToMonoChannelFilter = MultiToMonoChannelFilterType::New();
    InterpolatorPointer interpolator = InterpolatorType::New();
    FTMFilterPointer FTMFilter = FTMFilterType::New();

    multiToMonoChannelFilter->SetInput(m_LabelMapToSimulatedImageFilter->GetOutput());
    multiToMonoChannelFilter->SetChannel(i + 1);
    multiToMonoChannelFilter->SetExtractionRegion(
                                                  m_LabelMapToSimulatedImageFilter->GetOutput()->GetLargestPossibleRegion());

    //TODO comment changer interpolateur !
    interpolator->SetInputImage(multiToMonoChannelFilter->GetOutput());
    interpolator->SetRadius(m_Radius);
    interpolator->Initialize();

    multiToMonoChannelFilter->UpdateOutputInformation();

    FTMFilter->SetInterpolator(interpolator);
    //       FTMFilter->SetOutputSize(multiToMonoChannelFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
    FTMFilter->SetSize(multiToMonoChannelFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
    FTMFilter->SetOutputSpacing(multiToMonoChannelFilter->GetOutput()->GetSignedSpacing());
    FTMFilter->SetOutputOrigin(multiToMonoChannelFilter->GetOutput()->GetOrigin());
    FTMFilter->SetInput(multiToMonoChannelFilter->GetOutput());
    FTMFilter->Update();

    m_ImageList->PushBack(FTMFilter->GetOutput());
    }
  m_ImageListToVectorImageFilter->SetInput(m_ImageList);

}

//In case of a true composite filter
// template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
// void
// ImageSimulationMethod< TInputVectorData, TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
// ::GenerateData()
// {
//
// //    m_Spatialisation->SetInput(m_InputVectorData);
// //    m_Spatialisation->SetInitialLabel(1);
// //    m_Spatialisation->SetBackgroundValue(0);
//
//    m_LabelMapToSimulatedImageFilter->SetInput(m_Spatialisation->GetOutput());
//    m_LabelMapToSimulatedImageFilter->SetNumberOfComponentsPerPixel(m_NumberOfComponentsPerPixel);
//    m_LabelMapToSimulatedImageFilter->SetSatRSRFilename(m_SatRSRFilename);
//    m_LabelMapToSimulatedImageFilter->SetNumberOfThreads(1);
//    m_LabelMapToSimulatedImageFilter->SetPathRoot(m_PathRoot);
//    m_LabelMapToSimulatedImageFilter->SetMean(m_Mean);
//    m_LabelMapToSimulatedImageFilter->SetVariance(m_Variance);
//
//    m_LabelMapToLabelImageFilter->SetInput(m_Spatialisation->GetOutput());
//    m_LabelMapToLabelImageFilter->GraftNthOutput(0, this->GetOutputLabelImage());
//    m_LabelMapToLabelImageFilter->Update();
//    this->GraftNthOutput(1, m_LabelMapToLabelImageFilter->GetOutput());
//
// //    m_LabelMapToSimulatedImageFilter->GraftNthOutput(0, this->GetOutputReflectanceImage());
// //    m_LabelMapToSimulatedImageFilter->Update();
// //    this->GraftNthOutput(0, m_LabelMapToSimulatedImageFilter->GetOutput());
//
//    for(unsigned int i = 0; i<m_NumberOfComponentsPerPixel; ++i)
//    {
//
//       MultiToMonoChannelFilterPointer multiToMonoChannelFilter = MultiToMonoChannelFilterType::New();
//       InterpolatorPointer interpolator = InterpolatorType::New();
//       FTMFilterPointer FTMFilter = FTMFilterType::New();
//
//       multiToMonoChannelFilter->SetInput(m_LabelMapToSimulatedImageFilter->GetOutput());
//       multiToMonoChannelFilter->SetChannel(i+1);
//       multiToMonoChannelFilter->SetExtractionRegion(m_LabelMapToSimulatedImageFilter->GetOutput()->GetLargestPossibleRegion());
//
//
//       //TODO comment changer interpolateur !
//       interpolator->SetInputImage(multiToMonoChannelFilter->GetOutput());
//       interpolator->SetRadius(m_Radius);
//       interpolator->Initialize();
//
//       multiToMonoChannelFilter->UpdateOutputInformation();
//
//       FTMFilter->SetInterpolator(interpolator);
// //       FTMFilter->SetOutputSize(multiToMonoChannelFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
//       FTMFilter->SetSize(multiToMonoChannelFilter->GetOutput()->GetLargestPossibleRegion().GetSize());
//       FTMFilter->SetOutputSpacing(multiToMonoChannelFilter->GetOutput()->GetSignedSpacing());
//       FTMFilter->SetOutputOrigin(multiToMonoChannelFilter->GetOutput()->GetOrigin());
//       FTMFilter->SetInput(multiToMonoChannelFilter->GetOutput());
//       FTMFilter->Update();
//
//       m_ImageList->PushBack(FTMFilter->GetOutput());
//    }
//    m_ImageListToVectorImageFilter->SetInput(m_ImageList);
//
//    m_ImageListToVectorImageFilter->GraftNthOutput(0, this->GetOutputReflectanceImage());
//    m_ImageListToVectorImageFilter->Update();
//    this->GraftNthOutput(0, m_ImageListToVectorImageFilter->GetOutput());
//
// }


template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
void
ImageSimulationMethod< TInputVectorData,  TSpatialisation, TSimulationStep1, TSimulationStep2, TFTM, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
   Superclass::PrintSelf(os, indent);

}

} //end namespace otb

#endif

