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

#ifndef otbOGRDataToSpectralStatisticsFilter_txx
#define otbOGRDataToSpectralStatisticsFilter_txx

#include "otbOGRDataToSpectralStatisticsFilter.h"

namespace otb
{
// --------- otb::PersistentOGRDataToSpectralStatisticsFilter ---------------------

template<class TInputImage, class TMaskImage>
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::PersistentOGRDataToSpectralStatisticsFilter()
  : m_MinField("Min")
  , m_MaxField("Max")
  , m_MeanField("Mean")
  , m_CovField("Cov")
  , m_NbPixelsField("Nbpixels")
{  
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TInputImage::New());
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOutputSamples(ogr::DataSource* data)
{
  this->SetNthOutput(1,data);
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  InputImageType *input = const_cast<InputImageType*>(this->GetInput());
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  input->SetRequestedRegion(requested);
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
}



template<class TInputImage, class TMaskImage>
ogr::DataSource*
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOutputSamples()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::Reset(void)
{
  TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();
  
  m_PolygonSizeThread.clear();
  m_PolygonMaxThread.clear();
  m_PolygonMinThread.clear();
  m_PolygonFirstOrderThread.clear();
  m_PolygonSecondOrderThread.clear();
  m_CurrentFID.clear();
  
  m_PolygonSizeThread.resize(this->GetNumberOfThreads());
  m_PolygonMaxThread.resize(this->GetNumberOfThreads());
  m_PolygonMinThread.resize(this->GetNumberOfThreads());
  m_PolygonFirstOrderThread.resize(this->GetNumberOfThreads());
  m_PolygonSecondOrderThread.resize(this->GetNumberOfThreads());
  m_CurrentFID.resize(this->GetNumberOfThreads());
  
  this->InitializeFields();
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  if (inputDS == output) 
    m_updateMode = 1;
    
  this->InitializeOutputDataSource(inputDS,output);
}


template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::Synthetize(void)
{
  PolygonSizeMapType PolygonSize;
  PolygonVectorMapType PolygonMin;
  PolygonVectorMapType PolygonMax;
  PolygonVectorMapType PolygonFirstOrderComponent;
  PolygonVectorMapType PolygonMeanComponent;
  PolygonMatrixMapType PolygonSecondOrderComponent;
  PolygonMatrixMapType PolygonCovComponent;
  
  const TInputImage* img = this->GetInput();
  unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
  
  // Accumulate Results from all thread
  const itk::ThreadIdType numberOfThreads = this->GetNumberOfThreads();
  
  for (itk::ThreadIdType threadId = 0; threadId < numberOfThreads; threadId++) 
  {
    PolygonSizeMapType::iterator itSize = m_PolygonSizeThread[threadId].begin();
    PolygonVectorMapType::iterator itFirstOrder = m_PolygonFirstOrderThread[threadId].begin();
    PolygonVectorMapType::iterator itMax = m_PolygonMaxThread[threadId].begin();
    PolygonVectorMapType::iterator itMin = m_PolygonMinThread[threadId].begin();
    PolygonMatrixMapType::iterator itSecondOrder = m_PolygonSecondOrderThread[threadId].begin();
    
    for (; itSize != m_PolygonSizeThread[threadId].end(); itSize++, itFirstOrder++, itSecondOrder++, itMax++, itMin++)
    {
      unsigned int fid = itSize->first;
      if (!PolygonSize.count(fid))
      {
        PolygonSize[fid] = itSize->second;
        PolygonFirstOrderComponent[fid] = itFirstOrder->second;
        PolygonSecondOrderComponent[fid] = itSecondOrder->second;
        PolygonMin[fid] = itMin->second;
        PolygonMax[fid] = itMax->second;
      }
      else
      {
        PolygonSize[fid] += itSize->second;
        for (unsigned int i =0; i < numberOfComponents ; i++)
        {
          PolygonFirstOrderComponent[fid][i] += itFirstOrder->second[i];
          if ( itMin->second[i] < PolygonMin[fid][i])
          {
            PolygonMin[fid][i] = itMin->second[i];
          }
          if ( itMax->second[i] > PolygonMax[fid][i])
          {
            PolygonMax[fid][i] = itMax->second[i]; 
          }
          
        }
        for (unsigned int r =0; r < numberOfComponents ; r++)
        {
          for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            PolygonSecondOrderComponent[fid][r][c] += itSecondOrder->second[r][c];
          }
        }
      }
    }
  }
  
