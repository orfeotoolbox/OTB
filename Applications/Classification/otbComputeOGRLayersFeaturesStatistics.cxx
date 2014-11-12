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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"
#include "otbStatisticsXMLFileWriter.h"
#include "itkVariableLengthVector.h"

#include <time.h>

namespace otb
{
namespace Wrapper
{
class ComputeOGRLayersFeaturesStatistics : public Application
{
public:
  typedef ComputeOGRLayersFeaturesStatistics Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  itkNewMacro(Self)
;

  itkTypeMacro(ComputeOGRLayersFeaturesStatistics, otb::Application)
;

private:
  void DoInit()
  {
    SetName("ComputeOGRLayersFeaturesStatistics");
    SetDescription("Compute statistics of the features in a set of OGR Layers");

    SetDocName("ComputeOGRLayersFeaturesStatistics");
    SetDocLongDescription("Compute statistics (mean and standard deviation) of the features in a set of OGR Layers, and write them in an XML file. This XML file can then be used by the training application.");
    SetDocLimitations("Experimental. For now only shapefiles are supported.");
    SetDocAuthors("David Youssefi during internship at CNES");
    SetDocSeeAlso("OGRLayerClassifier,TrainOGRLayersClassifier");
    AddDocTag(Tags::Segmentation);

    AddParameter(ParameterType_InputFilename, "inshp", "Name of the input shapefile");
    SetParameterDescription("inshp","Name of the input shapefile");

    AddParameter(ParameterType_InputFilename, "outxml", "XML file containing mean and variance of each feature.");
    SetParameterDescription("outxml", "XML file containing mean and variance of each feature.");

    AddParameter(ParameterType_ListView,  "feat", "List of features to consider for statistics.");
    SetParameterDescription("feat","List of features to consider for statistics.");
  }

  void DoUpdateParameters()
  {
    if ( HasValue("inshp") )
      {	
	const char * shapefile = GetParameterString("inshp").c_str();

	otb::ogr::DataSource::Pointer ogrDS;
	otb::ogr::Layer layer(NULL, false);

	OGRSpatialReference oSRS("");
	std::vector<std::string> options;
	
	ogrDS = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
	std::string layername = itksys::SystemTools::GetFilenameName(shapefile);
	layername = layername.substr(0,layername.size()-4);
	layer = ogrDS->GetLayer(0);

	otb::ogr::Feature feature = layer.ogr().GetNextFeature();
	ClearChoices("feat");
	for(unsigned int iField=0;iField<feature.ogr().GetFieldCount();iField++)
	  {
	    std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
	    key = item;
	    key.erase(std::remove(key.begin(), key.end(), ' '), key.end());
	    std::transform(key.begin(), key.end(), key.begin(), tolower);
	    key="feat."+key;
	    AddChoice(key,item);
	  }
      }
  }

  void DoExecute()
  {
      clock_t tic = clock();

      const char * shapefile = GetParameterString("inshp").c_str();
      const char * XMLfile = GetParameterString("outxml").c_str();

      otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
  
      otb::ogr::Layer layer = source->GetLayer(0); 
      bool goesOn = true;
      otb::ogr::Feature feature = layer.ogr().GetNextFeature();
  
      typedef double ValueType;
      typedef itk::VariableLengthVector<ValueType> MeasurementType;
      std::vector<MeasurementType> featValue;

      const int nbFeatures = GetSelectedItems("feat").size();
      
      if(feature.addr())
	while(goesOn)
	  {
	    MeasurementType mv; mv.SetSize(nbFeatures);
	    
	    for(unsigned int idx=0; idx < nbFeatures; ++idx)
	      mv[idx] = feature.ogr().GetFieldAsDouble(GetSelectedItems("feat")[idx]);
	    
	    featValue.push_back(mv);
	    feature = layer.ogr().GetNextFeature();
	    goesOn = feature.addr() != 0;
	  }
  
      MeasurementType mean; mean.SetSize(nbFeatures);
      MeasurementType stddev; stddev.SetSize(nbFeatures);

      for(unsigned int featIt=0;featIt<nbFeatures;featIt++){
	double sum = 0.0; for(unsigned add=0;add<featValue.size();add++)  sum += featValue[add][featIt];
	mean[featIt] =  sum / featValue.size();
	double accum = 0.0; for(unsigned add=0;add<featValue.size();add++) accum += (featValue[add][featIt] - mean[featIt]) * (featValue[add][featIt] - mean[featIt]);
	stddev[featIt] = sqrt(accum / (featValue.size()-1));}
  
      typedef otb::StatisticsXMLFileWriter<MeasurementType> StatisticsWriter;
      StatisticsWriter::Pointer writer = StatisticsWriter::New();
      writer->SetFileName(XMLfile);
      writer->AddInput("mean", mean);
      writer->AddInput("stddev", stddev);
      writer->Update();

      clock_t toc = clock();
      otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeOGRLayersFeaturesStatistics)


