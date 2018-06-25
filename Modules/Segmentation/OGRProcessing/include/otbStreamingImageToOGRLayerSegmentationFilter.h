/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbStreamingImageToOGRLayerSegmentationFilter_h
#define otbStreamingImageToOGRLayerSegmentationFilter_h

#include "itkExtractImageFilter.h"

#include "otbPersistentFilterStreamingDecorator.h"
#include "otbPersistentImageToOGRLayerFilter.h"
#include "otbRelabelComponentImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "otbLabeledOutputAccessor.h"

#include "otbMeanShiftSmoothingImageFilter.h"

namespace otb
{

/**
 * \class LabeledOutputAccessor
 * \brief Specialized class to get the index of the labeled output image in mean shift filter (new version).
 *
 * \ingroup OTBOGRProcessing
 */
template <class TInputImage, class TOutputImage, class TOutputImage2, class TKernelType>
class LabeledOutputAccessor<MeanShiftSmoothingImageFilter<TInputImage, TOutputImage, TOutputImage2, TKernelType> >
{
   public:
      typedef typename MeanShiftSmoothingImageFilter<TInputImage, TOutputImage, TOutputImage2, TKernelType>::OutputLabelImageType    LabelImageType;
      itkStaticConstMacro(LabeledOutputIndex, unsigned int, 0);
};

/** \class PersistentStreamingLabelImageToOGRDataFilter
 * \brief This filter is a framework for large scale segmentation.
 * For a detailed description @see StreamingImageToOGRLayerSegmentationFilter
 *
 * \ingroup OTBOGRProcessing
 */
template <class TImageType,  class TSegmentationFilter>
class PersistentImageToOGRLayerSegmentationFilter
  : public otb::PersistentImageToOGRLayerFilter<TImageType>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageToOGRLayerSegmentationFilter      Self;
  typedef PersistentImageToOGRLayerFilter<TImageType>      Superclass;
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
  itkTypeMacro(PersistentImageToOGRLayerSegmentationFilter, PersistentImageToOGRLayerFilter);

  /** Return a pointer to the segmentation filter used. */
  itkGetObjectMacro(SegmentationFilter, SegmentationFilterType);

  itkSetStringMacro(FieldName);
  itkGetStringMacro(FieldName);

  /** Set the first Label value (default is 1). Incremental step is 1.*/
  void SetStartLabel(const LabelPixelType & label)
  {
     m_StartLabel = label;
     m_TileMaxLabel = label;
  }
  /** Return the first label value*/
  itkGetMacro(StartLabel, LabelPixelType);

  /**
   * Set the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  itkSetMacro(Use8Connected, bool);
  /**
   * Get the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  itkGetMacro(Use8Connected, bool);

  /** Set the option for filtering small objects. Default to false. */
  itkSetMacro(FilterSmallObject, bool);
  /** Return the value of filter small objects option.*/
  itkGetMacro(FilterSmallObject, bool);

  /** Set the minimum object size (in pixels) in case FilterSmallObject option is true.*/
  itkSetMacro(MinimumObjectSize, unsigned int);
  /** Get the minimum object size.*/
  itkGetMacro(MinimumObjectSize, unsigned int);

  /** Option for simplifying geometries. Default to false.*/
  itkSetMacro(Simplify, bool);
  itkGetMacro(Simplify, bool);

  /** Set the tolerance value for simplifying geometries.
   * \sa \c OGRGeometry::Simplify() \c OGRGeometry::SimplifyPreserveTopology()
   */
  itkSetMacro(SimplificationTolerance, double);
  /** Get the tolerance value for simplifying geometries.
   * \sa \c OGRGeometry::Simplify() \c OGRGeometry::SimplifyPreserveTopology()
   */
  itkGetMacro(SimplificationTolerance, double);

  /** Set/Get the input mask image.
   * All pixels in the mask with a value of 0 will not be considered
   * suitable for vectorization.
   */
  virtual void SetInputMask(const LabelImageType *mask);
  virtual const LabelImageType * GetInputMask(void);

protected:
  PersistentImageToOGRLayerSegmentationFilter();

  ~PersistentImageToOGRLayerSegmentationFilter() override;


private:
  PersistentImageToOGRLayerSegmentationFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  OGRDataSourcePointerType ProcessTile() override;


  int m_TileMaxLabel;
  LabelPixelType m_StartLabel;
  typename SegmentationFilterType::Pointer m_SegmentationFilter;

  std::string m_FieldName;

  unsigned int m_TileNumber;
  bool m_Use8Connected;
  bool m_FilterSmallObject;
  unsigned int m_MinimumObjectSize;
  bool m_Simplify;
  double m_SimplificationTolerance;


};

/** \class StreamingImageToOGRLayerSegmentationFilter
 * \brief This filter is a framework for large scale segmentation.
 * It is a persistent filter that process the input image tile by tile.
 * This filter is templated over the segmentation filter. This later is used to segment each tile of the input image.
 * Each segmentation result (for each tile) is then vectorized using \c LabelImageToOGRDataSourceFilter
 * (based on \c GDALPolygonize()).
 * The output \c OGRDataSource of the \c LabelImageToOGRDataSourceFilter is a "memory" DataSource
 * (ie all features of a tile are kept in memory). From here some optional processing can be done,
 * depending on input parameters :
 * - Simplify option : If set to true, the SimplificationTolerance parameter is used to simplify all geometries,
 * using the \c OGRGeometry::Simplify() method, based on Douglas-Peuker algorithm.
 * - FilterSmallObject option : if set to true, polygons with a size less than MinimumObjectSize (in pixels)
 * are discarded.
 * Finally all features contained in the "memory" DataSource are copied into the input Layer,
 * in the layer specified with the \c SetLayerName() method.
 *
 * \note The Use8Connected parameter can be turn on and it will be used in \c GDALPolygonize(). But be carreful, it
 * can create cross polygons !
 * \note The input mask can be used to exclude pixels from vectorization process.
 * All pixels with a value of 0 in the input mask image will not be suitable for vectorization.
 *
 * \ingroup OTBOGRProcessing
 */
