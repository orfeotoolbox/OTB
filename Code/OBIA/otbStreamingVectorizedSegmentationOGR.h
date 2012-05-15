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

#include "itkExtractImageFilter.h"

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbPersistentImageToOGRDataFilter.h"
#include "otbRelabelComponentImageFilter.h"
#include "itkMultiplyImageFilter.h"

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
  typedef PersistentStreamingLabelImageToOGRDataFilter     Self;
  typedef PersistentImageToOGRDataFilter<TImageType>       Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;

  typedef typename Superclass::InputImageType              InputImageType;
  typedef typename Superclass::InputImagePointer           InputImagePointerType;

  typedef TSegmentationFilter                              SegmentationFilterType;
  typedef typename LabeledOutputAccessor<SegmentationFilterType>::LabelImageType  LabelImageType;
  typedef typename LabelImageType::PixelType                                      LabelPixelType;
  
  typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType>          LabelImageToOGRDataSourceFilterType;
  typedef typename Superclass::OGRDataSourceType                        OGRDataSourceType;
  typedef typename Superclass::OGRDataSourcePointerType                 OGRDataSourcePointerType;
  typedef typename Superclass::OGRLayerType                             OGRLayerType;
  
  typedef RelabelComponentImageFilter<LabelImageType,LabelImageType>    RelabelComponentImageFilterType;
  typedef itk::MultiplyImageFilter<LabelImageType,LabelImageType,LabelImageType>  MultiplyImageFilterType;

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
  
  itkSetMacro(FilterSmallObject, bool);
  itkGetMacro(FilterSmallObject, bool);
  
  itkSetMacro(MinimumObjectSize, unsigned int);
  itkGetMacro(MinimumObjectSize, unsigned int);
  
  itkSetMacro(Simplify, bool);
  itkGetMacro(Simplify, bool);
  
  itkSetMacro(SimplificationTolerance, double);
  itkGetMacro(SimplificationTolerance, double);
  
  virtual void SetInputMask(const LabelImageType *mask);
  virtual const LabelImageType * GetInputMask(void);

protected:
  PersistentStreamingLabelImageToOGRDataFilter();

  virtual ~PersistentStreamingLabelImageToOGRDataFilter();

  void GenerateInputRequestedRegion();

private:
  PersistentStreamingLabelImageToOGRDataFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourcePointerType ProcessTile();
  
  //std::string m_FieldName;
  int m_TileMaxLabel;
  LabelPixelType m_StartLabel;
  typename SegmentationFilterType::Pointer m_SegmentationFilter;
  
  unsigned int m_TileNumber;
  bool m_Use8Connected;
  bool m_FilterSmallObject;
  unsigned int m_MinimumObjectSize;
  bool m_Simplify;
  double m_SimplificationTolerance;
  
  
};

template <class TImageType,  class TSegmentationFilter>
class ITK_EXPORT StreamingVectorizedSegmentationOGR :
public PersistentFilterStreamingDecorator<PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter> >
{

public:
  /** Standard Self typedef */
  typedef StreamingVectorizedSegmentationOGR Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentStreamingLabelImageToOGRDataFilter<TImageType, TSegmentationFilter> >   Superclass;
  typedef itk::SmartPointer<Self>                                                    Pointer;
  typedef itk::SmartPointer<const Self>                                              ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingVectorizedSegmentationOGR, PersistentFilterStreamingDecorator);

  typedef TSegmentationFilter                      SegmentationFilterType;
  typedef TImageType                               InputImageType;
  typedef typename PersistentStreamingLabelImageToOGRDataFilter<TImageType,
               TSegmentationFilter>::LabelPixelType                           LabelPixelType;
  typedef typename PersistentStreamingLabelImageToOGRDataFilter<TImageType,
               TSegmentationFilter>::LabelImageType                           LabelImageType;
  typedef typename PersistentStreamingLabelImageToOGRDataFilter<TImageType,
               TSegmentationFilter>::OGRDataSourcePointerType                 OGRDataSourcePointerType;

  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }
  
  void SetInputMask(LabelImageType * mask)
  {
    this->GetFilter()->SetInputMask(mask);
  }
  const LabelImageType * GetInputMask()
  {
    return this->GetFilter()->GetInputMask();
  }
  
  void SetOGRDataSource( OGRDataSourcePointerType ogrDS )
  {
    this->GetFilter()->SetOGRDataSource(ogrDS);
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
  
  void SetLayerName(const std::string & fileName)
  {
     this->GetFilter()->SetLayerName(fileName);
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
  
  void SetFilterSmallObject(bool flag)
  {
     this->GetFilter()->SetFilterSmallObject(flag);
  }
  
  const bool GetFilterSmallObject()
  {
     return this->GetFilter()->GetFilterSmallObject();
  }
  
  void SetMinimumObjectSize(const unsigned int & size)
  {
     this->GetFilter()->SetMinimumObjectSize(size);
  }
  
  const unsigned int GetMinimumObjectSize()
  {
     return this->GetFilter()->GetMinimumObjectSize();
  }
  
  void SetSimplify(bool flag)
  {
     this->GetFilter()->SetSimplify(flag);
  }
  
  const bool GetSimplify()
  {
     return this->GetFilter()->GetSimplify();
  }
  
  void SetSimplificationTolerance(const double & tol)
  {
     this->GetFilter()->SetSimplificationTolerance(tol);
  }
  
  const double GetSimplificationTolerance()
  {
     return this->GetFilter()->GetSimplificationTolerance();
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
