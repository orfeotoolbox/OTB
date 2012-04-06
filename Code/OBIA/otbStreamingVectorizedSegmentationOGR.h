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
#ifndef __otbStreamingVectorizedSegmentationOGR_h
#define __otbStreamingVectorizedSegmentationOGR_h

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
#include "otbPersistentImageToOGRDataFilter.h"

#include "otbMeanShiftImageFilter.h"
#include "otbMeanShiftImageFilter2.h"
#include "otbMeanShiftVectorImageFilter.h"
#include "itkMacro.h"


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

/**
 * \class LabeledOutputAccessor
 * \brief Specialized class to get the index of the labeled output image in mean shift vector image filter.
 */
template <class TInputImage, class TOutputImage, class TLabeledImage>
class LabeledOutputAccessor<MeanShiftVectorImageFilter<TInputImage, TOutputImage, TLabeledImage> >
{
   public:
      typedef typename MeanShiftVectorImageFilter<TInputImage, TOutputImage, TLabeledImage>::LabeledOutputType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 2);
};

/**
 * \class LabeledOutputAccessor
 * \brief Specialized class to get the index of the labeled output image in mean shift filter (new version).
 */
template <class TInputImage, class TOutputImage, class TOutputImage2, class TKernelType>
class LabeledOutputAccessor<MeanShiftImageFilter2<TInputImage, TOutputImage, TOutputImage2, TKernelType> >
{
   public:
      typedef typename MeanShiftImageFilter2<TInputImage, TOutputImage, TOutputImage2, TKernelType>::LabeledOutputType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 0);
};

/** \class PersistentStreamingLabelImageToOGRDataFilter
 *  \brief this class uses GDALPolygonize method to transform a Label image into a VectorData.
 *
 *  This filter is a generic PersistentImageFilter, which encapsulate
 *  the LabelImageToVectorData filter.
 *
 * \sa PersistentImageToOGRDataFilter
 *
 */
template <class TImageType,  class TSegmentationFilter>
class PersistentStreamingLabelImageToOGRDataFilter
  : public otb::PersistentImageToOGRDataFilter<TImageType>
{
public:
  /** Standard Self typedef */
  typedef PersistentStreamingLabelImageToOGRDataFilter                     Self;
  typedef PersistentImageToOGRDataFilter<TImageType>                       Superclass;
  typedef itk::SmartPointer<Self>                                          Pointer;
  typedef itk::SmartPointer<const Self>                                    ConstPointer;

  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointerType;

  
  typedef TSegmentationFilter                              SegmentationFilterType;
  typedef typename LabeledOutputAccessor<SegmentationFilterType>::LabelImageType  LabelImageType;
  typedef typename LabelImageType::PixelType                                      LabelPixelType;
  
  typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType>          LabelImageToOGRDataSourceFilterType;
  typedef typename LabelImageToOGRDataSourceFilterType::OGRDataSourceObjectType  OGRDataSourceObjectType;
  typedef typename OGRDataSourceObjectType::Pointer OGRDataSourceObjectPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentStreamingLabelImageToOGRDataFilter, PersistentImageToOGRDataFilter);
  
  itkGetObjectMacro(SegmentationFilter, SegmentationFilterType);
  
  void SetStartLabel(const LabelPixelType & label)
  {
     m_StartLabel = label;
     m_TileMaxLabel = label;
  }
  itkGetMacro(StartLabel, LabelPixelType);
  
  itkSetMacro(Use8Connected, bool);
  itkGetMacro(Use8Connected, bool);

protected:
  PersistentStreamingLabelImageToOGRDataFilter();

  virtual ~PersistentStreamingLabelImageToOGRDataFilter();

  void GenerateInputRequestedRegion();

private:
  PersistentStreamingLabelImageToOGRDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourceObjectPointerType ProcessTile();
  
  //std::string m_FieldName;
  LabelPixelType m_TileMaxLabel;
  LabelPixelType m_StartLabel;
  typename SegmentationFilterType::Pointer m_SegmentationFilter;
  
  unsigned int m_TileNumber;
  bool m_Use8Connected;
  
  
};

template <class TImageType,  class TSegmentationFilter>
class ITK_EXPORT StreamingVectorizedSegmentationOGR :
public PersistentFilterStreamingDecorator<PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter> >
{

public:
  /** Standard Self typedef */
  typedef StreamingVectorizedSegmentationOGR Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingVectorizedSegmentationOGR, PersistentFilterStreamingDecorator);

  typedef TSegmentationFilter                      SegmentationFilterType;
  typedef TImageType                               InputImageType;
  typedef typename PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter>::LabelPixelType LabelPixelType;

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
  
  void SetFieldName(const std::string & field)
  {
     this->GetFilter()->SetFieldName(field);
  }
  
  const std::string & GetFieldName()
  {
     return this->GetFilter()->GetFieldName();
  }
  
  void SetStartLabel(const LabelPixelType & label)
  {
     this->GetFilter()->SetStartLabel(label);
  }
  
  const LabelPixelType & GetStartLabel()
  {
     return this->GetFilter()->GetStartLabel();
  }
  
  void SetFileName(const std::string & fileName)
  {
     this->GetFilter()->SetFileName(fileName);
  }
  
  void Initialize()
  {
     this->GetFilter()->Initialize();
  }
  
  void SetUse8Connected(bool flag)
  {
     this->GetFilter()->SetUse8Connected(flag);
  }
  
  const bool GetUse8Connected()
  {
     return this->GetFilter()->GetUse8Connected();
  }
  
protected:
  /** Constructor */
  StreamingVectorizedSegmentationOGR() {}
  /** Destructor */
  virtual ~StreamingVectorizedSegmentationOGR() {}

private:
  StreamingVectorizedSegmentationOGR(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};


}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingVectorizedSegmentationOGR.txx"
#endif

#endif
