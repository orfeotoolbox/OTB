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

#ifndef otbExtractClassifyFilter_hxx
#define otbExtractClassifyFilter_hxx

#include "otbExtractClassifyFilter.h"
#include "itkDefaultConvertPixelTraits.h"
#include "itkProgressReporter.h"

namespace otb
{
  // --------- otb::PersistentExtractClassifyFilter ---------------------

  template<class TInputImage, class TOutputLabel>
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::PersistentExtractClassifyFilter() :
    m_LabelFieldName(std::string("class")), m_ExtraFields({})
  {
    this->SetNumberOfRequiredOutputs(2);
    this->SetNthOutput(0,TInputImage::New());
  }

  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetOutputSamples(ogr::DataSource* data)
  {
    this->SetNthOutput(1,data);
  }

  template<class TInputImage, class TOutputLabel>
  ogr::DataSource*
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetOutputSamples()
  {
    if (this->GetNumberOfOutputs() < 2)
      {
	return 0;
      }
    return static_cast<ogr::DataSource *>(this->itk::ProcessObject::GetOutput(1));
  }

  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::Reset(void)
  {
    TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
    inputImage->UpdateOutputInformation();
    InitializeFields();

    // initialize output DataSource
    ogr::DataSource* inputDS = const_cast<ogr::DataSource*>(this->GetOGRData());
    ogr::DataSource* output  = this->GetOutputSamples();
    InitializeOutputDataSource(inputDS,output);

    // initialize Machine Learning Model
    m_Model = MachineLearningModelFactoryType::CreateMachineLearningModel(m_ModelString, MachineLearningModelFactoryType::ReadMode);

    if (m_Model.IsNull())
      {
	std::cout << "Error when loading model " << m_ModelString << " : unsupported model type";
      }

    m_Model->Load(m_ModelString);
  
  }

  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GenerateOutputInformation()
  {
    Superclass::GenerateOutputInformation();

    // Check SRS of input image and samples
    std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
    bool projectionInformationAvailable = !projectionRefWkt.empty();
    if(projectionInformationAvailable)
      {
	OGRSpatialReference imgSRS;
	const char *projWktCstr = projectionRefWkt.c_str();
	char **projWktPointer = const_cast<char**>(&projWktCstr);
	OGRErr err = imgSRS.importFromWkt( projWktPointer );
	if (err == OGRERR_NONE)
	  {
	    // get input layer
	    ogr::Layer inLayer = this->GetOGRData()->GetLayer(this->GetLayerIndex());
	    if ( !imgSRS.IsSame(inLayer.GetSpatialRef()) )
	      {
		char *layerSrsWkt = NULL;
		inLayer.GetSpatialRef()->exportToPrettyWkt(&layerSrsWkt);
		itkExceptionMacro(<< "Spatial reference of input image and samples don't match:  \n" << projectionRefWkt << "\nvs\n"<<  layerSrsWkt);
	      }
	  }
      }
  }

  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GenerateInputRequestedRegion()
  {
    InputImageType *input = const_cast<InputImageType*>(this->GetInput());
    RegionType requested = this->GetOutput()->GetRequestedRegion();
    input->SetRequestedRegion(requested);
  }


  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::ThreadedGenerateVectorData(const ogr::Layer& layerForThread, itk::ThreadIdType threadid)
  {
    // Retrieve inputs
    TInputImage* inputImage = const_cast<TInputImage*>(this->GetInput());
    unsigned int nbBand = inputImage->GetNumberOfComponentsPerPixel();

    ogr::Layer outputLayer = this->GetInMemoryOutput(threadid);

    itk::ProgressReporter progress( this, threadid, layerForThread.GetFeatureCount(true) );

    // Loop across the features in the layer (filtered by requested region in BeforeTGD already)
    OGRGeometry *geom;
    PointType imgPoint;
    IndexType imgIndex;
    PixelType imgPixel;

    ogr::Layer::const_iterator featIt = layerForThread.begin();

    // featIt->PrintSelf(std::cout, 0);
    
    for(; featIt!=layerForThread.end(); ++featIt)
      {
	geom = featIt->ogr().GetGeometryRef();
	switch (geom->getGeometryType())
	  {
	  case wkbPoint:
	  case wkbPoint25D:
	    {
	      OGRPoint* castPoint = dynamic_cast<OGRPoint*>(geom);
	      if (castPoint == NULL)
		{
		  // Wrong Type !
		  break;
		}
	      imgPoint[0] = castPoint->getX();
	      imgPoint[1] = castPoint->getY();
	      inputImage->TransformPhysicalPointToIndex(imgPoint,imgIndex);
	      imgPixel = inputImage->GetPixel(imgIndex);

	      //Process sample part
	      SampleType vectorToClassify = imgPixel;
	      const unsigned int s = vectorToClassify.Size();
	      if(!m_ExtraFields.empty()){
		//Resize vector, false indicates not to overwrite existing data (itk::VariableLengthVector) 
		vectorToClassify.SetSize(s+m_ExtraFields.size(),false);
		for (unsigned i = 0; i < m_ExtraFields.size(); i++) {
		  vectorToClassify[s+i]=(*featIt)[m_ExtraFields[i]].GetValue<double>();
		}
	      }
	      // try{
	      // 	if ((*featIt)["predicted"].GetValue<double>() != m_Model->Predict(vectorToClassify)[0]){
	      // 	  std::cout << "pred " << (*featIt)["predicted"].GetValue<double>() << "->" << m_Model->Predict(vectorToClassify)[0] << "   ";
	      // 	}
	      // }catch(const itk::ExceptionObject e){
		
	      // }
	
	      //Classify imgPixel
	      ogr::Feature dstFeature(outputLayer.GetLayerDefn());
	      dstFeature.SetFrom( *featIt, TRUE );
	      dstFeature.SetFID(featIt->GetFID());
	      dstFeature[m_LabelFieldName].SetValue((double) m_Model->Predict(vectorToClassify)[0]);
	      outputLayer.CreateFeature( dstFeature );
	      break;
	    }
	  default:
	    {
	      otbWarningMacro("Geometry not handled: " << geom->getGeometryName());
	      break;
	    }
	  }
	progress.CompletedPixel();
      }
    outputLayer.ogr().CommitTransaction();
  }


  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutputLabel>
  ::InitializeFields()
  {
    this->ClearAdditionalFields();
    this->CreateAdditionalField(m_LabelFieldName,OFTReal,24,15);
  }

