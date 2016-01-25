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
  typedef typename InputImageType::RegionType             
  
  typedef otb::ogr::DataSource                            OGRDataType;
  typedef otb::ogr::DataSource::Pointer                   OGRDataPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToClassStatisticsFilter, PersistentImageFilter);
  
  // TODO : store in m_InputImage
  void SetInput(const TInputImage* mask);
  const TInputImage* GetInput();
  
  // TODO : input #2
  void SetMaskInput(const TMaskImage* mask);
  const TMaskImage* GetMaskInput();
  
  // TODO : input # 1
  void SetOGRData(const otb::ogr::DataSource* vector);
  const otb::ogr::DataSource* GetOGRData();
  
  void Synthetize(void);
  
  /** Reset method called before starting the streaming*/
  // TODO : inputImage->UpdateOutputInformation()
  void Reset(void);
  
  // TODO : prevent loading of data into output

  itkSetMacro(ClassKey, std::string);
  itkGetMacro(ClassKey, std::string);
  
protected:
  /** Constructor */
  PersistentOGRDataToClassStatisticsFilter() {}
  /** Destructor */
  virtual ~PersistentOGRDataToClassStatisticsFilter() {}
  
  void ThreadedGenerateData(const RegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  PersistentOGRDataToClassStatisticsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  
  InputImagePointer m_InputImage;
  
  std::string m_ClassKey;

};

/**
 * \class OGRDataToClassStatisticsFilter
 * 
 * \brief Filter to compute class statistics based on vectors
 * 
 * \sa PersistentOGRDataToClassStatisticsFilter
 */
template<class TInputImage>
class ITK_EXPORT OGRDataToClassStatisticsFilter :
  public PersistentFilterStreamingDecorator<PersistentOGRDataToClassStatisticsFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef OGRDataToClassStatisticsFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentOGRDataToClassStatisticsFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(OGRDataToClassStatisticsFilter, PersistentFilterStreamingDecorator);
  
  

protected:
  /** Constructor */
  OGRDataToClassStatisticsFilter() {}
  /** Destructor */
  virtual ~OGRDataToClassStatisticsFilter() {}

private:

};


#endif
