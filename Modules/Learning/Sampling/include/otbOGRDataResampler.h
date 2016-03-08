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
#ifndef __otbOGRDataResampler_h
#define __otbOGRDataResampler_h

#include "otbOGRDataSourceWrapper.h"
#include "otbSamplingRateCalculator.h"

namespace otb
{

/**
 * \class OGRDataResampler
 *
 * \brief This class stores statistics of OGR data for each thread
 *
 */
class OGRDataResampler : public itk::Object
{
public:
  typedef OGRDataResampler  Self;
  typedef itk::SmartPointer<Self>            Pointer;
  itkNewMacro(Self);

  typedef std::map<std::string, unsigned long>      ClassCountMapType; //remove
  typedef std::map<unsigned long, unsigned long>    PolygonSizeMapType;//remove
  typedef std::map < std::string, std::vector< std::pair<double,double> > >  ClassToPhyPosMapType;

  /** Runtime information support. */
  itkTypeMacro(OGRDataResampler, itk::Object);

  template <typename TIterator>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           TIterator& imgIt);

  /** Reset the accumulator */
  void Reset();
  
  const ClassCountMapType& GetClassCountMap();
  
  const PolygonSizeMapType& GetPolygonSizeMap();
  
  const ClassToPhyPosMapType& GetClassToPhyPosMap();
  
  unsigned long GetNumberOfPixels();

  itkSetMacro(FieldIndex, int);
  itkGetMacro(FieldIndex, int);
  
  void SetRatesbyClass(const SamplingRateCalculator::mapRateType& map )
  {
      m_RatesbyClass = map;
      
  }
  void Prepare();

protected:
  /** Constructor */
  OGRDataResampler() {m_alreadyPrepared=false;}
  /** Destructor */
  virtual ~OGRDataResampler() {}

private:
  //Number of pixels in all the polygons
  unsigned long m_NbPixelsGlobal;
  //Number of pixels in each classes
  ClassCountMapType m_ElmtsInClass;
  //Number of pixels in each polygons
  PolygonSizeMapType m_Polygon;  // check the feature id
  
  //Map a class to the physical positions of the selected samples
  ClassToPhyPosMapType m_ClassToPhyPositions;
  
  int m_FieldIndex;
  bool m_alreadyPrepared;
  SamplingRateCalculator::mapRateType m_RatesbyClass;
  std::map<std::string, int > m_ClassToCurrentIndex;
  std::map<std::string, std::vector<bool> > m_ClassToBools;
  
  template <typename TIterator>
  void AddGeometry(OGRGeometry *geom,
                   TIterator& imgIt,
                   unsigned long &fId,
                   std::string &className);
                   
  bool TakeSample(std::string className);
  void keyInterpretor2(std::string key, std::string &className) const;

  // Not implemented
  OGRDataResampler(const Self&);
  void operator=(const Self&);
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataResampler.txx"
#endif

#endif