  template<class TInputImage, class TOutputLabel>
  void
  PersistentExtractClassifyFilter<TInputImage,TOutputLabel>
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
	OGRSpatialReference * oSRS = nullptr;
	if(projectionInformationAvailable)
	  {
	    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
	  }
	// Create layer
	outLayer = outputDS->CreateLayer(
					 this->GetOutLayerName(),
					 oSRS,
					 wkbPoint,
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

    // Add new field
    OGRFieldDefn ogrFieldDefinition(m_LabelFieldName.c_str(),this->GetAdditionalFields()[0].Type);
    ogrFieldDefinition.SetWidth( this->GetAdditionalFields()[0].Width );
    ogrFieldDefinition.SetPrecision( this->GetAdditionalFields()[0].Precision );
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


  template<class TInputImage, class TOutput>
  void
  PersistentExtractClassifyFilter<TInputImage, TOutput>
  ::SetExtraFields(const std::vector<std::string> &name)
  {
    m_ExtraFields = name;
  }

  template<class TInputImage, class TOutput>
  std::vector<std::string>
  PersistentExtractClassifyFilter<TInputImage, TOutput>
  ::GetExtraFields() const
  {
    return m_ExtraFields;
  }
  
  // -------------- otb::ExtractClassifyFilter --------------------------

  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetInput(const TInputImage* image)
  {
    this->GetFilter()->SetInput(image);
  }

  template<class TInputImage, class TOutputLabel>
  const TInputImage*
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetSamplePositions(const otb::ogr::DataSource* data)
  {
    this->GetFilter()->SetOGRData(data);
  }

  template<class TInputImage, class TOutputLabel>
  const otb::ogr::DataSource*
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetSamplePositions()
  {
    return this->GetFilter()->GetOGRData();
  }

  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetOutputSamples(OGRDataType::Pointer data)
  {
    this->GetFilter()->SetOutputSamples(data);
  }

  template<class TInputImage, class TOutputLabel>
  const otb::ogr::DataSource*
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetOutputSamples()
  {
    return this->GetFilter()->GetOutputSamples();
  }


  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetLabelFieldName(const std::string &name)
  {
    this->GetFilter()->SetLabelFieldName(name);
  }

  template<class TInputImage, class TOutputLabel>
  const std::string
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetLabelFieldName()
  {
    return this->GetFilter()->GetLabelFieldName();
  }
  
  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetModelString(const std::string &name)
  {
    this->GetFilter()->SetModelString(name);
  }

  template<class TInputImage, class TOutputLabel>
  const std::string
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetModelString()
  {
    return this->GetFilter()->GetModelString();
  }

  template<class TInputImage, class TOutputLabel>
  void
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::SetLayerIndex(int index)
  {
    this->GetFilter()->SetLayerIndex(index);
  }

  template<class TInputImage, class TOutputLabel>
  int
  ExtractClassifyFilter<TInputImage, TOutputLabel>
  ::GetLayerIndex()
  {
    return this->GetFilter()->GetLayerIndex();
  }

  // Remove this when field name issue is resolved

  template<class TInputImage, class TOutput>
  void
  ExtractClassifyFilter<TInputImage, TOutput>
  ::SetFieldName(const std::string &name)
  {
    this->GetFilter()->SetFieldName(name);
  }

  template<class TInputImage, class TOutput>
  const std::string
  ExtractClassifyFilter<TInputImage, TOutput>
  ::GetFieldName()
  {
    return this->GetFilter()->GetFieldName();
  }

  template<class TInputImage, class TOutput>
  void
  ExtractClassifyFilter<TInputImage, TOutput>
  ::SetExtraFields(const std::vector<std::string> &name)
  {
    this->GetFilter()->SetExtraFields(name);
  }

  template<class TInputImage, class TOutput>
  const std::vector<std::string>
  ExtractClassifyFilter<TInputImage, TOutput>
  ::GetExtraFields()
  {
    return this->GetFilter()->GetExtraFields();
  }

} // end of namespace otb

#endif