template <class TImageType,  class TSegmentationFilter>
class ITK_EXPORT StreamingImageToOGRLayerSegmentationFilter :
public PersistentFilterStreamingDecorator<PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter> >
{

public:
  /** Standard Self typedef */
  typedef StreamingImageToOGRLayerSegmentationFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentImageToOGRLayerSegmentationFilter<TImageType, TSegmentationFilter> >   Superclass;
  typedef itk::SmartPointer<Self>                                                    Pointer;
  typedef itk::SmartPointer<const Self>                                              ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingImageToOGRLayerSegmentationFilter, PersistentFilterStreamingDecorator);

  typedef TSegmentationFilter                      SegmentationFilterType;
  typedef TImageType                               InputImageType;
  typedef typename PersistentImageToOGRLayerSegmentationFilter<TImageType,
               TSegmentationFilter>::LabelPixelType                           LabelPixelType;
  typedef typename PersistentImageToOGRLayerSegmentationFilter<TImageType,
               TSegmentationFilter>::LabelImageType                           LabelImageType;
  typedef typename PersistentImageToOGRLayerSegmentationFilter<TImageType,
               TSegmentationFilter>::OGRDataSourcePointerType                 OGRDataSourcePointerType;
  typedef typename PersistentImageToOGRLayerSegmentationFilter<TImageType,
               TSegmentationFilter>::OGRLayerType                             OGRLayerType;

  typedef typename InputImageType::SizeType                                   SizeType;

  /** Set the input image. */
  using Superclass::SetInput;
  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }
  /** Set/Get the input mask image.
   * All pixels in the mask with a value of 0 will not be considered
   * suitable for vectorization.
   */
  void SetInputMask(LabelImageType * mask)
  {
    this->GetFilter()->SetInputMask(mask);
  }
  const LabelImageType * GetInputMask()
  {
    return this->GetFilter()->GetInputMask();
  }
  /** Set the \c ogr::Layer in which the layer LayerName will be created. */
  void SetOGRLayer( const OGRLayerType & ogrLayer )
  {
    this->GetFilter()->SetOGRLayer(ogrLayer);
  }

  void SetFieldName( const std::string & fieldName )
  {
    this->GetFilter()->SetFieldName(fieldName);
  }

  const std::string & GetFieldName() const
  {
    return this->GetFilter()->GetFieldName();
  }


  SegmentationFilterType * GetSegmentationFilter()
  {
     return this->GetFilter()->GetSegmentationFilter();
  }

  /** Set the first Label value (default is 1). Incremental step is 1.*/
  void SetStartLabel(const LabelPixelType & label)
  {
     this->GetFilter()->SetStartLabel(label);
  }
  /** Return the first label value*/
  const LabelPixelType & GetStartLabel()
  {
     return this->GetFilter()->GetStartLabel();
  }

  /** Retrieve the actual streamsize used */
  SizeType GetStreamSize()
  {
    return this->GetFilter()->GetStreamSize();
  }

  void Initialize()
  {
     this->GetFilter()->Initialize();
  }
  /**
   * Set the value of 8-connected neighborhood option used in \c LabelImageToOGRDataSourceFilter
   */
  void SetUse8Connected(bool flag)
  {
     this->GetFilter()->SetUse8Connected(flag);
  }

  bool GetUse8Connected()
  {
     return this->GetFilter()->GetUse8Connected();
  }
  /** Set the option for filtering small objects. Default to false. */
  void SetFilterSmallObject(bool flag)
  {
     this->GetFilter()->SetFilterSmallObject(flag);
  }

  bool GetFilterSmallObject()
  {
     return this->GetFilter()->GetFilterSmallObject();
  }
  /** Set the minimum object size (in pixels) in case FilterSmallObject option is true.*/
  void SetMinimumObjectSize(const unsigned int & size)
  {
     this->GetFilter()->SetMinimumObjectSize(size);
  }

  unsigned int GetMinimumObjectSize()
  {
     return this->GetFilter()->GetMinimumObjectSize();
  }
  /** Option for simplifying geometries. Default to false.*/
  void SetSimplify(bool flag)
  {
     this->GetFilter()->SetSimplify(flag);
  }

  bool GetSimplify()
  {
     return this->GetFilter()->GetSimplify();
  }
  /** Set the tolerance value for simplifying geometries.
   * \sa \c OGRGeometry::Simplify() \c OGRGeometry::SimplifyPreserveTopology()
   */
  void SetSimplificationTolerance(const double & tol)
  {
     this->GetFilter()->SetSimplificationTolerance(tol);
  }

  double GetSimplificationTolerance()
  {
     return this->GetFilter()->GetSimplificationTolerance();
  }

protected:
  /** Constructor */
  StreamingImageToOGRLayerSegmentationFilter() {}
  /** Destructor */
  ~StreamingImageToOGRLayerSegmentationFilter() override {}

private:
  StreamingImageToOGRLayerSegmentationFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};


}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageToOGRLayerSegmentationFilter.hxx"
#endif

#endif
