#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbStreamingStatisticsImageFilter.h"
#include "otbExtractROI.h"
#include "otbOGRFeatureWrapper.h"
#include "otbOGRLayerWrapper.h"
#include "otbOGRIOHelper.h"
#include "otbLabelImageToOGRDataSourceFilter.h"
#include "otbOGRDataSourceWrapper.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkConstNeighborhoodIterator.h"
#include "otbOGRDataToSamplePositionFilter.h"
#include "otbRandomSampler.h"
#include "otbOGRDataSourceToLabelImageFilter.h"
#include "otbImageSampleExtractorFilter.h"
#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbWrapperCompositeApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbExtractClassifyFilter.h"
#include "ogr_api.h"
#include <limits>
#include <unordered_set>

#include <chrono>
#include <ctime>

namespace otb
{
  namespace Wrapper
  {
    class TrainAutoContext : public CompositeApplication
    {
    public:
      typedef TrainAutoContext                                                         Self;
      typedef Application                                                              Superclass;
      typedef itk::SmartPointer<Self>                                                  Pointer;
      typedef itk::SmartPointer<const Self>                                            ConstPointer;
      
      typedef unsigned int                                                             LabelType;
      typedef otb::Image<LabelType>                                                    LabelImageType;
      typedef otb::ImageFileReader<LabelImageType>                                     LabelReaderType;
      typedef otb::ImageFileWriter<LabelImageType>                                     LabelWriterType;
      typedef otb::VectorImage<LabelType>                                              LabelVectorImageType;
      typedef otb::VectorImage<LabelType>::PixelType                                   LabelVectorImagePixelType;
      typedef otb::ImageFileWriter<LabelVectorImageType>                               LabelVectorWriterType;
      typedef otb::ImageFileReader<LabelVectorImageType>                               LabelVectorReaderType;
      typedef float                                                                    ComponentType;
      typedef otb::VectorImage<ComponentType>                                          FloatVectorImageType;
      typedef otb::ImageFileReader<FloatVectorImageType>                               FloatVectorImageReaderType;
      typedef otb::ImageFileWriter<FloatVectorImageType>                               FloatVectorImageWriterType;
      typedef typename FloatVectorImageType::PixelType                                 SampleType;
      typedef otb::StreamingStatisticsImageFilter<LabelImageType>                      StatisticsImageFilterType;
      typedef otb::ExtractROI<LabelType,LabelType>                                     ExtractROIFilterType;
      typedef itk::ImageRegionConstIterator<LabelImageType>                            LabelImageIterator;
      typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType>                     LabelImageToOGRDataSourceFilterType;
      typedef otb::VectorDataFileWriter<VectorDataType>                                VectorDataFileWriterType;
      typedef otb::OGRDataToSamplePositionFilter<LabelImageType, LabelImageType,otb::RandomSampler> RandomSamplerType;

      typedef otb::SamplingRateCalculator                                               RateCalculatorType;
      typedef otb::OGRDataSourceToLabelImageFilter<LabelImageType>                      OGRDataSourceToMapFilterType;
      typedef otb::VectorImage<float>                                                   VectorImageType;
      typedef std::map<std::string, unsigned long>                                      ClassCountMapType;
      typedef RateCalculatorType::MapRateType                                           MapRateType;
      typedef otb::OGRDataToClassStatisticsFilter<LabelImageType,LabelImageType>        ClassStatFilterType;
      typedef otb::ConcatenateVectorImageFilter< VectorImageType, VectorImageType, LabelVectorImageType > ConcatenateImageFilter;
      //~ typedef otb::ConcatenateVectorImageFilter< LabelVectorImageType, LabelVectorImageType, LabelVectorImageType > ConcatenateImageFilter;

      typedef otb::ImageToVectorImageCastFilter< LabelImageType, VectorImageType >      CastFilterType;
      typedef std::unordered_map<LabelType,std::vector<double> >                        LabeledVectorMapType;
      typedef std::unordered_map<LabelType,LabelType>                                   LabeledIntMapType;
      typedef RAMDrivenAdaptativeStreamingManager<FloatVectorImageType>                 RAMDrivenAdaptativeStreamingManagerType;
      typedef FloatVectorImageType::RegionType                                          RegionType;

      itkNewMacro(Self);
      itkTypeMacro(Merging, otb::CompositeApplication);

    private:

