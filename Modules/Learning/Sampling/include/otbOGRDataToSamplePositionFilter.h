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
#include "otbOGRDataResampler.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbSamplingRateCalculator.h"


namespace otb
{

/**
 * \class PersistentOGRDataToSamplePositionFilter
 * 
 * \brief Persistent filter to compute class statistics based on vectors
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage>
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
    
  
  void SetRatesbyClass(const SamplingRateCalculator::mapRateType& map )
  {
      m_RatesbyClass = map;
  }
  
  itkSetMacro(MaxSamplingVecSize, unsigned int);
  itkGetMacro(MaxSamplingVecSize, unsigned int);
  
  itkSetMacro(OutputVectorDataPath, std::string);
  itkGetMacro(OutputVectorDataPath, std::string);
  
  itkSetMacro(OutputSamplingVectorsPath, std::string);
  itkSetMacro(InputSamplingVectorsPath, std::string);
  
  

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

  //virtual void BeforeThreadedGenerateData();

  //virtual void ThreadedGenerateData(const RegionType& outputRegionForThread,
  //                                  itk::ThreadIdType threadId);
  virtual void GenerateData();

private:
  PersistentOGRDataToSamplePositionFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void ApplyPolygonsSpatialFilter();

  RegionType FeatureBoundingRegion(const TInputImage* image, otb::ogr::Layer::const_iterator& featIt) const;

  std::string m_FieldName;
  OGRDataResampler::Pointer m_TemporaryStats;
  SamplingRateCalculator::mapRateType  m_RatesbyClass;

  // Layer to use in the shape file, default to 0
  int m_LayerIndex;
  std::string m_OutputSamplingVectorsPath;
  std::string m_InputSamplingVectorsPath;
  
  unsigned int m_MaxSamplingVecSize;
  std::string m_OutputVectorDataPath;
  
  
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
template<class TInputImage, class TMaskImage>
class ITK_EXPORT OGRDataToSamplePositionFilter :
  public PersistentFilterStreamingDecorator<PersistentOGRDataToSamplePositionFilter<TInputImage,TMaskImage> >
{
public:
  /** Standard Self typedef */
  typedef OGRDataToSamplePositionFilter  Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentOGRDataToSamplePositionFilter
      <TInputImage,TMaskImage> >          Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef TMaskImage                      MaskImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  
  typedef typename Superclass::FilterType             FilterType;

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
  
  void SetRatesbyClass(const SamplingRateCalculator::mapRateType& map);
  void SetMaxSamplingVecSize(
  
  unsigned int index);
  unsigned int GetMaxSamplingVecSize();
 
  void SetOutputVectorDataPath(std::string);
 
  void SetOutputSamplingVectorsPath(std::string);
  void SetInputSamplingVectorsPath(std::string);
 
  const std::string* GetOutputVectorDataPath() const; 
  std::string* GetOutputVectorDataPath();
 
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