  // Final computation
  
  PolygonSizeMapType::iterator itSizeFull = PolygonSize.begin();
  PolygonVectorMapType::iterator itMean = PolygonFirstOrderComponent.begin();
  PolygonMatrixMapType::iterator itCov = PolygonSecondOrderComponent.begin();
  
  //ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  
  
  for (; itMean !=  PolygonFirstOrderComponent.end(); itMean++,itSizeFull++, itCov++)
  {
    // Retrieve the feature corresponding to the fid in the output layer
    int fid = itMean->first;
    
    // Size computation
    int sizePolygon =  itSizeFull->second;

    if (sizePolygon > 0)
    {
      // Mean Computation 
      PolygonMeanComponent[itMean->first].SetSize(numberOfComponents);
      for (unsigned int i =0; i < numberOfComponents ; i++)
      {
        PolygonMeanComponent[fid][i] = itMean->second[i] / sizePolygon;
      }
      
      // Covariance computation
      PolygonCovComponent[itMean->first].SetSize(numberOfComponents,numberOfComponents);
      for (unsigned int r =0; r < numberOfComponents ; r++)
      {
        for (unsigned int c =0; c < numberOfComponents ; c++)
        {
          PolygonCovComponent[itMean->first][r][c] = itCov->second[r][c] / sizePolygon - PolygonMeanComponent[itMean->first][r]*PolygonMeanComponent[itMean->first][c];
        }
      }
    }
    
  }

  // initialize output DataSource
  ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
  ogr::DataSource* output  = this->GetOutputSamples();
  
  auto outLayer = output->GetLayer(0);
  auto inLayer = inputDS->GetLayer(0);
  
  PolygonSizeMapType::iterator itSizeout = PolygonSize.begin();
  PolygonVectorMapType::iterator itFirstOrderout = PolygonFirstOrderComponent.begin();
  PolygonVectorMapType::iterator itMeanout = PolygonMeanComponent.begin();
  PolygonVectorMapType::iterator itMaxout = PolygonMax.begin();
  PolygonVectorMapType::iterator itMinout = PolygonMin.begin();
  PolygonMatrixMapType::iterator itCovout = PolygonCovComponent.begin();
  
  
  for (; itSizeout != PolygonSize.end(); itSizeout++, itFirstOrderout++, itMeanout++, itCovout++, itMaxout++, itMinout++)
  {  
    // Write features in the ouput shapefile
    int fid = itMeanout->first;
    //auto outFeat = inLayer.GetFeature(fid);
    ogr::Feature outFeat(outLayer.GetLayerDefn());
    outFeat.SetFrom( inLayer.GetFeature(fid), TRUE );
    outFeat.SetGeometry( inLayer.GetFeature(fid).GetGeometry());
    
    outFeat.SetFID(fid);
    outFeat[m_NbPixelsField].SetValue<int>(itSizeout->second);
    for (unsigned int i =0; i < numberOfComponents ; i++)
    {
      outFeat[m_MeanField+"b"+std::to_string(i)].SetValue<double>(itMeanout->second[i]);
      outFeat[m_MinField+"b"+std::to_string(i)].SetValue<double>(itMinout->second[i]);
      outFeat[m_MaxField+"b"+std::to_string(i)].SetValue<double>(itMaxout->second[i]);
    }
    for (unsigned int r =0; r < numberOfComponents ; r++)
    {
      for (unsigned int c =0; c < numberOfComponents ; c++)
      {
        outFeat[m_CovField+"b"+std::to_string(r)+"b"+std::to_string(c)].SetValue<double>(itCovout->second[r][c]);
      }
    } 
    if (m_updateMode==0)
    {
      outLayer.CreateFeature(outFeat);
    }
    else
    {
      outLayer.SetFeature(outFeat);
    }
  }
  
  
}


