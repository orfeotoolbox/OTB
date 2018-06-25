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

#ifndef otbExtractSegmentsImageFilter_h
#define otbExtractSegmentsImageFilter_h


#include "otbPixelSuppressionByDirectionImageFilter.h"
#include "otbLocalHoughFilter.h"
#include "otbFillGapsFilter.h"
#include "otbDrawLineSpatialObjectListFilter.h"
#include "itkRescaleIntensityImageFilter.h"


namespace otb
{

/** \class ExtractSegmentsImageFilter
 *
 * This class implements a composite filter that generate an image of segments
 * primitives. It combines four filters :
 *   - otb::PixelSuppressionByDirectionImageFilter
 *   - otb::LocalHoughFilter
 *   - otb::FillGapsFilter
 *   - otb::DrawLineSpatialObjectListFilter
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage,
    class TOutputImage>
class ITK_EXPORT ExtractSegmentsImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:

  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef TInputImage PSOutputImageType;

  typedef ExtractSegmentsImageFilter                               Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(ExtractSegmentsImageFilter, itk::ImageToImageFilter);

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

  /** Definition of the list of lines. */
  typedef LineSpatialObjectList LinesListType;

  /** Set/Get the radius of the region of the pixel suppression by direction image filter. */
  void SetPixelSuppressionRadius(SizeType Radius);
  const SizeType GetPixelSuppressionRadius();

  /** Set/Get Angular Accuracy on the direction of the central pixel for
      the pixel suppression by direction image filter. */
  void SetPixelSuppressionAngularBeam(float AngularBeam);
  float GetPixelSuppressionAngularBeam();

  /** Set/Get the radius used to define the region of local hough filter. */
  void SetLocalHoughRadius(SizeType Radius);
  const SizeType GetLocalHoughRadius();

  /** Set/Get the number of lines we are looking for in the local hough filter. */
  void SetLocalHoughNumberOfLines(unsigned int Radius);
  unsigned int GetLocalHoughNumberOfLines();

  /** Set/Get the radius of the disc to remove from the accumulator
   *  for each line found */
  void SetLocalHoughDiscRadius(float DiscRadius);
  float GetLocalHoughDiscRadius();

  /** Set/Get the variance of the gaussian bluring for the accumulator */
  void SetLocalHoughVariance(float Variance);
  float GetLocalHoughVariance();

  /** Set/Get the radius between two segments in the fill gaps filter. */
  void SetFillGapsRadius(float Radius);
  float GetFillGapsRadius();

  /** Set/Get Angular Beam between two segments in the fill gaps filter. */
  void SetFillGapsAngularBeam(float AngularBeam);
  float GetFillGapsAngularBeam();

  /** Set/Get the image input of this filter.  */
  void SetInputImage(const InputImageType *image);
  const InputImageType * GetInputImage();

  /** Set/Get the image direction of this filter.  */
  void SetInputImageDirection(const InputImageType *image);
  const InputImageType * GetInputImageDirection();

  /** Set/Get the value of the drawed line*/
  itkGetMacro(LineValue, typename OutputImageType::PixelType);
  itkSetMacro(LineValue, typename OutputImageType::PixelType);

protected:
  ExtractSegmentsImageFilter();
  ~ExtractSegmentsImageFilter() override {}

  typedef PixelSuppressionByDirectionImageFilter<InputImageType, PSOutputImageType> PixelSuppressionType;
  typedef LocalHoughFilter<InputImageType>                                          LocalHoughType;
  typedef FillGapsFilter                                                            FillGapsType;
  typedef DrawLineSpatialObjectListFilter<InputImageType, OutputImageType>          DrawLineListType;
  typedef  itk::RescaleIntensityImageFilter<TInputImage, TInputImage>               RescaleType;
  void GenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ExtractSegmentsImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  typename OutputImageType::PixelType m_LineValue;

  typename PixelSuppressionType::Pointer m_PixelSuppression;
  typename LocalHoughType::Pointer m_LocalHough;
  typename FillGapsType::Pointer m_FillGaps;
  typename DrawLineListType::Pointer m_DrawLineList;
  typename RescaleType::Pointer m_Rescaler;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractSegmentsImageFilter.hxx"
#endif

#endif
