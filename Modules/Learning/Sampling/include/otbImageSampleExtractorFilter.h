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
#ifndef otbImageSampleExtractorFilter_h
#define otbImageSampleExtractorFilter_h

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbImage.h"

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
  public PersistentSamplingFilterBase<TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentImageSampleExtractorFilter            Self;
  typedef PersistentSamplingFilterBase<TInputImage>     Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::IndexType              IndexType;
  typedef typename InputImageType::PixelType              PixelType;
  typedef typename InputImageType::InternalPixelType      InternalPixelType;

  typedef ogr::DataSource                                 OGRDataType;
  typedef ogr::DataSource::Pointer                        OGRDataPointer;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentImageSampleExtractorFilter, PersistentSamplingFilterBase);

  /** Set the output samples OGR container
   * (shall be equal to the input container for an 'update' mode) */
  void SetOutputSamples(ogr::DataSource* data);

  /** Get the output samples OGR container */
  ogr::DataSource* GetOutputSamples();

  virtual void Synthetize(void){}

  /** Reset method called before starting the streaming*/
  virtual void Reset(void);
  
  itkSetMacro(SampleFieldPrefix, std::string);
  itkGetMacro(SampleFieldPrefix, std::string);

  /** Directly set the output field names (the prefix won't be used) */
  void SetSampleFieldNames(std::vector<std::string> &names);

  /** Get the sample names */
  const std::vector<std::string> & GetSampleFieldNames();

protected:
  /** Constructor */
  PersistentImageSampleExtractorFilter();
  /** Destructor */
  virtual ~PersistentImageSampleExtractorFilter() {}

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  /** process only points */
  virtual void ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid);

private:
  PersistentImageSampleExtractorFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Initialize fields to store extracted values (Real type) */
  void InitializeFields();

  /** Prefix to generate field names for each input channel
   *  (ignored if the field names are given directly) */
  std::string m_SampleFieldPrefix;

  /** List of field names for each component */
  std::vector<std::string> m_SampleFieldNames;
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
  
  void SetOutputSamples(OGRDataType::Pointer data);
  const otb::ogr::DataSource* GetOutputSamples();

  void SetOutputFieldPrefix(const std::string &key);
  std::string GetOutputFieldPrefix();

  /** Set the output field names */
  void SetOutputFieldNames(std::vector<std::string> &names);

  /** Get the output field names */
  const std::vector<std::string> & GetOutputFieldNames();
  
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