template<class TInputImage,class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::InitializeFields()
{
  // Get number of components, to determine of many field will be created
  const TInputImage* img = this->GetInput();
  unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
  this->ClearAdditionalFields();
  
  this->CreateAdditionalField(m_NbPixelsField,OFTInteger,24,15);
  
  // Create min fields
  for (unsigned int i=0; i < numberOfComponents; i++)
  {
    this->CreateAdditionalField(m_MinField + "b" +std::to_string(i) ,OFTReal,24,15);
  }
  
  // Create max fields
  for (unsigned int i=0; i < numberOfComponents; i++)
  {
    this->CreateAdditionalField(m_MaxField + "b" +std::to_string(i) ,OFTReal,24,15);
  }
  for (unsigned int i=0; i < numberOfComponents; i++)
  
  // Create mean fields
  {
    this->CreateAdditionalField(m_MeanField + "b" +std::to_string(i) ,OFTReal,24,15);
  }
  
  // Create covariance fields
  for (unsigned int r=0; r < numberOfComponents; r++)
  {
    for (unsigned int c=0; c < numberOfComponents; c++)
    {
      this->CreateAdditionalField(m_CovField + "b" +std::to_string(r)+"b"+std::to_string(c),OFTReal,24,15);
    }
  }
}


template <class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::ProcessPolygon(const ogr::Feature& feature,
                 OGRPolygon* polygon,
                 RegionType& region,
                 itk::ThreadIdType& threadid)
{
  const TInputImage* img = this->GetInput();
  TMaskImage* mask = const_cast<TMaskImage*>(this->GetMask());
  typename TInputImage::IndexType imgIndex;
  typename TInputImage::PointType imgPoint;
  
  OGRPoint tmpPoint;

  typedef itk::ImageRegionConstIterator<TInputImage> ValueIteratorType;
  ValueIteratorType it(img,region);
    
  unsigned int numberOfComponents = img->GetNumberOfComponentsPerPixel();
    
    
  it.GoToBegin();
  int fid = m_CurrentFID[threadid];
    
  // Initialize accumulator sizes
    
  if (!m_PolygonMinThread[threadid].count(fid))
  {
    m_PolygonFirstOrderThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonFirstOrderThread[threadid][fid].Fill(0);
      
    m_PolygonMinThread[threadid][fid].SetSize(numberOfComponents);
    m_PolygonMaxThread[threadid][fid].SetSize(numberOfComponents);

    m_PolygonMinThread[threadid][fid].Fill(itk::NumericTraits<double>::max());
    m_PolygonMaxThread[threadid][fid].Fill(itk::NumericTraits<double>::NonpositiveMin());
      
    m_PolygonSecondOrderThread[threadid][fid].SetSize(numberOfComponents, numberOfComponents);
    m_PolygonSecondOrderThread[threadid][fid].Fill(0);
  }
    
  while (!it.IsAtEnd())
  {
    imgIndex = it.GetIndex();
    img->TransformIndexToPhysicalPoint(imgIndex,imgPoint);
    tmpPoint.setX(imgPoint[0]);
    tmpPoint.setY(imgPoint[1]);
      
    bool isInside = this->IsSampleInsidePolygon(polygon,&tmpPoint);
    if (isInside)
    {
      PixelType VectorValue = it.Get();
      m_PolygonSizeThread[threadid][fid] ++;
          
      for (unsigned int i =0; i < numberOfComponents ; i++)
      {
        m_PolygonFirstOrderThread[threadid][fid][i] += VectorValue[i];
        if (VectorValue[i] > m_PolygonMaxThread[threadid][fid][i])
        {
          m_PolygonMaxThread[threadid][fid][i] = VectorValue[i];
        }
            
        if (VectorValue[i] < m_PolygonMinThread[threadid][fid][i])
        {
          m_PolygonMinThread[threadid][fid][i] = VectorValue[i];
        }
      }
          
      for (unsigned int r =0; r < numberOfComponents ; r++)
      {
        for (unsigned int c =0; c < numberOfComponents ; c++)
          {
            m_PolygonSecondOrderThread[threadid][fid][r][c] += VectorValue[r]*VectorValue[c];
          }
      }
    }
    ++it;
  }
    
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::PrepareFeature(const ogr::Feature& feature,
                 itk::ThreadIdType& threadid)
{
  std::string className(feature.ogr().GetFieldAsString(this->GetFieldIndex()));
  unsigned long fId = feature.ogr().GetFID();
  if (!m_PolygonSizeThread[threadid].count(fId))
    {
    m_PolygonSizeThread[threadid][fId] = 0;
    }
  m_CurrentFID[threadid] = fId;
}

template<class TInputImage, class TMaskImage>
void
PersistentOGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS)
{
  TInputImage *inputImage = const_cast<TInputImage*>(this->GetInput());
  inputImage->UpdateOutputInformation();

  ogr::Layer inLayer = inputDS->GetLayer(this->GetLayerIndex());

  bool updateMode = false;
  if (inputDS == outputDS)
    {
    updateMode = true;
    // Check m_OutLayerName is same as input layer name
    this->SetOutLayerName(inLayer.GetName());
    }

  // First get list of current fields
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  std::map<std::string, OGRFieldType> currentFields;
  for (int k=0 ; k<layerDefn.GetFieldCount() ; k++)
    {
    OGRFieldDefn fieldDefn(layerDefn.GetFieldDefn(k));
    std::string currentName(fieldDefn.GetNameRef());
    currentFields[currentName] = fieldDefn.GetType();
    }

  ogr::Layer outLayer = inLayer;
  if (!updateMode)
    {
    std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
    bool projectionInformationAvailable = !projectionRefWkt.empty();
    OGRSpatialReference * oSRS = ITK_NULLPTR;
    if(projectionInformationAvailable)
      {
      oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
      }
    // Create layer
    outLayer = outputDS->CreateLayer(
      this->GetOutLayerName(),
      oSRS,
       wkbPolygon,
      this->GetOGRLayerCreationOptions());
    // Copy existing fields
    for (int k=0 ; k<layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn fieldDefn(layerDefn.GetFieldDefn(k));
      outLayer.CreateField(fieldDefn);
      }

    if (oSRS)
      {
      oSRS->Release();
      }
    }

  // Add new fields
  
  auto AdditionalFields = this->GetAdditionalFields();
  for (unsigned int k=0 ; k<AdditionalFields.size() ; k++)
    {
    OGRFieldDefn ogrFieldDefinition(AdditionalFields[k].Name.c_str(),AdditionalFields[k].Type);
    ogrFieldDefinition.SetWidth( AdditionalFields[k].Width );
    ogrFieldDefinition.SetPrecision( AdditionalFields[k].Precision );
    ogr::FieldDefn fieldDef(ogrFieldDefinition);
    // test if field is already present
    if (currentFields.count(fieldDef.GetName()))
      {
      // test the field type
      if (currentFields[fieldDef.GetName()] != fieldDef.GetType())
        {
        itkExceptionMacro("Field name "<< fieldDef.GetName() << " already exists with a different type!");
        }
      }
    else
      {
      outLayer.CreateField(fieldDef);
      }
    }
}


