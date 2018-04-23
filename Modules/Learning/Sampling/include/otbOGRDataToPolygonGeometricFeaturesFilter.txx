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

#ifndef otbOGRDataToPolygonGeometricFeaturesFilter_txx
#define otbOGRDataToPolygonGeometricFeaturesFilter_txx

#include "otbOGRDataToPolygonGeometricFeaturesFilter.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor.
 */

OGRDataToPolygonGeometricFeaturesFilter
::OGRDataToPolygonGeometricFeaturesFilter()
  : m_OutLayerName(std::string("output"))
  , m_SizeField("size")
  , m_PerimeterField("perimeter")
{
  std::cout << "constructor" << std::endl;
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);
}
/**
 * Destructor.
 */

OGRDataToPolygonGeometricFeaturesFilter
::~OGRDataToPolygonGeometricFeaturesFilter()
{}


void
OGRDataToPolygonGeometricFeaturesFilter
::SetOutput(const otb::ogr::DataSource* ds)
{
  this->ProcessObject::SetNthOutput(0, const_cast<otb::ogr::DataSource *>(ds));
}

const typename OGRDataToPolygonGeometricFeaturesFilter::OGRDataSourceType *
OGRDataToPolygonGeometricFeaturesFilter
::GetOutput()
{
  return static_cast< const OGRDataSourceType *>(
    this->ProcessObject::GetOutput(0));
}

void
OGRDataToPolygonGeometricFeaturesFilter
::SetInput(const otb::ogr::DataSource* ds)
{
  this->Superclass::SetNthInput(0, const_cast<otb::ogr::DataSource *>(ds));
}

const typename OGRDataToPolygonGeometricFeaturesFilter::OGRDataSourceType *
OGRDataToPolygonGeometricFeaturesFilter
::GetInput(unsigned int idx)
{
  return static_cast<const OGRDataSourceType *>
    (this->itk::ProcessObject::GetInput(idx));
}

void
OGRDataToPolygonGeometricFeaturesFilter
::CreateAdditionalField(std::string name,
                        OGRFieldType type,
                        int width,
                        int precision)
{
  SimpleFieldDefn defn;
  defn.Name = name;
  defn.Type = type;
  defn.Width = width;
  defn.Precision = precision;
  this->m_AdditionalFields.push_back(defn);
}

const std::vector<typename OGRDataToPolygonGeometricFeaturesFilter::SimpleFieldDefn>& 
OGRDataToPolygonGeometricFeaturesFilter
::GetAdditionalFields()
{
  return this->m_AdditionalFields;
}

void
OGRDataToPolygonGeometricFeaturesFilter
::InitializeOutputDataSource(ogr::DataSource* inputDS, ogr::DataSource* outputDS)
{
  ogr::Layer inLayer = inputDS->GetLayer(0);

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
    std::string projectionRefWkt = inLayer.GetProjectionRef();
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
      m_OGRLayerCreationOptions);
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
  
  this->CreateAdditionalField(m_SizeField ,OFTReal,24,15);
  this->CreateAdditionalField(m_PerimeterField ,OFTReal,24,15);
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

void
OGRDataToPolygonGeometricFeaturesFilter
::AllocateOutputs(void)
{
  std::cout << "AllocateOutputs" << std::endl;
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetInput(0));
  ogr::Layer inLayer = vectors->GetLayer(0);

  unsigned int numberOfThreads = this->GetNumberOfThreads();

  // Prepare temporary input
  this->m_InMemoryInputs.clear();
  this->m_InMemoryInputs.reserve(numberOfThreads);
  std::string tmpLayerName("thread");
  OGRSpatialReference * oSRS = ITK_NULLPTR;
  if (inLayer.GetSpatialRef())
    {
    oSRS = inLayer.GetSpatialRef()->Clone();
    }
  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  //std::vector<ogr::Layer> tmpLayers;
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    ogr::DataSource::Pointer tmpOgrDS = ogr::DataSource::New();
    ogr::Layer tmpLayer = tmpOgrDS->CreateLayer(
      tmpLayerName,
      oSRS,
      inLayer.GetGeomType());
    // add field definitions
    for (int k=0 ; k < layerDefn.GetFieldCount() ; k++)
      {
      OGRFieldDefn originDefn(layerDefn.GetFieldDefn(k));
      ogr::FieldDefn fieldDefn(originDefn);
      tmpLayer.CreateField(fieldDefn);
   
      
      }
    this->m_InMemoryInputs.push_back(tmpOgrDS);
    }

  // Prepare in-memory outputs
  this->m_InMemoryOutputs.clear();
  this->m_InMemoryOutputs.reserve(numberOfThreads);
  tmpLayerName = std::string("threadOut");
  for (unsigned int i=0 ; i < numberOfThreads ; i++)
    {
    std::vector<OGRDataPointer> tmpContainer;
    // iterate over outputs, only process ogr::DataSource
    //for (unsigned int k=0 ; k < this->GetNumberOfOutputs() ; k++)
      //{
      ogr::DataSource* realOutput = dynamic_cast<ogr::DataSource *>(
        this->itk::ProcessObject::GetOutput(0));
      if (realOutput)
        {
        ogr::Layer realLayer = realOutput->GetLayersCount() == 1
                               ? realOutput->GetLayer(0)
                               : realOutput->GetLayer(m_OutLayerName);
        OGRFeatureDefn &outLayerDefn = realLayer.GetLayerDefn();
        ogr::DataSource::Pointer tmpOutput = ogr::DataSource::New();
        ogr::Layer tmpLayer = tmpOutput->CreateLayer(
          tmpLayerName, oSRS,  realLayer.GetGeomType());
        // add field definitions
        for (int f=0 ; f < outLayerDefn.GetFieldCount() ; f++)
          {
          OGRFieldDefn originDefn(outLayerDefn.GetFieldDefn(f));
          tmpLayer.CreateField(originDefn);
          }
        tmpContainer.push_back(tmpOutput);
        this->m_InMemoryOutputs.push_back(tmpOutput);
        }
      //}
    
    }

  if (oSRS)
    {
    oSRS->Release();
    }
}
/**
 * Main computation method.
 */