      void DoInit()
      {
	ClearApplications();
	SetName("TrainAutoContext");
	SetDescription("");

	SetDocLongDescription(" ");
	SetDocLimitations(" ");
	SetDocAuthors(" ");
	SetDocSeeAlso(" ");

	AddParameter(ParameterType_InputImage,  "in",    "Input image");
	SetParameterDescription( "in", "The input image." );
   
	AddParameter(ParameterType_String, "inseg", "Input segmentation");
	SetParameterDescription( "inseg", "Input raster containing the segment label IDs" );

	AddParameter(ParameterType_Directory,  "tmpdir",    "Temporary directory");

        AddParameter(ParameterType_String, "refdata", "Reference data");
	SetParameterDescription( "refdata", "Shapefile containing labeled points for training." );
	
	AddParameter(ParameterType_String, "field", "Label field");
	SetParameterDescription( "refdata", "Field in reference data containing the training labels." );

	AddParameter(ParameterType_String, "out", "output path");
	SetParameterDescription( "out", "Path for storing results" );
		
	AddParameter(ParameterType_Int, "nit", "number of iterations");
	SetParameterDescription( "nit", "Number of iterations for AutoContext" );
	SetDefaultParameterInt("nit",3);
	SetMinimumParameterIntValue("nit",1);
	
	AddRAMParameter();
	
	GDALSetCacheMax(0);
	
	ClearApplications();
	AddApplication("TrainVectorClassifier", "train", "train vector classifier");
	AddApplication("VectorClassifier","classifier","Vector Classifier");
	
      }

      void DoUpdateParameters()
      {
      }

