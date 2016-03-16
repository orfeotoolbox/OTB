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
#ifndef __otbImageSampleExtractorFilter_h
#define __otbImageSampleExtractorFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"

namespace otb
{

/**
 * \class PersistentImageSampleExtractorFilter
 * 
 * \brief Persistent filter to extract sample values from an image
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage>
class ITK_EXPORT PersistentImageSampleExtractorFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageSampleExtractorFilter            Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::IndexType              IndexType;
  typedef typename InputImageType::PixelType              PixelType;
  typedef typename InputImageType::InternalPixelType      InternalPixelType;
  

  typedef otb::ogr::DataSource                            OGRDataType;
  typedef otb::ogr::DataSource::Pointer                   OGRDataPointer;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentImageSampleExtractorFilter, PersistentImageFilter);

  void SetInputOGRData(const otb::ogr::DataSource* vector);
  const otb::ogr::DataSource* GetInputOGRData();

  void SetOutputOGRData(const otb::ogr::DataSource* vector);
  const otb::ogr::DataSource* GetOutputOGRData();

  void Synthetize(void);

  /** Reset method called before starting the streaming*/
  void Reset(void);

  
  itkSetMacro(FieldPrefix, std::string);
  itkGetMacro(FieldPrefix, std::string);
  
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int);

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

protected:
  /** Constructor */
  PersistentImageSampleExtractorFilter();
  /** Destructor */
  virtual ~PersistentImageSampleExtractorFilter() {}

  // virtual void GenerateInputRequestedRegion();

  virtual void GenerateData();

private:
  PersistentImageSampleExtractorFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void ApplyPolygonsSpatialFilter();

  RegionType FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const;

  std::string m_FieldPrefix;

  // Layer to use in the shape file, default to 0
  int m_LayerIndex;
};

/**
 * \class ImageSampleExtractorFilter
 * 
 * \brief Extract sample values from an image into an OGRDataSource using a persistent filter
 * 
 * \sa PersistentImageSampleExtractorFilter
 *
 * \ingroup OTBSampling
 */
template<class TInputImage>
class ITK_EXPORT ImageSampleExtractorFilter :
  public PersistentFilterStreamingDecorator<PersistentImageSampleExtractorFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef ImageSampleExtractorFilter      Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentImageSampleExtractorFilter
      <TInputImage> >                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  
  typedef typename Superclass::FilterType             FilterType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageSampleExtractorFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetInputOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetInputOGRData();
  
  void SetOutputOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetOutputOGRData();

  void SetFieldPrefix(std::string &key);
  std::string GetFieldPrefix();
  
  void SetLayerIndex(int index);
  int GetLayerIndex();

protected:
  /** Constructor */
  ImageSampleExtractorFilter() {}
  /** Destructor */
  virtual ~ImageSampleExtractorFilter() {}

private:
  ImageSampleExtractorFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSampleExtractorFilter.txx"
#endif

#endif