void
OGRDataToPolygonGeometricFeaturesFilter
::GenerateData()
{
  this->InitializeOutputDataSource(const_cast<ogr::DataSource*>(this->GetInput(0)), const_cast<ogr::DataSource*>(this->GetOutput()));
  this->AllocateOutputs();
  // Call a method that can be overridden by a subclass to perform
  // some calculations prior to splitting the main computations into
  // separate threads
  this->BeforeThreadedGenerateVectorData();

  // Split the data into in-memory layers
  this->DispatchInputVectors();

  // Set up the multithreaded processing
  VectorThreadStruct str;
  str.Filter = this;

  // Setting up multithreader
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->VectorThreaderCallback, &str);

  // multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // gather the data from in-memory output layers
  this->GatherOutputVectors();

  // Call a method that can be overridden by a subclass to perform
  // some calculations after all the threads have completed
  this->AfterThreadedGenerateVectorData();
}

void
OGRDataToPolygonGeometricFeaturesFilter
::DispatchInputVectors()
{
  //TInputImage* outputImage = this->GetOutput();
  ogr::DataSource* vectors = const_cast<ogr::DataSource*>(this->GetInput(0));
  ogr::Layer inLayer = vectors->GetLayer(0);
/*  ogr::Layer inLayer = vectors->GetLayer(m_LayerIndex);

  const RegionType& requestedRegion = outputImage->GetRequestedRegion();
  itk::ContinuousIndex<double> startIndex(requestedRegion.GetIndex());
  itk::ContinuousIndex<double> endIndex(requestedRegion.GetUpperIndex());
  startIndex[0] += -0.5;
  startIndex[1] += -0.5;
  endIndex[0] += 0.5;
  endIndex[1] += 0.5;
  itk::Point<double, 2> startPoint;
  itk::Point<double, 2> endPoint;
  outputImage->TransformContinuousIndexToPhysicalPoint(startIndex, startPoint);
  outputImage->TransformContinuousIndexToPhysicalPoint(endIndex, endPoint);

  // create geometric extent
  OGRPolygon tmpPolygon;
  OGRLinearRing ring;
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  ring.addPoint(startPoint[0],endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,endPoint[1]  ,0.0);
  ring.addPoint(endPoint[0]  ,startPoint[1],0.0);
  ring.addPoint(startPoint[0],startPoint[1],0.0);
  tmpPolygon.addRing(&ring);

  inLayer.SetSpatialFilter(&tmpPolygon);
*/
  unsigned int numberOfThreads = this->GetNumberOfThreads();
  std::vector<ogr::Layer> tmpLayers;
  tmpLayers.reserve(numberOfThreads);
  for (unsigned int i=0 ; i<numberOfThreads ; i++)
    {
    tmpLayers.push_back(this->GetInMemoryInput(i));
    }

  const unsigned int nbFeatThread = std::ceil(inLayer.GetFeatureCount(true) / (float) numberOfThreads);
  //assert(nbFeatThread > 0);

  OGRFeatureDefn &layerDefn = inLayer.GetLayerDefn();
  ogr::Layer::const_iterator featIt = inLayer.begin();
  unsigned int counter=0;
  unsigned int cptFeat = 0;
  for(; featIt!=inLayer.end(); ++featIt)
    {
    ogr::Feature dstFeature(layerDefn);
    dstFeature.SetFrom( *featIt, TRUE );
    dstFeature.SetFID(featIt->GetFID());
    tmpLayers[counter].CreateFeature( dstFeature );
    cptFeat++;
    if (cptFeat > nbFeatThread && (counter + 1) < numberOfThreads)
      {
      counter++;
      cptFeat=0;
      }
    }

  inLayer.SetSpatialFilter(ITK_NULLPTR);
}
void
OGRDataToPolygonGeometricFeaturesFilter
::GatherOutputVectors(void)
{
  // clean temporary inputs
  this->m_InMemoryInputs.clear();

  // gather temporary outputs and write to output
  const otb::ogr::DataSource* vectors = this->GetInput(0);
  otb::Stopwatch chrono = otb::Stopwatch::StartNew();

  ogr::DataSource* realOutput = dynamic_cast<ogr::DataSource *>(
        this->itk::ProcessObject::GetOutput(0));
  if (realOutput)
  {
    this->FillOneOutput( realOutput, bool(vectors == realOutput));
  }

  chrono.Stop();
  otbMsgDebugMacro(<< "Writing OGR points took " << chrono.GetElapsedMilliseconds() << " ms");
  this->m_InMemoryOutputs.clear();
}

