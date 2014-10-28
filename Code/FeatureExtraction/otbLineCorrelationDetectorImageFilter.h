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
#ifndef __otbLineCorrelationDetectorImageFilter_h
#define __otbLineCorrelationDetectorImageFilter_h

#include "itkLinearInterpolateImageFunction.h"
#include "otbLineDetectorImageFilterBase.h"

namespace otb
{
/**
 *\class LineCorrelationDetectorImageFilter
 * \brief To be documented
 *
 */

template <class TInputImage,
    class TOutputImage,
    class TOutputImageDirection = TOutputImage,
    class TInterpolator = itk::LinearInterpolateImageFunction<TInputImage> >
class ITK_EXPORT LineCorrelationDetectorImageFilter :  public LineDetectorImageFilterBase<TInputImage, TOutputImage,
      TOutputImageDirection,
      TInterpolator>
{
public:
  /**   Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef for the classes standards. */
  typedef LineCorrelationDetectorImageFilter                                                           Self;
  typedef LineDetectorImageFilterBase<TInputImage, TOutputImage, TOutputImageDirection, TInterpolator> Superclass;
  typedef itk::SmartPointer<Self>                                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                                ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(LineCorrelationDetectorImageFilter, LineDetectorImageFilterBase);

  typedef typename Superclass::InputImageType           InputImageType;
  typedef typename Superclass::OutputImageType          OutputImageType;
  typedef typename Superclass::OutputImageDirectionType OutputImageDirectionType;
  typedef typename Superclass::InterpolatorType         InterpolatorType;

  /** Typedefs to describe and access Interpolator */
  typedef typename InterpolatorType::Pointer      InterpolatorPointer;
  typedef typename InterpolatorType::CoordRepType CoordRepType;

  typedef typename InputImageType::PointType TPoint;

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

protected:
  LineCorrelationDetectorImageFilter();
  virtual ~LineCorrelationDetectorImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Compute the measure */
  virtual double ComputeMeasure(std::vector<double>* m1, std::vector<double>* m2, std::vector<double>* m3);

private:
  LineCorrelationDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineCorrelationDetectorImageFilter.txx"
#endif

#endif
