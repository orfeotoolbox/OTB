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
#ifndef __otbLabelizeImageFilterBase_h
#define __otbLabelizeImageFilterBase_h

#include "itkImageToImageFilter.h"
#include "itkAddImageFilter.h"

#include "otbThresholdImageToPointSetFilter.h"
#include "otbMultiplyByScalarImageFilter.h"

namespace otb
{
/** \class LabelizeImageFilterBase
 *  \brief Base class for filter labelizing image region based on segmentation.
 *
 */
template <class TInputImage, class TOutputImage, class TFilter>
class ITK_EXPORT LabelizeImageFilterBase
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** typedef for standard classes. */
  typedef LabelizeImageFilterBase                            Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef typename TInputImage::PixelType  InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

  typedef typename TInputImage::IndexType IndexType;

  typedef itk::PointSet<InputPixelType, 2> PointSetType;
  typedef typename PointSetType::Pointer   PointSetPointerType;

  typedef TFilter                   RegionGrowingFilterType;
  typedef typename TFilter::Pointer RegionGrowingFilterPointerType;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(LabelizeImageFilterBase, ImageToImageFilter);

  /** Get lower threshold */
  itkGetMacro(LowerThreshold, InputPixelType);

  /** Set lower threshold */
  itkSetMacro(LowerThreshold, InputPixelType);

  /** Get upper threshold */
  itkGetMacro(UpperThreshold, InputPixelType);

  /** Set upper threshold */
  itkSetMacro(UpperThreshold, InputPixelType);

  /** Get Object count*/
  // only set after completion
//       itkGetConstReferenceMacro(ObjectCount, unsigned long);
  itkGetMacro(ObjectCount, unsigned long);

  /** Internal */
  typedef otb::ThresholdImageToPointSetFilter<InputImageType, PointSetType> ThresholdFilterType;
  typedef typename ThresholdFilterType::Pointer                             ThresholdFilterPointerType;

  typedef otb::MultiplyByScalarImageFilter<InputImageType, OutputImageType> MultiplyFilterType;
  typedef typename MultiplyFilterType::Pointer                              MultiplyFilterPointerType;

  typedef itk::AddImageFilter<OutputImageType, OutputImageType, OutputImageType> AddImageFilterType;
  typedef typename AddImageFilterType::Pointer                                   AddImageFilterPointerType;

protected:
  LabelizeImageFilterBase();
  virtual ~LabelizeImageFilterBase() {}
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Main computation method */
  virtual void GenerateData();

  /** Region growing */
  virtual void RegionGrowing(const IndexType itkNotUsed(indexSeed)) {}

  /** Instance region growing filter*/
  RegionGrowingFilterPointerType m_RegionGrowingFilter;

  /** Lower threshold */
  InputPixelType m_LowerThreshold;

  /** Upper threshold */
  InputPixelType m_UpperThreshold;

  /** Point set to store thresold results */
  PointSetPointerType m_PointSet;

  /** Multiply filter*/
  MultiplyFilterPointerType m_MultiplyFilter;

  /** Threshold point set filter */
  ThresholdFilterPointerType m_ThresholdPointSetFilter;

  /** Object counting */
  unsigned long m_ObjectCount;

private:
  LabelizeImageFilterBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

}; // end class LabelizeImageFilterBase

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelizeImageFilterBase.txx"
#endif

#endif
