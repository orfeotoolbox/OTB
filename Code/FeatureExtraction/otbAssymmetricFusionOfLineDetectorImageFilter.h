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
#ifndef __otbAssymmetricFusionOfLineDetectorImageFilter_h
#define __otbAssymmetricFusionOfLineDetectorImageFilter_h

#include "itkImageToImageFilter.h"

#include "otbLineRatioDetectorImageFilter.h"
#include "otbLineCorrelationDetectorImageFilter.h"
#include "otbAssociativeSymmetricalSumImageFilter.h"

namespace otb
{

/** \class AssymmetricFusionOfLineDetectorImageFilter
 *
 * This class implements a composite filter that combines three filters:
 * two filters of line detector ( a line detector by ratio and a line
 * detector by croos-correlation) and a filter that produces a fusion of
 * those two line detector filters.
 *
 */

template <class TInputImage,
    class TOutputImage,
    class TOutputImageDirection = TOutputImage,
    class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT AssymmetricFusionOfLineDetectorImageFilter :
  public LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator>
{
public:

  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  typedef AssymmetricFusionOfLineDetectorImageFilter                                                   Self;
  typedef LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator> Superclass;
  typedef itk::SmartPointer<Self>                                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                                ConstPointer;

  itkNewMacro(Self);

  itkTypeMacro(AssymmetricFusionOfLineDetectorImageFilter, LineDetectorImageFilterBase);

  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;

  typedef OutputImageType InputImageType1;
  typedef OutputImageType InputImageType2;

  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename InputImageType::SizeType  SizeType;

  typedef typename OutputImageType::PixelType OutputPixelType;

protected:
  AssymmetricFusionOfLineDetectorImageFilter();
  virtual ~AssymmetricFusionOfLineDetectorImageFilter() {}

  typedef otb::LineRatioDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType,
      InterpolatorType>        LineRatioType;
  typedef otb::LineCorrelationDetectorImageFilter<InputImageType, OutputImageType, OutputImageDirectionType,
      InterpolatorType>  LineCorrelationType;
  typedef otb::AssociativeSymmetricalSumImageFilter<InputImageType1, InputImageType2,
      OutputImageType> AssSymSumType;

  virtual void GenerateData();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AssymmetricFusionOfLineDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  typename LineRatioType::Pointer m_LineRatio;
  typename LineCorrelationType::Pointer m_LineCorrelation;
  typename AssSymSumType::Pointer m_AssSymSum;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAssymmetricFusionOfLineDetectorImageFilter.txx"
#endif

#endif
