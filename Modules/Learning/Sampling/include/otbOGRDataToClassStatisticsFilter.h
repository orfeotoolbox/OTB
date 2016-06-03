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

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"

namespace otb
{

/**
 * \class PersistentOGRDataToClassStatisticsFilter
 * 
 * \brief Persistent filter to compute class statistics based on vectors
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage>
class ITK_EXPORT PersistentOGRDataToClassStatisticsFilter :
  public PersistentSamplingFilterBase<TInputImage, TMaskImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToClassStatisticsFilter        Self;
  typedef PersistentSamplingFilterBase<
    TInputImage,
    TMaskImage>                                           Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;

  /** Wrap output type as DataObject */
  typedef std::map<std::string, unsigned long>      ClassCountMapType;
  typedef std::map<unsigned long, unsigned long>    PolygonSizeMapType;
  typedef itk::SimpleDataObjectDecorator<ClassCountMapType>  ClassCountObjectType;
  typedef itk::SimpleDataObjectDecorator<PolygonSizeMapType> PolygonSizeObjectType;

  typedef itk::DataObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToClassStatisticsFilter, PersistentSamplingFilterBase);

  void Synthetize(void);

  /** Reset method called before starting the streaming*/
  void Reset(void);

  /** the class count map is stored as output #2 */
  const ClassCountObjectType* GetClassCountOutput() const;
  ClassCountObjectType* GetClassCountOutput();

  /** the polygon size map is stored as output #3 */
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

  /** Implement generic method called at each candidate position */
  virtual void ProcessSample(const ogr::Feature& feature,
                             typename TInputImage::IndexType& imgIndex,
                             typename TInputImage::PointType& imgPoint,
                             itk::ThreadIdType& threadid);

  /** Prepare temporary variables for the current feature */
  virtual void PrepareFeature(const ogr::Feature& feature,
                              itk::ThreadIdType& threadid);

private:
  PersistentOGRDataToClassStatisticsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Number of pixels in all the polygons (per thread) */
  std::vector<unsigned long> m_NbPixelsThread;
  /** Number of pixels in each classes (per thread) */
  std::vector<ClassCountMapType> m_ElmtsInClassThread;
  /** Number of pixels in each polygons (per thread) */
  std::vector<PolygonSizeMapType> m_PolygonThread;
  /** Class name of the current feature (per thread) */
  std::vector<std::string> m_CurrentClass;
  /** FID of the current feature (per thread) */
  std::vector<unsigned long> m_CurrentFID;

};

/**
 * \class OGRDataToClassStatisticsFilter
 * 
 * \brief Computes class statistics based on vectors using a persistent filter
 * 
 * \sa PersistentOGRDataToClassStatisticsFilter
 *
 * \ingroup OTBSampling
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
  
  void SetLayerIndex(int index);
  int GetLayerIndex();

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
