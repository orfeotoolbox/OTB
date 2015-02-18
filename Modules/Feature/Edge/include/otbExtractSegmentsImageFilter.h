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
#ifndef __otbExtractSegmentsImageFilter_h
#define __otbExtractSegmentsImageFilter_h


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
  virtual ~ExtractSegmentsImageFilter() {}

  typedef PixelSuppressionByDirectionImageFilter<InputImageType, PSOutputImageType> PixelSuppressionType;
  typedef LocalHoughFilter<InputImageType>                                          LocalHoughType;
  typedef FillGapsFilter                                                            FillGapsType;
  typedef DrawLineSpatialObjectListFilter<InputImageType, OutputImageType>          DrawLineListType;
  typedef  itk::RescaleIntensityImageFilter<TInputImage, TInputImage>               RescaleType;
  virtual void GenerateData();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ExtractSegmentsImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename OutputImageType::PixelType m_LineValue;

  typename PixelSuppressionType::Pointer m_PixelSuppression;
  typename LocalHoughType::Pointer m_LocalHough;
  typename FillGapsType::Pointer m_FillGaps;
  typename DrawLineListType::Pointer m_DrawLineList;
  typename RescaleType::Pointer m_Rescaler;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractSegmentsImageFilter.txx"
#endif

#endif
