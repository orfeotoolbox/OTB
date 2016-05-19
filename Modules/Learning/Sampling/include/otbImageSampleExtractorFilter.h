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

#include "otbPersistentImageToOGRDataFilter.h"
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
  public PersistentImageToOGRDataFilter<TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageSampleExtractorFilter            Self;
  typedef PersistentImageToOGRDataFilter<TInputImage>     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::IndexType              IndexType;
  typedef typename InputImageType::PixelType              PixelType;
  typedef typename InputImageType::InternalPixelType      InternalPixelType;

  typedef typename Superclass::OGRDataSourceType          OGRDataSourceType;
  typedef typename Superclass::OGRDataSourcePointerType   OGRDataSourcePointerType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentImageSampleExtractorFilter, PersistentImageToOGRDataFilter);

  void SetSamplePositions(const otb::ogr::DataSource* vector);
  const otb::ogr::DataSource* GetSamplePositions();

  virtual void Synthetize(void);

  /** Reset method called before starting the streaming*/
  virtual void Reset(void);
  
  itkSetMacro(SampleFieldPrefix, std::string);
  itkGetMacro(SampleFieldPrefix, std::string);
  
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int);

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  //virtual itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx);
  //using Superclass::MakeOutput;

protected:
  /** Constructor */
  PersistentImageSampleExtractorFilter();
  /** Destructor */
  virtual ~PersistentImageSampleExtractorFilter() {}

  // virtual void GenerateInputRequestedRegion();

  //virtual void GenerateData();

private:
  PersistentImageSampleExtractorFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  virtual OGRDataSourcePointerType ProcessTile();

  /** Apply a spatial filtering on the OGRDataSource corresponding to the processed tile */
  void ApplyPolygonsSpatialFilter();

  /** Initialize fields to store extracted values (Real type) */
  void InitializeFields(ogr::Layer &layer, unsigned int size);

  /** Layer to use in the shape file, default to 0 */
  int m_LayerIndex;
  
  /** Prefix to generate field names for each input channel */
  std::string m_SampleFieldPrefix;
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

  void SetSamplePositions(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetSamplePositions();
  
  void SetOutputOGRData(OGRDataType::Pointer data);
  const otb::ogr::DataSource* GetOutputOGRData();

  void SetOutputFieldPrefix(const std::string &key);
  std::string GetOutputFieldPrefix();
  
  void SetLayerIndex(int index);
  int GetLayerIndex();

  void SetClassFieldName(const std::string &name);
  std::string GetClassFieldName(void);

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
