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
 * \ingroup OTBSampling
 *
 */
class OGRDataResampler : public itk::Object
{
public:
  typedef OGRDataResampler  Self;
  typedef itk::SmartPointer<Self>            Pointer;
  itkNewMacro(Self);

  typedef std::map < std::string, std::vector< std::pair<double,double> > >  ClassToPhyPosMapType;

  /** Runtime information support. */
  itkTypeMacro(OGRDataResampler, itk::Object);

  template <typename TIterator>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           TIterator& imgIt,
           const typename TIterator::ImageType *mask );

  /** Reset the accumulator */
  void Reset();
  
  
  const ClassToPhyPosMapType& GetClassToPhyPosMap();
  
  unsigned long GetNumberOfPixels();

  itkSetMacro(FieldIndex, int);
  itkGetMacro(FieldIndex, int);
  
  itkSetMacro(MaxSamplingTabSize, unsigned int);
  itkGetMacro(MaxSamplingTabSize, unsigned int);
  
  void SetRatesbyClass(const SamplingRateCalculator::mapRateType& map )
  {
      m_RatesbyClass = map;
      
  }
  void Prepare();

protected:
  /** Constructor */
  OGRDataResampler() {m_alreadyPrepared=false; m_MaxSamplingTabSize=1000;} 
  /** Destructor */
  virtual ~OGRDataResampler() {}

private:
 
  //Map a class to the physical positions of the selected samples
  ClassToPhyPosMapType m_ClassToPhyPositions;
  
  int m_FieldIndex;
  bool m_alreadyPrepared;
  SamplingRateCalculator::mapRateType m_RatesbyClass;
  
  std::map<std::string, std::pair<unsigned int,unsigned int> > m_ClassToCurrentIndices;
  std::map<std::string, std::pair<std::vector<bool>,std::vector<bool> > > m_ClassToBools;
  
  template <typename TIterator>
  void AddGeometry(OGRGeometry *geom,
                   TIterator& imgIt,
                   const typename TIterator::ImageType *mask,
                   unsigned long &fId,
                   std::string &className);
                   
  unsigned int m_MaxSamplingTabSize;                 
  unsigned int selectN1(double per,unsigned int T);                 
  unsigned int selectN2(double per,unsigned int T);  
  std::vector<bool> randArray(unsigned int N,unsigned int T); 
  unsigned int findBestSize(unsigned int tot);              
                   
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
