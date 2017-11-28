/*
* Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
*
* This file is part of Orfeo Toolbox
*
* https://www.orfeo-toolbox.org/
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "itkVariableLengthVector.h"
#include "otbStatisticsXMLFileReader.h"
#include "itkListSample.h"
#include "otbShiftScaleSampleListFilter.h"
#include "otbDimensionalityReductionModelFactory.h"
#include <time.h>

namespace otb
{
namespace Wrapper
{
	
/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
{
return !std::isalnum(c);
}

class VectorDimensionalityReduction : public Application
{
	public:
	
		/** Standard class typedefs. */
  typedef VectorDimensionalityReduction Self;
  typedef Application Superclass;
		typedef itk::SmartPointer<Self> Pointer;
		typedef itk::SmartPointer<const Self> ConstPointer;
		
		/** Standard macro */
		itkNewMacro(Self);
		itkTypeMacro(Self, Application)
		
		/** Filters typedef */
		
		typedef float 															ValueType;
		typedef itk::VariableLengthVector<ValueType> 							InputSampleType;
		typedef itk::Statistics::ListSample<InputSampleType> 					ListSampleType;
		typedef MachineLearningModel<itk::VariableLengthVector<ValueType>, itk::VariableLengthVector<ValueType>>	DimensionalityReductionModelType;
		typedef DimensionalityReductionModelFactory<ValueType,ValueType> 		DimensionalityReductionModelFactoryType;
		typedef DimensionalityReductionModelType::Pointer 						ModelPointerType;
		
		/** Statistics Filters typedef */
		typedef itk::VariableLengthVector<ValueType> 										MeasurementType;
		typedef otb::StatisticsXMLFileReader<MeasurementType> 								StatisticsReader;
		typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;
  ~VectorDimensionalityReduction() ITK_OVERRIDE
  {
		DimensionalityReductionModelFactoryType::CleanFactories();
		}
		
	private:
	
		void DoInit() ITK_OVERRIDE
		{
		SetName("VectorDimensionalityReduction");
		SetDescription("Performs dimensionality reduction of the input vector data according to a model file.");
		SetDocName("Vector Dimensionality Reduction");
		SetDocAuthors("OTB-Team");
		SetDocLongDescription("This application performs a vector data dimensionality reduction based on a model file produced by the TrainDimensionalityReduction application.");
		SetDocSeeAlso("TrainDimensionalityReduction");
		AddDocTag(Tags::Learning);
		
		AddParameter(ParameterType_InputVectorData, "in", "Name of the input vector data");
		SetParameterDescription("in","The input vector data to reduce.");
		
		AddParameter(ParameterType_InputFilename, "instat", "Statistics file");
		SetParameterDescription("instat", "A XML file containing mean and standard deviation to center"
		"and reduce samples before dimensionality reduction (produced by ComputeImagesStatistics application).");
		MandatoryOff("instat");
		
		AddParameter(ParameterType_InputFilename, "model", "Model file");
		SetParameterDescription("model", "A model file (produced by the TrainDimensionalityReduction application,");
		
		AddParameter(ParameterType_ListView, "feat", "Input features to use for reduction."); //
		SetParameterDescription("feat","List of field names in the input vector data used as features for reduction."); //
		
		AddParameter(ParameterType_StringList, "featout", "Names of the new output features."); //
		SetParameterDescription("featout","List of field names for the output features which result from the reduction."); //
		
		AddParameter(ParameterType_OutputFilename, "out", "Output vector data file containing the reduced vector");
		SetParameterDescription("out","Output vector data file storing sample values (OGR format)."
                                        "If not given, the input vector data file is used. In overwrite mode, the original features will be lost.");
		MandatoryOff("out");
				
		AddParameter(ParameterType_Int, "indim", "Dimension of the input vector");
		SetParameterDescription("indim","Dimension of the whole input vector, this value is required if only a part of the bands contained in the vector are used."
		"If not given, the dimension is deduced from the length of the 'feat' parameter");
		MandatoryOff("indim");
		
		AddParameter(ParameterType_Int, "pcadim", "Principal component"); //
		SetParameterDescription("pcadim","This optional parameter can be set to reduce the number of eignevectors used in the PCA model file."); //
		MandatoryOff("pcadim");
		
		AddParameter(ParameterType_String, "mode", "Writting mode"); //
		SetParameterString("mode","overwrite", false);
		SetParameterDescription("mode","This parameter determines if the output file is overwritten or updated [overwrite/update]. If an output file name is given, the original file is copied before creating the new features."); //

		
		// Doc example parameter settings
		SetDocExampleParameterValue("in", "vectorData.shp");
		SetDocExampleParameterValue("instat", "meanVar.xml");
		SetDocExampleParameterValue("model", "model.txt");
		SetDocExampleParameterValue("out", "vectorDataOut.shp");
		SetDocExampleParameterValue("feat", "perimeter area width");
		SetDocExampleParameterValue("featout", "perimeter area width");
		//SetOfficialDocLink(); 
		}
		//   
		void DoUpdateParameters() ITK_OVERRIDE
		{
			
			if ( HasValue("in") )
			{
				
				std::string shapefile = GetParameterString("in");
				otb::ogr::DataSource::Pointer ogrDS;
				OGRSpatialReference oSRS("");
				std::vector<std::string> options;
				ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
				otb::ogr::Layer layer = ogrDS->GetLayer(0);
				OGRFeatureDefn &layerDefn = layer.GetLayerDefn();
				ClearChoices("feat");
				//ClearChoices("featout");
				
				for(int iField=0; iField< layerDefn.GetFieldCount(); iField++)
				{
					std::string item = layerDefn.GetFieldDefn(iField)->GetNameRef();
					std::string key(item);
					std::string::iterator end = std::remove_if( key.begin(), key.end(), IsNotAlphaNum );
					std::transform( key.begin(), end, key.begin(), tolower );
					/*
					key.erase( std::remove_if(key.begin(),key.end(),IsNotAlphaNum), key.end());
					std::transform(key.begin(), key.end(), key.begin(), tolower);*/
					//OGRFieldType fieldType = layerDefn.GetFieldDefn(iField)->GetType();
				/*	if(fieldType == OFTInteger || ogr::version_proxy::IsOFTInteger64(fieldType) || fieldType == OFTReal)
					{*/
						//std::string tmpKey="feat."+key;
						std::string tmpKey = "feat." + key.substr( 0, static_cast<unsigned long>( end - key.begin() ) );
						AddChoice(tmpKey,item);
					//}   // this is the same as in otbVectorClassifier, but it doesnt work
				}
				
			}
			
		}
		
		void DoExecute() ITK_OVERRIDE
		{
			clock_t tic = clock();
			
			
			std::string shapefile = GetParameterString("in");
			otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
			otb::ogr::Layer layer = source->GetLayer(0);
			ListSampleType::Pointer input = ListSampleType::New();
			int nbFeatures = GetSelectedItems("feat").size();
			
			input->SetMeasurementVectorSize(nbFeatures);
			otb::ogr::Layer::const_iterator it = layer.cbegin();
			otb::ogr::Layer::const_iterator itEnd = layer.cend();
			
			for( ; it!=itEnd ; ++it)
			{
				MeasurementType mv;
				mv.SetSize(nbFeatures);
				
				for(int idx=0; idx < nbFeatures; ++idx)
				{
					mv[idx] = static_cast<float>( (*it)[GetSelectedItems("feat")[idx]].GetValue<double>() );
					
				}
				input->PushBack(mv);
				
			}
				
			
			/** Statistics for shift/scale */
			
			MeasurementType meanMeasurementVector;
			MeasurementType stddevMeasurementVector;
			
			if (HasValue("instat") && IsParameterEnabled("instat"))
			{
				StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
				std::string XMLfile = GetParameterString("instat");
				statisticsReader->SetFileName(XMLfile);
				meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
				stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
			}
			else
			{
				meanMeasurementVector.SetSize(nbFeatures);
				meanMeasurementVector.Fill(0.);
				stddevMeasurementVector.SetSize(nbFeatures);
				stddevMeasurementVector.Fill(1.);
			}
			
			ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
			trainingShiftScaleFilter->SetInput(input);
			trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
			trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
			trainingShiftScaleFilter->Update();
			otbAppLogINFO("mean used: " << meanMeasurementVector);
			otbAppLogINFO("standard deviation used: " << stddevMeasurementVector);
			otbAppLogINFO("Loading model");
			
			
			/** Read the model */

			m_Model = DimensionalityReductionModelFactoryType::CreateDimensionalityReductionModel(GetParameterString("model"),
			DimensionalityReductionModelFactoryType::ReadMode);
			if (m_Model.IsNull())
			{
				otbAppLogFATAL(<< "Error when loading model " << GetParameterString("model") << " : unsupported model type");
			}
			if (HasValue("pcadim") && IsParameterEnabled("pcadim"))
			{
				int dimension = GetParameterInt("pcadim");
				m_Model->SetDimension(dimension );
			}

			
			m_Model->Load(GetParameterString("model"));
			otbAppLogINFO("Model loaded");
			
			/** Perform Dimensionality Reduction */
			
			ListSampleType::Pointer listSample = trainingShiftScaleFilter->GetOutput();
			ListSampleType::Pointer target = m_Model->PredictBatch(listSample);
			
			/** Create/Update Output Shape file */
						
			ogr::DataSource::Pointer output;
			ogr::DataSource::Pointer buffer = ogr::DataSource::New();
			bool updateMode = false;
				
			int nbBands = nbFeatures;
			if (HasValue("indim") && IsParameterEnabled("indim"))
				{nbBands = GetParameterInt("indim");} 
			
			
			if (IsParameterEnabled("out") && HasValue("out"))
			{
				// Create new OGRDataSource
				if (GetParameterString("mode")=="overwrite")
				{
                                output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Overwrite);
                                otb::ogr::Layer newLayer = output->CreateLayer(GetParameterString("out"),
                                                                               const_cast<OGRSpatialReference*>(layer.GetSpatialRef()),
                                                                               layer.GetGeomType());
                                // Copy existing fields
                                OGRFeatureDefn &inLayerDefn = layer.GetLayerDefn();
                                for (int k=0 ; k<inLayerDefn.GetFieldCount()-nbBands ; k++) // we don't copy the original bands 
                                  {
                                  OGRFieldDefn fieldDefn(inLayerDefn.GetFieldDefn(k));
                                  newLayer.CreateField(fieldDefn);
                                  }
				}
				else if (GetParameterString("mode")=="update")
				{
					//output = ogr::DataSource::New(GetParameterString("out"), ogr::DataSource::Modes::Update_LayerCreateOnly);
					// Update mode
					otb::ogr::DataSource::Pointer source_output = otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Read);
					layer = source_output->GetLayer(0);
					updateMode = true;
					otbAppLogINFO("Update input vector data.");
					
					// fill temporary buffer for the transfer
					otb::ogr::Layer inputLayer = layer;
					layer = buffer->CopyLayer(inputLayer, std::string("Buffer"));
					// close input data source 
					source_output->Clear();
					// Re-open input data source in update mode
					output = otb::ogr::DataSource::New(GetParameterString("out"), otb::ogr::DataSource::Modes::Update_LayerUpdate);
				
				}
				else
				{
					otbAppLogFATAL(<< "Error when creating the output file" << GetParameterString("mode") << " : unsupported writting mode type");
				}
				
			
			}
			

		
			
			otb::ogr::Layer outLayer = output->GetLayer(0);
			
			OGRErr errStart = outLayer.ogr().StartTransaction();
			
			if (errStart != OGRERR_NONE)
			{
				itkExceptionMacro(<< "Unable to start transaction for OGR layer " << outLayer.ogr().GetName() << ".");
			}
			
			// Add the field of prediction in the output layer if field not exist
			
			for (unsigned int i=0; i<GetParameterStringList("featout").size() ;i++)
			{
				OGRFeatureDefn &layerDefn = outLayer.GetLayerDefn();
				int idx = layerDefn.GetFieldIndex(GetParameterStringList("featout")[i].c_str());
				
				if (idx >= 0)
				{
					if (layerDefn.GetFieldDefn(idx)->GetType() != OFTReal)
					itkExceptionMacro("Field name "<< GetParameterStringList("featout")[i] << " already exists with a different type!");
				}
				else
				{
					OGRFieldDefn predictedField(GetParameterStringList("featout")[i].c_str(), OFTReal);
					ogr::FieldDefn predictedFieldDef(predictedField);
					outLayer.CreateField(predictedFieldDef);
				}
			}
			
		
			// Fill output layer
			
			unsigned int count=0;
			auto classfieldname = GetParameterStringList("featout");
			it = layer.cbegin();
			itEnd = layer.cend();
		
			for( ; it!=itEnd ; ++it, ++count)
			{
				ogr::Feature dstFeature(outLayer.GetLayerDefn());
			
				dstFeature.SetFrom( *it , TRUE);
				dstFeature.SetFID(it->GetFID());
			
				
				
				for (std::size_t i=0; i<classfieldname.size(); ++i){
					dstFeature[classfieldname[i]].SetValue<double>(target->GetMeasurementVector(count)[i]);
				}
				if (updateMode)
				{
					outLayer.SetFeature(dstFeature);
				}
				else
				{
					outLayer.CreateFeature(dstFeature);
				}
			}
			
			if(outLayer.ogr().TestCapability("Transactions"))
			{
				const OGRErr errCommitX = outLayer.ogr().CommitTransaction();
				if (errCommitX != OGRERR_NONE)
				{
					itkExceptionMacro(<< "Unable to commit transaction for OGR layer " << outLayer.ogr().GetName() << ".");
				}
			}
			output->SyncToDisk();
			clock_t toc = clock();
			otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
		}
		
		ModelPointerType m_Model;
};
}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDimensionalityReduction)
