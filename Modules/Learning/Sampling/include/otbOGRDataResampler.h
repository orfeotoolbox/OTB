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

  typedef otb::ogr::DataSource::Pointer                                      OGRDataSourcePointerType;
  typedef otb::ogr::Layer                                                    LayerType;
  typedef std::map<std::string, unsigned long>                               ClassCountMapType;
  typedef std::map<std::string, std::pair<unsigned int,unsigned int> >       ClassToCurrentIndicesType;
  typedef std::map<std::string, std::pair<std::vector<bool>,std::vector<bool> > > ClassToBoolsType;

  /** Runtime information support. */
  itkTypeMacro(OGRDataResampler, itk::Object);

  template <typename TIterator>
  void Add(otb::ogr::Layer::const_iterator& featIt,
           TIterator& imgIt,
           const typename TIterator::ImageType *mask );

  /** Reset the accumulator */
  void Reset();
  
  
  unsigned long GetNumberOfPixels();

  itkSetMacro(FieldIndex, int);
  itkGetMacro(FieldIndex, int);
  
  itkSetMacro(LayerIndex, int);
  itkGetMacro(LayerIndex, int);
  
  itkSetMacro(FieldName, std::string );
  itkGetMacro(FieldName, std::string );
  
  itkSetMacro(OutputPath, std::string );
  itkGetMacro(OutputPath, std::string );
  
  itkSetMacro(MaxSamplingTabSize, unsigned int);
  itkGetMacro(MaxSamplingTabSize, unsigned int);
  
  itkSetMacro(OutputSamplingVectorsPath, std::string);
  itkSetMacro(InputSamplingVectorsPath, std::string);
  
  void SetRatesbyClass( const SamplingRateCalculator::mapRateType& map )
  {
      m_RatesbyClass = map;
      
  }
  
  void SetInputOGRDataSourcePointer(const otb::ogr::DataSource* inOGR)
  {
    m_InputOGRDataSourcePointer = inOGR;
  }
  
  void Prepare();
  
  void PrepareOutputOGRData()
  {
      // Output OGR data 
      m_OutputOGRDataSourcePointer = otb::ogr::DataSource::New(  m_OutputPath, otb::ogr::DataSource::Modes::Overwrite );
      LayerType outputLayer = m_OutputOGRDataSourcePointer->CreateLayer(m_InputOGRDataSourcePointer->GetLayer( m_LayerIndex  ).GetName(),NULL,wkbPoint); //Create new layer
      OGRFieldDefn fieldClass(m_FieldName.c_str(), OFTString);
      outputLayer.CreateField(fieldClass, false);
  }

protected:
  /** Constructor */
  OGRDataResampler() {
  
  m_alreadyPrepared=false; 
  m_MaxSamplingTabSize=1000;
  m_LayerIndex = 0;
  
  } 
  /** Destructor */
  virtual ~OGRDataResampler() {}

private:
  
  bool m_alreadyPrepared;
  SamplingRateCalculator::mapRateType m_RatesbyClass;
  
  ClassToCurrentIndicesType m_ClassToCurrentIndices;
  ClassToBoolsType m_ClassToBools;
  ClassCountMapType m_ElmtsInClass;
  
  const otb::ogr::DataSource* m_InputOGRDataSourcePointer;
  OGRDataSourcePointerType m_OutputOGRDataSourcePointer;
  int                      m_LayerIndex;
  std::string              m_FieldName;
  int                      m_FieldIndex;
  std::string              m_OutputPath;
  
  std::string m_OutputSamplingVectorsPath;
  std::string m_InputSamplingVectorsPath;
  
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
  
  void OutputSamplingVectors();
  void InputSamplingVectors();

  // Not implemented
  OGRDataResampler(const Self&);
  void operator=(const Self&);
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataResampler.txx"
#endif

#endif
