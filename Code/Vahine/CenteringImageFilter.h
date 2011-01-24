/**
 * CenteringImageFilter.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINECENTERINGIMAGE_H
#define __VAHINECENTERINGIMAGE_H

#include "VahineMacro.h"

#include "itkImageToImageFilter.h"
#include "itkStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"

namespace itk {

template<class TInputImage,class TOutputImage> class ITK_EXPORT CenteringImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage> {
public:
  /** Extract dimension from input and output image. */
  //itkStaticConstMacro(InputImageDimension, unsigned int,
  // TInputImage::ImageDimension);
  //itkStaticConstMacro(OutputImageDimension, unsigned int,
  // TOutputImage::ImageDimension);

  /** Standard Self typedef */
  typedef CenteringImageFilter                          Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(CenteringImageFilter, ImageToImageFilter);

  /** Image related typedefs. */
  typedef typename TInputImage::Pointer  InputImagePointer;
  typedef typename TOutputImage::Pointer OutputImagePointer;

  /** NormalizeImageFilter must call modified on its internal filters */
  virtual void Modified() const;

protected:
  CenteringImageFilter();

  /** GenerateData. */
  void  GenerateData();

  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion();

private:
  CenteringImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&);       //purposely not implemented

  typename StatisticsImageFilter<TInputImage>::Pointer                        m_StatisticsFilter;
  typename ShiftScaleImageFilter<TInputImage,TOutputImage>::Pointer   m_ShiftScaleFilter;
};     // end of class

} // end namespace itk

#include "CenteringImageFilter.txx"

#endif