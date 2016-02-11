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
#ifndef __otbOGRDataToClassStatisticsFilter_h
#define __otbOGRDataToClassStatisticsFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbPolygonClassStatisticsAccumulator.h"
#include "itkSimpleDataObjectDecorator.h"

namespace otb
{

/**
 * \class PersistentOGRDataToClassStatisticsFilter
 * 
 * \brief Persistent filter to compute class statistics based on vectors
 * 
 */
template<class TInputImage, class TMaskImage>
class ITK_EXPORT PersistentOGRDataToClassStatisticsFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToClassStatisticsFilter        Self;
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

  typedef otb::PolygonClassStatisticsAccumulator::ClassCountMapType   ClassCountMapType;
  typedef otb::PolygonClassStatisticsAccumulator::PolygonSizeMapType  PolygonSizeMapType;

  /** Wrap output type as DataObject */
  typedef itk::SimpleDataObjectDecorator<ClassCountMapType>  ClassCountObjectType;
  typedef itk::SimpleDataObjectDecorator<PolygonSizeMapType> PolygonSizeObjectType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToClassStatisticsFilter, PersistentImageFilter);

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

  // TODO: store the class count map as output #2
  const ClassCountObjectType* GetClassCountOutput() const;
  ClassCountObjectType* GetClassCountOutput();

  // TODO: store the polygon size map as output #3
  const PolygonSizeObjectType* GetPolygonSizeOutput() const;
  PolygonSizeObjectType* GetPolygonSizeOutput();

  /** Make a DataObject of the correct type to be used as the specified
   * output. */
  virtual itk::DataObject::Pointer MakeOutput(DataObjectPointerArraySizeType idx);
  using Superclass::MakeOutput;

protected:
  /** Constructor */
  PersistentOGRDataToClassStatisticsFilter();
  /** Destructor */
  virtual ~PersistentOGRDataToClassStatisticsFilter() {}

  virtual void GenerateOutputInformation();

  virtual void GenerateInputRequestedRegion();

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const RegionType& outputRegionForThread,
                                    itk::ThreadIdType threadId);

private:
  PersistentOGRDataToClassStatisticsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  void ApplyPolygonsSpatialFilter();

  RegionType FeatureBoundingRegion(const TInputImage* image, const otb::ogr::Feature& feature) const;

  std::string m_FieldName;

  std::vector<PolygonClassStatisticsAccumulator::Pointer> m_TemporaryStats;

  // Layer to use in the shape file, default to 0
  vcl_size_t m_layerIndex;
};

/**
 * \class OGRDataToClassStatisticsFilter
 * 
 * \brief Computes class statistics based on vectors using a persistent filter
 * 
 * \sa PersistentOGRDataToClassStatisticsFilter
 */
template<class TInputImage, class TMaskImage>
class ITK_EXPORT OGRDataToClassStatisticsFilter :
  public PersistentFilterStreamingDecorator<PersistentOGRDataToClassStatisticsFilter<TInputImage,TMaskImage> >
{
public:
  /** Standard Self typedef */
  typedef OGRDataToClassStatisticsFilter  Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentOGRDataToClassStatisticsFilter
      <TInputImage,TMaskImage> >          Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef TMaskImage                      MaskImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  
  typedef typename Superclass::FilterType             FilterType;
  typedef typename FilterType::ClassCountMapType      ClassCountMapType;
  typedef typename FilterType::PolygonSizeMapType     PolygonSizeMapType;
  typedef typename FilterType::ClassCountObjectType   ClassCountObjectType;
  typedef typename FilterType::PolygonSizeObjectType  PolygonSizeObjectType;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(OGRDataToClassStatisticsFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);

  const TInputImage* GetInput();

  void SetOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetOGRData();

  void SetMask(const TMaskImage* mask);
  const TMaskImage* GetMask();

  void SetFieldName(std::string &key);
  std::string GetFieldName();

  const ClassCountObjectType* GetClassCountOutput() const;
  ClassCountObjectType* GetClassCountOutput();

  const PolygonSizeObjectType* GetPolygonSizeOutput() const;
  PolygonSizeObjectType* GetPolygonSizeOutput();

protected:
  /** Constructor */
  OGRDataToClassStatisticsFilter() {}
  /** Destructor */
  virtual ~OGRDataToClassStatisticsFilter() {}

private:
  OGRDataToClassStatisticsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataToClassStatisticsFilter.txx"
#endif

#endif
