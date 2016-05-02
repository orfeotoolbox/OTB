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
#ifndef __otbOGRDataToSamplePositionFilter_h
#define __otbOGRDataToSamplePositionFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
//#include "otbOGRDataResampler.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbSamplingRateCalculator.h"
#include "otbPeriodicSampler.h"
#include "otbImage.h"


namespace otb
{

/**
 * \class PersistentOGRDataToSamplePositionFilter
 * 
 * \brief Persistent filter to compute class statistics based on vectors
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage, class TSampler>
class ITK_EXPORT PersistentOGRDataToSamplePositionFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToSamplePositionFilter        Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;

  typedef TMaskImage                                      MaskImageType;
  typedef typename MaskImageType::Pointer                 MaskImagePointer;

  typedef otb::ogr::DataSource                            OGRDataType;
  typedef otb::ogr::DataSource::Pointer                   OGRDataPointer;

  typedef TSampler                                        SamplerType;
  typedef typename SamplerType::Pointer                   SamplerPointerType;
  typedef typename SamplerType::ParameterType             SamplerParameterType;
  typedef typename std::map
    <std::string, SamplerPointerType>                     SamplerMapType;

  /** Wrap output type as DataObject */

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToSamplePositionFilter, PersistentImageFilter);

  void SetOGRData(const otb::ogr::DataSource* vector);
  const otb::ogr::DataSource* GetOGRData();

  void SetMask(const TMaskImage* mask);
  const TMaskImage* GetMask();

  void Synthetize(void);

  /** Reset method called before starting the streaming*/
  void Reset(void);

  // TODO : prevent loading of data into output

  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);
  
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int); 

  SamplerMapType& GetSamplers(unsigned int level);
  
  void SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level);

  const otb::ogr::DataSource* GetOutputPositionContainer(unsigned int level) const;
  otb::ogr::DataSource* GetOutputPositionContainer(unsigned int level);

  unsigned int GetNumberOfLevels();
  
  void ClearOutputs();
  
  /** Set the OGR layer creation options */
  void SetOGRLayerCreationOptions(const std::vector<std::string> & options);
  
  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

protected:
  /** Constructor */
  PersistentOGRDataToSamplePositionFilter();
  /** Destructor */
  virtual ~PersistentOGRDataToSamplePositionFilter() {}

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateData();

private:
  PersistentOGRDataToSamplePositionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void ApplyPolygonsSpatialFilter();

  RegionType FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const;

  /** Process the current geometry using an image iterator */
  template <typename TIterator>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           TIterator& imgIt,
           const typename TIterator::ImageType *img);

  /** Recursive method to process geometries */
  template <typename TIterator>
  void AddGeometry(OGRGeometry *geom,
                   TIterator& imgIt,
                   const typename TIterator::ImageType *img,
                   unsigned long &fId,
                   std::string &className);

  void CallSamplers(const PointType &point,
                    const std::string &className);

  std::string m_FieldName;

  int m_FieldIndex;

  std::vector<SamplerMapType> m_Samplers;

  // Layer to use in the shape file, default to 0
  int m_LayerIndex;
  
  // (internal) name of the layer at position 'm_LayerIndex'
  std::string m_LayerName;

  std::vector<std::string> m_OGRLayerCreationOptions;

  std::vector<OGRDataPointer> m_InMemoryOutputs;
};

/**
 * \class OGRDataToSamplePositionFilter
 * 
 * \brief Computes class statistics based on vectors using a persistent filter
 * 
 * \sa PersistentOGRDataToSamplePositionFilter
 *
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage = otb::Image<unsigned char> , class TSampler = otb::PeriodicSampler >
class ITK_EXPORT OGRDataToSamplePositionFilter :
  public PersistentFilterStreamingDecorator<PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage,TSampler> >
{
public:
  /** Standard Self typedef */
  typedef OGRDataToSamplePositionFilter  Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentOGRDataToSamplePositionFilter
      <TInputImage,TMaskImage,TSampler> >          Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef TMaskImage                      MaskImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  
  typedef typename Superclass::FilterType             FilterType;

  typedef TSampler                                        SamplerType;
  typedef typename SamplerType::Pointer                   SamplerPointerType;
  typedef typename SamplerType::ParameterType             SamplerParameterType;
  typedef typename std::map
    <std::string, SamplerPointerType>                     SamplerMapType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(OGRDataToSamplePositionFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetOGRData();

  void SetMask(const TMaskImage* mask);
  const TMaskImage* GetMask();

  void SetFieldName(std::string key);
  std::string GetFieldName();
  
  void SetLayerIndex(int index);
  int GetLayerIndex();

  void SetSamplerParameters(SamplerParameterType param, unsigned int level=0);

  SamplerMapType& GetSamplers(unsigned int level=0);

  void SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level=0);

  otb::ogr::DataSource* GetOutputPositionContainer(unsigned int level=0);
 
protected:
  /** Constructor */
  OGRDataToSamplePositionFilter() {}
  /** Destructor */
  virtual ~OGRDataToSamplePositionFilter() {}

private:
  OGRDataToSamplePositionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataToSamplePositionFilter.txx"
#endif

#endif
