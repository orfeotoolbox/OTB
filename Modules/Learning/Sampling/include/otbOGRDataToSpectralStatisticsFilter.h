/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbOGRDataToSpectralStatisticsFilter_h
#define otbOGRDataToSpectralStatisticsFilter_h

#include "otbPersistentSamplingFilterBase.h"
#include "otbPersistentFilterStreamingDecorator.h"
// #include "itkSimpleDataObjectDecorator.h" 

#include "itkArray.h"
namespace otb
{

/**
 * \class PersistentOGRDataToSpectralStatisticsFilter
 * 
 * \brief Persistent filter to compute spectral statistics based on vectors
 * 
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage>
class ITK_EXPORT PersistentOGRDataToSpectralStatisticsFilter :
  public PersistentSamplingFilterBase<TInputImage, TMaskImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentOGRDataToSpectralStatisticsFilter        Self;
  typedef PersistentSamplingFilterBase<
    TInputImage,
    TMaskImage>                                           Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  typedef TInputImage                                     InputImageType;
  typedef typename InputImageType::Pointer                InputImagePointer;
  typedef typename InputImageType::RegionType             RegionType;
  typedef typename InputImageType::PointType              PointType;
  typedef typename InputImageType::PixelType         PixelType;
  typedef typename InputImageType::InternalPixelType InternalPixelType;
  
  typedef itk::VariableSizeMatrix<double>        MatrixType;
  typedef itk::VariableLengthVector<double>      RealPixelType;

  typedef std::map<unsigned long, unsigned long>                    PolygonSizeMapType;
  typedef std::map<unsigned long, double>                           PolygonScalarMapType;
  typedef std::map< unsigned long, RealPixelType>                   PolygonVectorMapType;
  typedef std::map< unsigned long, MatrixType>                      PolygonMatrixMapType;
  
  typedef ogr::DataSource                                           OGRDataType;
  typedef ogr::DataSource::Pointer                                  OGRDataPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentOGRDataToSpectralStatisticsFilter, PersistentSamplingFilterBase);

  void Synthetize(void) override;

  /** Reset method called before starting the streaming*/
  void Reset(void) override;
  
  
  /** Set the output samples OGR container
   * (shall be equal to the input container for an 'update' mode) */
  void SetOutputSamples(ogr::DataSource* data);

  /** Get the output samples OGR container */
  ogr::DataSource* GetOutputSamples();

  /** Set/Get macro for the min field name prefix*/
  itkSetMacro(MinField, std::string);
  itkGetMacro(MinField, std::string);

  /** Set/Get macro for the max field name prefix*/
  itkSetMacro(MaxField, std::string);
  itkGetMacro(MaxField, std::string);
  
  /** Set/Get macro for the mean field name prefix*/
  itkSetMacro(MeanField, std::string);
  itkGetMacro(MeanField, std::string);

  /** Set/Get macro for the covariance field name prefix*/
  itkSetMacro(CovField, std::string);
  itkGetMacro(CovField, std::string);

  /** Set/Get macro for the number of pixels field name prefix*/
  itkSetMacro(NbPixelsField, std::string);
  itkGetMacro(NbPixelsField, std::string);

protected:
  /** Constructor */
  PersistentOGRDataToSpectralStatisticsFilter();
  /** Destructor */
  ~PersistentOGRDataToSpectralStatisticsFilter() override {};
  
  /** Initialize output fields */
  void InitializeFields();

  void GenerateOutputInformation() override;

  void GenerateInputRequestedRegion() override;

  
  /** Process a polygon : use pixels inside the polygon */
  virtual void ProcessPolygon(const ogr::Feature& feature,
                              OGRPolygon* polygon,
                              RegionType& region,
                              itk::ThreadIdType& threadid);

  /** Prepare temporary variables for the current feature */
  void PrepareFeature(const ogr::Feature& feature,
                      itk::ThreadIdType& threadid) override;