// -------------- otb::OGRDataToSpectralStatisticsFilter --------------------------

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetInput(const TInputImage* image)
{
  this->GetFilter()->SetInput(image);
}

template<class TInputImage, class TMaskImage>
const TInputImage*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetInput()
{
  return this->GetFilter()->GetInput();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOGRData(const otb::ogr::DataSource* data)
{
  this->GetFilter()->SetOGRData(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOGRData()
{
  return this->GetFilter()->GetOGRData();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetOutputSamples( OGRDataType::Pointer data)
{
  this->GetFilter()->SetOutputSamples(data);
}

template<class TInputImage, class TMaskImage>
const otb::ogr::DataSource*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetOutputSamples()
{
  return this->GetFilter()->GetOutputSamples();
}


template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetMask(const TMaskImage* mask)
{
  this->GetFilter()->SetMask(mask);
}

template<class TInputImage, class TMaskImage>
const TMaskImage*
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetMask()
{
  return this->GetFilter()->GetMask();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetFieldName(std::string key)
{
  this->GetFilter()->SetFieldName(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetFieldName()
{
  return this->GetFilter()->GetFieldName();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetMinField(std::string key)
{
  this->GetFilter()->SetMinField(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetMinField()
{
  return this->GetFilter()->GetMinField();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetMaxField(std::string key)
{
  this->GetFilter()->SetMaxField(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetMaxField()
{
  return this->GetFilter()->GetMaxField();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetMeanField(std::string key)
{
  this->GetFilter()->SetMeanField(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetMeanField()
{
  return this->GetFilter()->GetMeanField();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetCovField(std::string key)
{
  this->GetFilter()->SetCovField(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetCovField()
{
  return this->GetFilter()->GetCovField();
}

template<class TInputImage, class TMaskImage>
void
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::SetNbPixelsField(std::string key)
{
  this->GetFilter()->SetNbPixelsField(key);
}

template<class TInputImage, class TMaskImage>
std::string
OGRDataToSpectralStatisticsFilter<TInputImage,TMaskImage>
::GetNbPixelsField()
{
  return this->GetFilter()->GetNbPixelsField();
}
} // end of namespace otb

#endif

