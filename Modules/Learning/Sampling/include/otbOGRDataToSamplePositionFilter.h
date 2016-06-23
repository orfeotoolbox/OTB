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

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
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
  public PersistentSamplingFilterBase<TInputImage, TMaskImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToSamplePositionFilter         Self;
  typedef PersistentSamplingFilterBase<
    TInputImage,
    TMaskImage>                                           Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;

  typedef TMaskImage                                      MaskImageType;
  typedef typename MaskImageType::Pointer                 MaskImagePointer;

  typedef ogr::DataSource                                 OGRDataType;
  typedef ogr::DataSource::Pointer                        OGRDataPointer;

  typedef TSampler                                        SamplerType;
  typedef typename SamplerType::Pointer                   SamplerPointerType;
  typedef typename SamplerType::ParameterType             SamplerParameterType;
  typedef typename std::map
    <std::string, SamplerPointerType>                     SamplerMapType;

  typedef std::map<std::string, unsigned int>             ClassPartitionType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToSamplePositionFilter, PersistentSamplingFilterBase);

  /** Synthetize the persistent filter*/
  virtual void Synthetize(void);

  /** Reset method called before starting the streaming*/
  virtual void Reset(void);

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

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

  /** Get/Set of the field name storing the original FID of each sample */
  itkSetMacro(OriginFieldName, std::string);
  itkGetMacro(OriginFieldName, std::string);

protected:
  /** Constructor */
  PersistentOGRDataToSamplePositionFilter();
  /** Destructor */
  virtual ~PersistentOGRDataToSamplePositionFilter() {}

  /** Call samplers on a current position, for a given class */
  virtual void ProcessSample(const ogr::Feature& feature,
                             typename TInputImage::IndexType& imgIndex,
                             typename TInputImage::PointType& imgPoint,
                             itk::ThreadIdType& threadid);

  /** Method to split the input OGRDataSource
   *  according to the class partition
   */
  virtual void DispatchInputVectors(ogr::Layer &inLayer, std::vector<ogr::Layer> &tmpLayers);

private:
  PersistentOGRDataToSamplePositionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void ComputeClassPartition(void);

  /** (internal) map associating a class name with a thread number */
  ClassPartitionType m_ClassPartition;

  /** Internal samplers*/
  std::vector<SamplerMapType> m_Samplers;

  /** Field name to store the FID of the geometry each sample comes from */
  std::string m_OriginFieldName;
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

  /** Set the field name storing the original FID of each sample*/
  void SetOriginFieldName(std::string key);

  /** Get the field name storing the original FID of each sample*/
  std::string GetOriginFieldName();

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
