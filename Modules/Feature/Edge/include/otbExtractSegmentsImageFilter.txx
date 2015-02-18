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
#ifndef __otbExtractSegmentsImageFilter_txx
#define __otbExtractSegmentsImageFilter_txx

#include "otbExtractSegmentsImageFilter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::ExtractSegmentsImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_PixelSuppression     = PixelSuppressionType::New();
  m_LocalHough      = LocalHoughType::New();
  m_FillGaps         = FillGapsType::New();
  m_DrawLineList         = DrawLineListType::New();
  m_Rescaler             = RescaleType::New();

  m_LineValue = static_cast<typename OutputImageType::PixelType>(255.);
}

/**
 * Set/Get image input
 */
template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetInputImage(const InputImageType *image)
{
  this->SetInput(0, image);
}

template <class TInputImage, class TOutputImage>
const typename ExtractSegmentsImageFilter<TInputImage, TOutputImage>::InputImageType *
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetInputImage()
{
  return static_cast<const InputImageType *>
           (this->GetInput(0));
}

/**
 * Set/Get image direction input
 */
template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetInputImageDirection(const InputImageType *image)
{
  this->SetInput(1, image);
}

template <class TInputImage, class TOutputImage>
const typename ExtractSegmentsImageFilter<TInputImage, TOutputImage>::InputImageType *
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetInputImageDirection()
{
  return static_cast<const InputImageType *>
           (this->GetInput(1));
}

/**
 * Set/Get PixelSuppressionyDirectionImageFilter parameters
 */
template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetPixelSuppressionRadius(SizeType Radius)
{
  m_PixelSuppression->SetRadius(Radius);
}

template <class TInputImage, class TOutputImage>
const typename ExtractSegmentsImageFilter<TInputImage, TOutputImage>::SizeType
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetPixelSuppressionRadius()
{
  return (m_PixelSuppression->GetRadius());
}

template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetPixelSuppressionAngularBeam(float AngularBeam)
{
  m_PixelSuppression->SetAngularBeam(AngularBeam);
}

template <class TInputImage, class TOutputImage>
float
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetPixelSuppressionAngularBeam()
{
  return (m_PixelSuppression->GetAngularBeam());
}

/**
 * Set/Get LocalHoughFilter parameters
 */
template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetLocalHoughRadius(SizeType Radius)
{
  m_LocalHough->SetRadius(Radius);
}

template <class TInputImage, class TOutputImage>
const typename ExtractSegmentsImageFilter<TInputImage, TOutputImage>::SizeType
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetLocalHoughRadius()
{
  return (m_LocalHough->GetRadius());
}

template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetLocalHoughNumberOfLines(unsigned int NumberOfLines)
{
  m_LocalHough->SetNumberOfLines(NumberOfLines);
}

template <class TInputImage, class TOutputImage>
unsigned int
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetLocalHoughNumberOfLines()
{
  return (m_LocalHough->GetNumberOfLines());
}

template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetLocalHoughDiscRadius(float DiscRadius)
{
  m_LocalHough->SetDiscRadius(DiscRadius);
}

template <class TInputImage, class TOutputImage>
float
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetLocalHoughDiscRadius()
{
  return (m_LocalHough->GetDiscRadius());
}

template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetLocalHoughVariance(float Variance)
{
  m_LocalHough->SetVariance(Variance);
}

template <class TInputImage, class TOutputImage>
float
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetLocalHoughVariance()
{
  return (m_LocalHough->GetVariance());
}

/**
 * Set/Get FillGapsFilter parameters
 */
template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetFillGapsRadius(float Radius)
{
  m_FillGaps->SetRadius(Radius);
}

template <class TInputImage, class TOutputImage>
float
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetFillGapsRadius()
{
  return (m_FillGaps->GetRadius());
}

template <class TInputImage, class TOutputImage>
void ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::SetFillGapsAngularBeam(float AngularBeam)
{
  m_FillGaps->SetAngularBeam(AngularBeam);
}

template <class TInputImage, class TOutputImage>
float
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GetFillGapsAngularBeam()
{
  return (m_FillGaps->GetAngularBeam());
}

template <class TInputImage, class TOutputImage>
void
ExtractSegmentsImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  m_PixelSuppression->SetInputImage(this->GetInputImage());
  m_PixelSuppression->SetInputImageDirection(this->GetInputImageDirection());

  m_Rescaler->SetInput(m_PixelSuppression->GetOutput());

  m_LocalHough->SetInput(m_Rescaler->GetOutput());

  m_FillGaps->SetInput (m_LocalHough->GetOutput());

  m_DrawLineList->SetInput(this->GetInputImage());
  m_DrawLineList->SetInputLineSpatialObjectList(m_FillGaps->GetOutput());
  m_DrawLineList->SetValue(m_LineValue);

  m_DrawLineList->GraftOutput(this->GetOutput());
  m_DrawLineList->Update();
  this->GraftOutput(m_DrawLineList->GetOutput());

}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
ExtractSegmentsImageFilter<TInputImage, TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  /*  os << indent << "Length: " << m_LengthLine << std::endl;
    os << indent << "Width: " << m_WidthLine << std::endl; */

}

} // end namespace otb

#endif