void
OGRDataToPolygonGeometricFeaturesFilter
::FillOneOutput(ogr::DataSource* outDS, bool update)
{//ogr::Layer inLayer = GetInMemoryInput(0);
  ogr::Layer outLayer = outDS->GetLayersCount() == 1
                        ? outDS->GetLayer(0)
                        : outDS->GetLayer(m_OutLayerName);
  
  OGRErr err = outLayer.ogr().StartTransaction();
  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
    }

  unsigned int numberOfThreads = this->GetNumberOfThreads();
  for (unsigned int thread=0 ; thread < numberOfThreads ; thread++)
    {
    ogr::Layer inLayer = GetInMemoryOutput(thread);
    if (!inLayer)
      {
      continue;
      }
    ogr::Layer::const_iterator tmpIt = inLayer.begin();
    
    if (update)
      {
      // Update mode
      for(; tmpIt!=inLayer.end(); ++tmpIt)
        {
        outLayer.SetFeature( *tmpIt );
        }
      }
    else
      {
      // Copy mode
      for(; tmpIt!=inLayer.end(); ++tmpIt)
        {
        ogr::Feature dstFeature(outLayer.GetLayerDefn());
        dstFeature.SetFrom( *tmpIt, TRUE );
        outLayer.CreateFeature( dstFeature );
        }
      }
    }
  err = outLayer.ogr().CommitTransaction();
  if (err != OGRERR_NONE)
    {
    itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
    }
}

void
OGRDataToPolygonGeometricFeaturesFilter
::ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid)
{
  std::cout << "ThreadedGenerateVectorData" << std::endl;
  ogr::Layer outLayer = this->GetInMemoryOutput(threadid);

  for (auto && feature : layerForThread)
  {
    //std::cout << "FID : " <<feature.GetFID() << std::endl;
    
    auto && geometry = feature.GetGeometry();
    assert(geometry->getGeometryType() == wkbPolygon); // otherwise get_area will crash with no explanation
    const double area = static_cast<OGRPolygon  const*>(geometry)->get_Area();
    const double perimeter = static_cast<OGRPolygon const*>(geometry)->getExteriorRing()->get_Length();
    //(*featit)["yo"].SetValue<double>(featit->GetFID());
    ogr::Feature dstFeature(outLayer.GetLayerDefn());
    dstFeature.SetFrom( feature, TRUE );
    dstFeature.SetFID( feature.GetFID() );
    dstFeature[m_SizeField].SetValue<double>(area);
    dstFeature[m_PerimeterField].SetValue<double>(perimeter);
    
    outLayer.CreateFeature( dstFeature );
  }
    
}

void
OGRDataToPolygonGeometricFeaturesFilter
::BeforeThreadedGenerateVectorData()
{
}

void
OGRDataToPolygonGeometricFeaturesFilter
::AfterThreadedGenerateVectorData()
{
}

// Callback routine used by the threading library. This routine just calls
// the ThreadedGenerateData method after setting the correct region for this
// thread.


ITK_THREAD_RETURN_TYPE
OGRDataToPolygonGeometricFeaturesFilter
::VectorThreaderCallback(void *arg)
{
  VectorThreadStruct *str = (VectorThreadStruct*)(((itk::MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  int threadId = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  int threadCount = ((itk::MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;

  ogr::Layer layer = str->Filter->GetInMemoryInput(threadId);

  if (threadId < threadCount)
  {
    str->Filter->ThreadedGenerateVectorData(layer,threadId);
  }

  return ITK_THREAD_RETURN_VALUE;
}


ogr::Layer
OGRDataToPolygonGeometricFeaturesFilter
::GetInMemoryInput(unsigned int threadId)
{
  if (threadId >= m_InMemoryInputs.size())
    {
    itkExceptionMacro(<< "Requested in-memory input layer not available " << threadId << " (total size : "<< m_InMemoryInputs.size() <<").");
    }
  return m_InMemoryInputs[threadId]->GetLayerChecked(0);
}

ogr::Layer
OGRDataToPolygonGeometricFeaturesFilter
::GetInMemoryOutput(unsigned int threadId)
{
  if (threadId >= m_InMemoryOutputs.size())
    {
    itkExceptionMacro(<< "Requested in-memory output layer not available " << threadId << " (total size : "<< m_InMemoryOutputs.size() <<").");
    }
  return m_InMemoryOutputs[threadId]->GetLayerChecked(0);
}


void
OGRDataToPolygonGeometricFeaturesFilter
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // end namespace otb
#endif
