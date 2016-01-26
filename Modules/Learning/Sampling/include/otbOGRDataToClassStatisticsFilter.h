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
#include "otbMaskedIteratorDecorator.h"

/**
 * \class PolygonClassStatisticsAccumulator
 *
 * \brief This class stores statistics of OGR data for each thread
 *
 */
class PolygonClassStatisticsAccumulator : public itk::Object
{
public:
  typedef PolygonClassStatisticsAccumulator  Self;
  typedef itk::SmartPointer<Self>            Pointer;
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PolygonClassStatisticsAccumulator, itk::Object);

  template <typename TInputImage>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           otb::MaskedIteratorDecorator<itk::ImageRegionIterator<TInputImage> >& pixelIt,
           const std::string &fieldName)
  {
    // TODO : move code to txx

    // TODO : switch case with feature type

    //OGRPolygon* inPolygon = dynamic_cast<OGRPolygon *>(geom);
    //OGRLinearRing* exteriorRing = inPolygon->getExteriorRing();
      //itk::Point<double, 2> point;
      //inputImage->TransformIndexToPhysicalPoint(it.GetIndex(), point);
      // ->Test if the current pixel is in a polygon hole
      // If point is in feature
      //if(exteriorRing->isPointInRing(&pointOGR, TRUE) && isNotInHole)
      //{
      //}
      // Count
      //nbOfPixelsInGeom++;
      //nbPixelsGlobal++;

    //Class name recuperation
    //int className = featIt->ogr().GetFieldAsInteger(GetParameterString("cfield").c_str());

    //Counters update, number of pixel in each classes and in each polygons
    //polygon[featIt->ogr().GetFID()] += nbOfPixelsInGeom;

    //Generation of a random number for the sampling in a polygon where we only need one pixel, it's choosen randomly
    //elmtsInClass[className] = elmtsInClass[className] + nbOfPixelsInGeom;
  }

  /** Reset the accumulator */
  void Reset();

protected:
  /** Constructor */
  PolygonClassStatisticsAccumulator() {}
  /** Destructor */
  virtual ~PolygonClassStatisticsAccumulator() {}

private:
  //Number of pixels in all the polygons
  int nbPixelsGlobal;
  //Number of pixels in each classes
  std::map<int, int> m_elmtsInClass;
  //Number of pixels in each polygons
  std::map<unsigned long, int> m_polygon;  // check the feature id

  // Not implemented
  PolygonClassStatisticsAccumulator(const Self&);
  void operator=(const Self&);
};

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