      void DoExecute()
      {
    otbAppLogINFO("Start Training AutoContext");
    unsigned int threadsNumber = 12;
	//~ LabelVectorReaderType::Pointer vreader = LabelVectorReaderType::New();
	//~ vreader->SetFileName(GetParameterString("in"));
	//~ vreader->UpdateOutputInformation();
	//~ auto imageIn = vreader->GetOutput();
	VectorImageType::Pointer imageIn = GetParameterImage("in");
    imageIn->UpdateOutputInformation();

	LabelReaderType::Pointer lreader = LabelReaderType::New();
	lreader->SetFileName(GetParameterString("inseg"));
	lreader->UpdateOutputInformation();
	LabelImageType::Pointer imageSeg = lreader->GetOutput();

    //~ About streaming 
    RegionType largestRegion = imageSeg->GetLargestPossibleRegion();
    
    RAMDrivenAdaptativeStreamingManagerType::Pointer
              streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
    int availableRAM = GetParameterInt("ram");
    streamingManager->SetAvailableRAMInMB(availableRAM);
    float bias = 2.0; // empiric value;
    streamingManager->SetBias(bias);
    streamingManager->PrepareStreaming(imageSeg, largestRegion);
    
	CastFilterType::Pointer castFilter = CastFilterType::New();
	castFilter->SetInput(imageSeg);
	castFilter->UpdateOutputInformation();
	
	const std::string tmpdir = GetParameterString("tmpdir");
	const unsigned ram = GetParameterInt("ram");
	const std::string field = GetParameterString("field");
    otbAppLogINFO("Preparing data");
	// Vectorize segmentation
	otb::ogr::DataSource::Pointer SPDataSource = vectorizeNoStreaming(imageSeg,tmpdir);
	otb::ogr::Layer SPLayer = SPDataSource->GetLayerChecked(0);

	//Intersection with reference data samples
	otb::ogr::DataSource::Pointer refLabelDataSource = otb::ogr::DataSource::New(GetParameterString("refdata"), otb::ogr::DataSource::Modes::Read);

	//Create list of labels
	std::vector<LabelType> labelList;
	auto refLabelLayer = refLabelDataSource->GetLayerChecked(0);
	otb::ogr::Layer::feature_iter<otb::ogr::Feature> refLabelIt;
	for (refLabelIt=refLabelLayer.begin();refLabelIt!=refLabelLayer.end(); refLabelIt++) {
	  //std::set will sort and only insert if unique
	  labelList.push_back((*refLabelIt)[field].GetValue<long long>());
	}
	std::sort(labelList.begin(),labelList.end());
	auto last = std::unique(labelList.begin(),labelList.end());
	labelList.erase(last,labelList.end());
	std::cout << "Labels found : ";
	for (auto e : labelList) {
	  std::cout << e << " ";
	}
	std::cout << "\n";
	const unsigned labelListSize=labelList.size();

	std::string tempName = tmpdir+ "/featExtract.shp";
	
	otb::ogr::DataSource::Pointer inter = extractFeatures<VectorImageType>(castFilter->GetOutput(), refLabelDataSource , tempName, "SPID", field, ram, threadsNumber);

    otbAppLogINFO("Add SuperPixel ID to reference : Done");
	//Store labels of intersection
	std::unordered_set<int> interSet;
	auto l = inter->GetLayerChecked(0);
	otb::ogr::Layer::feature_iter<otb::ogr::Feature> fi;
	for (fi=l.begin();fi!=l.end(); fi++) {
	  for (int i = 0; i < fi->GetSize(); i++) {
	    interSet.insert((*fi)["SPID0"].GetValue<double>());
	  }
	}

    otbAppLogINFO("Start sampling SuperPixels at 100% rate");
	auto outSamples = fullSampleSelection(lreader->GetOutput(),
                                          tmpdir, ram, interSet, streamingManager,
                                          threadsNumber);
    otbAppLogINFO("sampling SuperPixels at 100% rate : Done");
	//Rasterize ref data
	auto rasterFilter = OGRDataSourceToMapFilterType::New();
	rasterFilter->AddOGRDataSource(refLabelDataSource);
	rasterFilter->SetOutputSize(lreader->GetOutput()->GetLargestPossibleRegion().GetSize());
	rasterFilter->SetOutputOrigin(lreader->GetOutput()->GetOrigin());
	rasterFilter->SetOutputSpacing(lreader->GetOutput()->GetSignedSpacing());
	rasterFilter->SetBackgroundValue(0);
	rasterFilter->SetBurnAttributeMode(true);
	rasterFilter->SetBurnAttribute(field);
	rasterFilter->SetOutputProjectionRef(lreader->GetOutput()->GetProjectionRef());
	rasterFilter->UpdateOutputInformation();
  
	auto refRasterCast = CastFilterType::New();
	refRasterCast->SetInput(rasterFilter->GetOutput());
	refRasterCast->UpdateOutputInformation();

	//Set up image concatenation
	ConcatenateImageFilter::Pointer concatImRefData = ConcatenateImageFilter::New();
	concatImRefData->SetInput1(imageIn);
	concatImRefData->SetInput2(refRasterCast->GetOutput());

	//Extract features only on labeled samples for training
	const std::string initTrainSamples_s = tmpdir+"/initTrainSamples.shp";
    otbAppLogINFO("Start sample Extraction : initialize training sample-set");
	auto initTrainSamples = extractFeatures<VectorImageType>(imageIn, inter, initTrainSamples_s, "feature",field, 1000);
	initTrainSamples->SyncToDisk();
    otbAppLogINFO("Start sample Extraction : initialize training sample-set : DONE");

	//Setup first iteration of training
    otbAppLogINFO("Start training firts iteration");
	auto VectorTrainer = GetInternalApplication("train");
	std::string modelName=GetParameterString("out") + "model_it_0.rf";
	
	VectorTrainer->SetParameterString("io.out",modelName);
	VectorTrainer->SetParameterStringList("io.vd",{initTrainSamples_s});
	// VectorTrainer->SetParameterString("io.confmatout","/disk2/DATA/temp/confMatTest");
	std::vector<std::string> featureList;
	for (unsigned i = 0; i < imageIn->GetNumberOfComponentsPerPixel(); i++) {
	  std::stringstream s;
	  s << "feature" << i;
	  featureList.push_back(s.str());
	}
    
    //~ //use sharkrf could improve results and computational time
	UpdateInternalParameters("train");
	VectorTrainer->SetParameterStringList("feat",featureList);
	UpdateInternalParameters("train");
	VectorTrainer->SetParameterStringList("cfield",{field});
	VectorTrainer->SetParameterString("classifier","rf");
	VectorTrainer->SetParameterString("rand","0");
	UpdateInternalParameters("train");
	VectorTrainer->ExecuteAndWriteOutput();
    otbAppLogINFO("Training firts iteration : DONE");

	auto trainSamples = otb::ogr::DataSource::New();
	//Extract and classify features for all samples
	std::vector<std::string> histoNames = std::vector<std::string>();
	for (int it = 1; it <= this->GetParameterInt("nit"); it++) {
      otbAppLogINFO("Start iteration " << it);
	  if(it==1){
	    //Use "outSamples" which doesn't contain histogram fields
	    extractFeaturesAndClassify<VectorImageType>(imageIn, outSamples, trainSamples, modelName, "predicted", histoNames, "label", 1000);

	    for (unsigned i = 0; i < labelListSize; i++) {
	      std::stringstream s;
	      s << "histo" << labelList[i];
	      featureList.push_back(s.str());
	      histoNames.push_back(s.str());
	    }
	  }
	  else{
      otbAppLogINFO("Extract SuperPixels features, then Classify pixels");
	  otb::ogr::DataSource::Pointer trainSamplesNew = otb::ogr::DataSource::New();
	  extractFeaturesAndClassify<VectorImageType>(imageIn, trainSamples, trainSamplesNew, modelName, "predicted", histoNames, "label", 1000);
      otbAppLogINFO("Extract SuperPixels features, then Classify pixels : DONE");
	  trainSamples=trainSamplesNew;
	  }
	  //Create histograms
      otbAppLogINFO("Calculate histograms");
	  LabeledVectorMapType histos;
	  LabeledIntMapType counts;
	  
	  auto classifiedPointsLayer = trainSamples->GetLayerChecked(0);
	  otb::ogr::Layer::feature_iter<otb::ogr::Feature> classifiedPointsIt;
	  for (classifiedPointsIt=classifiedPointsLayer.begin();classifiedPointsIt!=classifiedPointsLayer.end(); classifiedPointsIt++) {
	    const int predictedLabel=(*classifiedPointsIt)["predicted"].GetValue<int>();
	    const int spLabel=(*classifiedPointsIt)["label"].GetValue<int>();
	    auto histoIt = histos.find(spLabel);
	    if (histoIt != histos.end()) {
	      //Add to exisiting histogram
	      histoIt->second[std::find(labelList.begin(),labelList.end(),predictedLabel)-labelList.begin()]++;
	      counts.find(spLabel)->second++;
	    }
	    else{
	      //Create new histogram
	      histos.insert(std::pair<int,std::vector<double> >(spLabel,std::vector<double>(labelList.size(),0.0)));
	      counts.insert(std::pair<LabelType,unsigned int>(spLabel,0));
	    }
	  }
      otbAppLogINFO("Write histograms init : first iteration");
	  //For immediate training of the new model
	  writeHistograms(initTrainSamples, labelList, histos, counts,"SPID0", (it > 1));
	  initTrainSamples->SyncToDisk();

      otbAppLogINFO("Write histograms all");
	  //For next iteration, avoid doing on last iteration
	  if (it < this->GetParameterInt("nit")) {
	    writeHistograms(trainSamples, labelList, histos, counts,"label",(it > 1));
	  }
	  //Update model name
      otbAppLogINFO("Restart training");
	  std::stringstream modelName_s;
	  modelName_s << GetParameterString("out") << "model_it_" << it << ".rf";
	  modelName=modelName_s.str();
	  VectorTrainer->SetParameterString("io.out",modelName);
	  VectorTrainer->SetParameterStringList("io.vd",{initTrainSamples_s});
	  
      //~ //use sharkrf could improve results and computational time
	  UpdateInternalParameters("train");
	  VectorTrainer->SetParameterStringList("feat",featureList);
	  UpdateInternalParameters("train");
	  VectorTrainer->SetParameterStringList("cfield",{field});
	  VectorTrainer->SetParameterString("classifier","rf");
	  VectorTrainer->SetParameterString("rand","0");
	  UpdateInternalParameters("train");
	  ExecuteInternal("train");
	}
	//TODO cleanup temp dir
      }

