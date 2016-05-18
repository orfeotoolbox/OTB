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
#include "itkSimpleDataObjectDecorator.h"
#include "otbSamplingRateCalculator.h"
#include "otbPeriodicSampler.h"
#include "otbImage.h"

namespace otb
{

/**
 * \class PersistentOGRDataToSamplePositionFilter
 * 
 * \brief Persistent filter to extract sample position from an image
 *
 * This filter uses an input image (to define the sampling grid), an optional
 * input mask, and a set of input vectors that define the sampling regions for
 * each class.
 *
 * The filter has a set of samplers (one for each class), they define what
 * sampling rate and strategy should be performed.
 * 
 * Several levels of output are supported. For instance, with two outputs :
 * when the sampler from level 1 discards a sample, the sampler from level 2 is
 * called.
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

  /** Set the input OGRDataSource that contains sampling areas for each class*/
  void SetOGRData(const otb::ogr::DataSource* vector);

  /** Get the input OGRDataSource with sampling regions*/
  const otb::ogr::DataSource* GetOGRData();

  /** Set an input mask (optional) */
  void SetMask(const TMaskImage* mask);

  /** Get the input mask (may be null) */
  const TMaskImage* GetMask();

  /** Synthetize the persistent filter*/
  void Synthetize(void);

  /** Reset method called before starting the streaming*/
  void Reset(void);

  /** Set/Get macro for the field name containing class names
   * in the input vectors.*/
  itkSetMacro(FieldName, std::string);
  itkGetMacro(FieldName, std::string);

  /** Set/Get macro for the layer index containing the sampling areas */
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int); 

  /** Get a reference to the internal samplers at a given level */
  SamplerMapType& GetSamplers(unsigned int level);

  /** Set an output container for sample position associated
   * with corresponding rates, for a given level.*/
  void SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level);

  /** Get the output position container of a given level */
  const otb::ogr::DataSource* GetOutputPositionContainer(unsigned int level) const;
  otb::ogr::DataSource* GetOutputPositionContainer(unsigned int level);

  /** Get the number of sampling levels used in this filter.*/
  unsigned int GetNumberOfLevels();

  /** Clear all output position containers */
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

  /** Filter the input vector to the extent of the stream being processed.*/
  void ApplyPolygonsSpatialFilter();

  /** Get the region bounding a set of features */
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

  /** Call samplers on a current position, for a given class */
  void CallSamplers(const PointType &point,
                    const std::string &className);

  /** Field name containing the class name*/
  std::string m_FieldName;

  /** Field index containing the class name*/
  int m_FieldIndex;

  /** Internal samplers*/
  std::vector<SamplerMapType> m_Samplers;

  /** Layer to use in the input vector file, default to 0 */
  int m_LayerIndex;
  
  /** (internal) name of the layer at position 'm_LayerIndex' */
  std::string m_LayerName;

  /** OGR Layer creation option for output position containers */
  std::vector<std::string> m_OGRLayerCreationOptions;

  /** In-memory containers storing position during iteration loop*/
  std::vector<OGRDataPointer> m_InMemoryOutputs;
};

/**
 * \class OGRDataToSamplePositionFilter
 * 
 * \brief Extracts sample position from an image using a persistent filter
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

  /** Set the input image */
  virtual void SetInput(const TInputImage* image);

  /** Get the input image*/
  const TInputImage* GetInput();

  /** Set the input OGRDataSource containing sampling areas */
  void SetOGRData(const otb::ogr::DataSource* data);

  /** Get the input OGRDataSource containing sampling areas */
  const otb::ogr::DataSource* GetOGRData();

  /** Set the input mask (optional) */
  void SetMask(const TMaskImage* mask);

  /** Get input mask (may be null)*/
  const TMaskImage* GetMask();

  /** Set the field name containing class names*/
  void SetFieldName(std::string key);

  /** Get the field name containing class names*/
  std::string GetFieldName();

  /** Set the layer index containing sampling areas*/
  void SetLayerIndex(int index);

  /** Get the layer index containing sampling areas*/
  int GetLayerIndex();

  /** Set the sampling parameters for all classes at a given level.*/
  void SetSamplerParameters(SamplerParameterType param, unsigned int level=0);

  /** Get a reference to the internal sampler map at a given level.*/
  SamplerMapType& GetSamplers(unsigned int level=0);

  /** Set the output container with the associated rates at a given level.*/
  void SetOutputPositionContainerAndRates(
    otb::ogr::DataSource* data,
    const SamplingRateCalculator::MapRateType& map,
    unsigned int level=0);

  /** Get the output position container at a given level.*/
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
