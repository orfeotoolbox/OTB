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
#ifndef __otbStreamingVectorizedSegmentation_h
#define __otbStreamingVectorizedSegmentation_h

#include <vector>

#include "itkImageRegion.h"
#include "otbVectorData.h"
#include "itkPreOrderTreeIterator.h"

#include "itkDataObject.h"
#include "itkDataObjectDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

#include "otbLabelImageToVectorDataFilter.h"
#include "itkExtractImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbPersistentImageToVectorDataFilter.h"

#include "otbMeanShiftImageFilter.h"


namespace otb
{

/**
 * \class LabeledOutputAccessor
 * \brief Accessor to the index of the labeled output image of the Template Filter.
 */
template <class TFilter>
class LabeledOutputAccessor
{
   public:
      typedef typename TFilter::OutputImageType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 0);
};

/**
 * \class LabeledOutputAccessor
 * \brief Specialized class to get the index of the labeled output image in mean shift filter.
 */
template <class TInputImage, class TOutputImage, class TLabeledImage, class TBufferConverter>
class LabeledOutputAccessor<MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledImage, TBufferConverter> >
{
   public:
      typedef typename MeanShiftImageFilter<TInputImage, TOutputImage, TLabeledImage, TBufferConverter>::LabeledOutputType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 2);
};


/** \class PersistentStreamingLabelImageToVectorDataFilter
 *  \brief this class uses GDALPolygonize method to transform a Label image into a VectorData.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate
 *  the LabelImageToVectorData filter.
 *
 * \sa PersistentImageToVectorDataFilter
 *
 */
template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
class PersistentStreamingLabelImageToVectorDataFilter
  : public otb::PersistentImageToVectorDataFilter<TImageType,TOutputVectorData >
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingLabelImageToVectorDataFilter                     Self;
  typedef PersistentImageToVectorDataFilter<TImageType, TOutputVectorData >   Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointerType;

  typedef typename Superclass::OutputVectorDataType        OutputVectorDataType;
  typedef typename Superclass::OutputVectorDataPointerType OutputVectorDataPointerType;
  
  typedef TSegmentationFilter                              SegmentationFilterType;
  typedef typename LabeledOutputAccessor<SegmentationFilterType>::LabelImageType  LabelImageType;
  
  typedef otb::LabelImageToVectorDataFilter<LabelImageType,
    typename OutputVectorDataType::PrecisionType>          LabelImageToVectorDataFilterType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingLabelImageToVectorDataFilter, PersistentImageToVectorDataFilter);
  
  itkGetObjectMacro(SegmentationFilter, SegmentationFilterType);

protected:
  PersistentStreamingLabelImageToVectorDataFilter();

  virtual ~PersistentStreamingLabelImageToVectorDataFilter();

  void GenerateInputRequestedRegion();

private:
  PersistentStreamingLabelImageToVectorDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OutputVectorDataPointerType ProcessTile();
  
  typename SegmentationFilterType::Pointer m_SegmentationFilter;
};

template <class TImageType, class TOutputVectorData, class TSegmentationFilter>
class ITK_EXPORT StreamingVectorizedSegmentation :
public PersistentFilterStreamingDecorator<PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter> >
{

public:
  /** Standard Self typedef */
  typedef StreamingVectorizedSegmentation Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentStreamingLabelImageToVectorDataFilter<TImageType, TOutputVectorData, TSegmentationFilter> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingVectorizedSegmentation, PersistentFilterStreamingDecorator);

  typedef TSegmentationFilter                      SegmentationFilterType;
  typedef TImageType                               InputImageType;
  typedef TOutputVectorData                        OutputVectorDataType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  SegmentationFilterType * GetSegmentationFilter()
  {
     return this->GetFilter()->GetSegmentationFilter();
  }
  
  const OutputVectorDataType * GetOutputVectorData()
  {
     return this->GetFilter()->GetOutputVectorData();
  }
  
protected:
  /** Constructor */
  StreamingVectorizedSegmentation() {}
  /** Destructor */
  virtual ~StreamingVectorizedSegmentation() {}

private:
  StreamingVectorizedSegmentation(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};


}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingVectorizedSegmentation.txx"
#endif

#endif