      typename otb::ogr::DataSource::Pointer vectorizeNoStreaming(typename LabelImageType::Pointer labelIn, std::string tmpdir){
	labelIn->UpdateOutputInformation();

	auto projRef = labelIn->GetProjectionRef();
	otb::ogr::DataSource::Pointer ogrDS;
	otb::ogr::Layer layer(ITK_NULLPTR, false);

	OGRSpatialReference oSRS(projRef.c_str());
	std::vector<std::string> options;

	std::stringstream shapefile;
	shapefile << tmpdir << "/tempShapeFile.shp";
  
	ogrDS = otb::ogr::DataSource::New(shapefile.str(), otb::ogr::DataSource::Modes::Overwrite);
	std::string layername = itksys::SystemTools::GetFilenameName(shapefile.str().c_str());
	std::string extension = itksys::SystemTools::GetFilenameLastExtension(shapefile.str().c_str());
	layername = layername.substr(0,layername.size()-(extension.size()));
	layer = ogrDS->CreateLayer(layername, &oSRS, wkbMultiPolygon, options);

	OGRFieldDefn labelField("label", OFTInteger);
	layer.CreateField(labelField, true);

	//Raster->Vecteur conversion
	LabelImageToOGRDataSourceFilterType::Pointer labelToOGR = LabelImageToOGRDataSourceFilterType::New();
	labelToOGR->SetInput(labelIn);
	labelToOGR->SetInputMask(labelIn);
	labelToOGR->SetFieldName("label");
	labelToOGR->Update();

	otb::ogr::DataSource::ConstPointer ogrDSTmp = labelToOGR->GetOutput();
	otb::ogr::Layer layerTmp = ogrDSTmp->GetLayerChecked(0);

	otb::ogr::Layer::const_iterator featIt = layerTmp.begin();
	for(; featIt!=layerTmp.end(); ++featIt)
	  {
	    otb::ogr::Feature dstFeature(layer.GetLayerDefn());
	    dstFeature.SetFrom( *featIt, TRUE );
	    layer.CreateFeature( dstFeature );
	  }

	layer.ogr().CommitTransaction();

	return ogrDS;
      }