  /** Initialize output datasource, this method is overrided because we need to set the output geometry type as wkbPolygon 
   * instead of wkbPoint (as it is in the base class) */
  void InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS) override;


private:
  PersistentOGRDataToSpectralStatisticsFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** Number of pixels in each polygons (per thread) */
  std::vector<PolygonSizeMapType> m_PolygonSizeThread;
  
  /** Maximum of the pixel values in each polygons (per thread) */
  std::vector<PolygonVectorMapType> m_PolygonMaxThread;
  
  /** Minimum of the pixel values in each polygons (per thread) */
  std::vector<PolygonVectorMapType> m_PolygonMinThread;
  
  /** Sum of pixels  in each polygons (per thread) */
  std::vector<PolygonVectorMapType> m_PolygonFirstOrderThread;
  
  /** Cross sum for each pair of components of each pixels in each polygons (per thread) */
  std::vector<PolygonMatrixMapType> m_PolygonSecondOrderThread;
  
  /** FID of the current feature (per thread) */
  std::vector<unsigned long> m_CurrentFID;
  
  /** Update mode, set during Reset(), 1 if the output DS is the same as the input DS, 0 if it isn't*/
  bool m_updateMode = 0;

  /** Min field name prefix*/
  std::string m_MinField;
  
  /** Max field name prefix*/ 
  std::string m_MaxField;
  
  /** Mean field name prefix*/
  std::string m_MeanField;
  
  /** Covariance field name prefix */
  std::string m_CovField;  

  /** Number of pixels field name prefix */
  std::string m_NbPixelsField;  

};

/**
 * \class OGRDataToSpectralStatisticsFilter
 * 
 * \brief Computes class statistics based on vectors using a persistent filter
 * 
 * \sa PersistentOGRDataToSpectralStatisticsFilter
 *
 * \ingroup OTBSampling
 */
template<class TInputImage, class TMaskImage>
class ITK_EXPORT OGRDataToSpectralStatisticsFilter :
  public PersistentFilterStreamingDecorator<PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage> >
{
public:
  /** Standard Self typedef */
  typedef OGRDataToSpectralStatisticsFilter  Self;
  typedef PersistentFilterStreamingDecorator
    <PersistentOGRDataToSpectralStatisticsFilter
      <TInputImage,TMaskImage> >          Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInputImage                     InputImageType;
  typedef TMaskImage                      MaskImageType;
  typedef otb::ogr::DataSource            OGRDataType;
  typedef otb::ogr::Layer                 OGRLayerType;
  
  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(OGRDataToSpectralStatisticsFilter, PersistentFilterStreamingDecorator);

  using Superclass::SetInput;
  virtual void SetInput(const TInputImage* image);
  const TInputImage* GetInput();

  void SetOGRData(const otb::ogr::DataSource* data);
  const otb::ogr::DataSource* GetOGRData();
  
  void SetOutputSamples(OGRDataType::Pointer data);
  const otb::ogr::DataSource* GetOutputSamples();
  
  void SetMask(const TMaskImage* mask);
  const TMaskImage* GetMask();
  
  void SetFieldName(std::string key);
  std::string GetFieldName();
  
  void SetMinField(std::string key);
  std::string GetMinField();
  
  void SetMaxField(std::string key);
  std::string GetMaxField();
  
  void SetMeanField(std::string key);
  std::string GetMeanField();
  
  void SetCovField(std::string key);
  std::string GetCovField();
 
  void SetNbPixelsField(std::string key);
  std::string GetNbPixelsField();
 
protected:
  /** Constructor */
  OGRDataToSpectralStatisticsFilter() = default;
  /** Destructor */
  ~OGRDataToSpectralStatisticsFilter() override = default;
  
private:
  OGRDataToSpectralStatisticsFilter(const Self &) = delete; //purposely not implemented
  void operator =(const Self&) = delete; //purposely not implemented
};


} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOGRDataToSpectralStatisticsFilter.txx"
#endif

#endif