    template<typename TInputImageType>
    void extractFeaturesAndClassify(typename TInputImageType::Pointer im,
                                    otb::ogr::DataSource::Pointer const& inputDS,
                                    otb::ogr::DataSource::Pointer &outputDS,
                                    std::string modelName,
                                    std::string classField,
                                    std::vector<std::string> extraFieldNames,
                                    std::string randomFieldName="label",
                                    unsigned ram=512)
    {
        typedef otb::ExtractClassifyFilter<TInputImageType, LabelType> ExtractClassifyFilterType;
        typename ExtractClassifyFilterType::Pointer filter = ExtractClassifyFilterType::New();
        filter->SetSamplePositions(inputDS);
        filter->SetInput(im);
        filter->SetOutputSamples(outputDS);
        filter->SetLayerIndex(0);
        filter->SetLabelFieldName(classField);
        filter->SetModelString(modelName);
        // Remove when field name issue is resolved
        filter->SetFieldName(randomFieldName);
        if (!extraFieldNames.empty()){
          filter->SetExtraFields(extraFieldNames);
        }
        filter->GetStreamer()->SetAutomaticAdaptativeStreaming(ram);
        filter->Update();
        // outputDS->SyncToDisk();
    }

//~ otb::ogr::DataSource::Pointer extractFeatures(typename TInputImageType::Pointer im,
    template<typename TInputImageType>
    otb::ogr::DataSource::Pointer extractFeatures(typename TInputImageType::Pointer im,
                                                  otb::ogr::DataSource::Pointer positions,
                                                  std::string outputName,
                                                  std::string outputPrefix,
                                                  std::string randomFieldName,
                                                  unsigned ram=512,
                                                  unsigned int threadsNumber=1)
    {
        otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(outputName,otb::ogr::DataSource::Modes::Overwrite);
        typedef otb::ImageSampleExtractorFilter<TInputImageType> ExtractionFilterType;
        typename ExtractionFilterType::Pointer filter = ExtractionFilterType::New();
        filter->SetNumberOfThreads(threadsNumber);
        filter->SetInput(im);
        filter->SetLayerIndex(0);
        filter->SetSamplePositions(positions);
        filter->SetOutputSamples(output);
        filter->SetClassFieldName(randomFieldName);
        filter->SetOutputFieldPrefix(outputPrefix);
        std::vector<std::string> nameList;
        filter->SetOutputFieldNames(nameList);
        filter->GetStreamer()->SetAutomaticAdaptativeStreaming(ram);
        filter->Update();
        output->SyncToDisk();
        return output;
    }

    otb::ogr::DataSource::Pointer fullSampleSelection(LabelImageType::Pointer inputIm,
                                                      std::string tmpdir,
                                                      unsigned ram,
                                                      const std::unordered_set<int> & SP_id,
                                                      RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager,
                                                      unsigned int threadsNumber=1){

    auto slic_geo = inputIm->GetGeoTransform();
    unsigned long numberOfStreamDivisions = streamingManager->GetNumberOfSplits();
    std::vector<std::tuple<int, double, double>> pixels_of_interest;

    //~ start streaming by piece
    for(size_t piece=0;piece<numberOfStreamDivisions;++piece){
        RegionType streamingRegion = streamingManager->GetSplit(piece);
        inputIm->SetRequestedRegion(streamingRegion);
        inputIm->PropagateRequestedRegion();
        inputIm->UpdateOutputData();
        //~ otbAppLogINFO("Processing region: "<<streamingRegion);
        LabelImageIterator in(inputIm, inputIm->GetRequestedRegion());
        //~ get all super pixels of interest geo coordinates
        for (in.GoToBegin(); !in.IsAtEnd(); ++in)
          {
            auto SP_value = in.Get();
            if (SP_id.find(SP_value) != SP_id.end())
            {
                auto coords = in.GetIndex();
                double x_geo_coord = double(slic_geo[0]) + double(slic_geo[1]) / double(2.0) + double(slic_geo[1]) * double(coords[0]);
                double y_geo_coord = double(slic_geo[3]) + double(slic_geo[5]) / double(2.0) + double(slic_geo[5]) * double(coords[1]);
                pixels_of_interest.push_back(std::tuple<int, double, double>{SP_value, x_geo_coord, y_geo_coord});
            }
          }
    }
	std::stringstream tempName;
    tempName << tmpdir << "/fullSampleExtraction.shp";
    
	otb::ogr::DataSource::Pointer outputSamples = otb::ogr::DataSource::New(tempName.str(), otb::ogr::DataSource::Modes::Overwrite);
    std::string layername = itksys::SystemTools::GetFilenameName(tempName.str().c_str());
    std::string extension = itksys::SystemTools::GetFilenameLastExtension(tempName.str().c_str());
    
    otb::ogr::Layer layer(ITK_NULLPTR, false);
    auto projRef = inputIm->GetProjectionRef();
	OGRSpatialReference oSRS(projRef.c_str());
	std::vector<std::string> options;
    
    layername = layername.substr(0,layername.size() - (extension.size()));
    layer = outputSamples->CreateLayer(layername, &oSRS, wkbPoint, options);
    OGRFieldDefn labelField("label", OFTInteger64);
	layer.CreateField(labelField, true);

    for (auto&& i : pixels_of_interest)
    {
        int SP_val = std::get<0>(i);
        double coord_x = std::get<1>(i);
        double coord_y = std::get<2>(i);
        
        OGRFeature *poFeature;
        poFeature = OGRFeature::CreateFeature(&layer.GetLayerDefn());
        poFeature->SetField("label", SP_val);
        
        OGRPoint pt;
        pt.setX(coord_x);
        pt.setY(coord_y);
        poFeature->SetGeometry(&pt);
        layer.CreateFeature(poFeature);
    }
    layer.ogr().CommitTransaction();
    outputSamples->SyncToDisk();

	return outputSamples;
      }

      void writeHistograms(otb::ogr::DataSource::Pointer &classifiedPoints, std::vector<LabelType> labelList, LabeledVectorMapType & histos, LabeledIntMapType & counts, std::string fieldName, bool overwrite=false){

        const unsigned labelListSize=labelList.size();
        auto classifiedPointsLayer = classifiedPoints->GetLayerChecked(0);
        std::vector<std::string> histoNames;
        if (!overwrite) {
          for (const auto e : labelList) {
            std::stringstream s;
            s << "histo" << e;
            histoNames.push_back(s.str());
            OGRFieldDefn histoField(s.str().c_str(), OFTReal);
            otb::ogr::FieldDefn histoFieldDef(histoField);
            classifiedPointsLayer.CreateField(histoFieldDef);
            // std::cout << "Creating field " << s.str() << "in" << classifiedPoints_s << "\n";
          }
        }
        for (auto classifiedPointsIt=classifiedPointsLayer.begin();classifiedPointsIt!=classifiedPointsLayer.end(); classifiedPointsIt++) {
          const LabelType spLabel = (*classifiedPointsIt)[fieldName].GetValue<long long>();
          const std::vector<double> histo = histos[spLabel];
          const unsigned int count = counts[spLabel];
          otb::ogr::Feature dstFeature(classifiedPointsLayer.GetLayerDefn());
          dstFeature.SetFrom( *classifiedPointsIt , TRUE);
          dstFeature.SetFID(classifiedPointsIt->GetFID());
          for (unsigned i = 0; i < labelListSize; i++) {
            std::stringstream s;
            s << "histo" << labelList[i];
            dstFeature[s.str()].SetValue<double>(histo[i]/count);
          }
          classifiedPointsLayer.SetFeature(dstFeature);
        }
        classifiedPointsLayer.ogr().CommitTransaction();
        // classifiedPoints->SyncToDisk();
      }

 
    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainAutoContext)
