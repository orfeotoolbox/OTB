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

      typedef otb::ImageToVectorImageCastFilter< LabelImageType, VectorImageType >      CastFilterType;
      typedef std::unordered_map<LabelType,std::vector<double> >                        LabeledVectorMapType;
      typedef std::unordered_map<LabelType,LabelType>                                   LabeledIntMapType;
      typedef RAMDrivenAdaptativeStreamingManager<FloatVectorImageType>                 RAMDrivenAdaptativeStreamingManagerType;
      typedef FloatVectorImageType::RegionType                                          RegionType;
      typedef otb::ObjectList<FloatVectorImageType> FloaVectorImageListType;

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

        AddParameter(ParameterType_InputImageList,  "il",    "Input image list");
        SetParameterDescription( "il", "Image-list representing features" );

        AddParameter(ParameterType_StringList, "inseg", "Input segmentation list");
        SetParameterDescription( "inseg", "Input raster containing the segment label IDs" );

        AddParameter(ParameterType_Directory,  "tmpdir",    "Temporary directory");

        AddParameter(ParameterType_StringList, "refdata", "Reference data list");
        SetParameterDescription( "refdata", "Shapefile containing labeled points for training." );

        AddParameter(ParameterType_String, "field", "Label field");
        SetParameterDescription( "field", "Field in reference data containing the training labels." );

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
    {}

    void DoExecute()
    {
        unsigned int threadsNumber = 12;
        FloatVectorImageListType::Pointer in_img_list = GetParameterImageList("il");
        const std::vector<std::string> in_seg_list = GetParameterStringList("inseg");
        const std::vector<std::string> in_ref_list = GetParameterStringList("refdata");

        const std::string tmpdir = GetParameterString("tmpdir");
        const unsigned ram = GetParameterInt("ram");
        std::string field = GetParameterString("field");

        //~ Some checks
        const unsigned int nbImages = in_img_list->Size();
        const unsigned int nbSeg = in_seg_list.size();
        const unsigned int nbRef = in_ref_list.size();

        if (nbImages == 0)
        {
           itkExceptionMacro("No input Image set...; please set at least one input image");
        }
        if (nbImages != nbSeg || nbImages != nbRef)
        {
            itkExceptionMacro("number of input image, segmentation, reference must be the same");
        }
        //~ TODO : check if every input image get the same number of bands

        std::vector<otb::ogr::DataSource::Pointer> ref_extracted;
        std::vector<std::string> ref_paths;
        std::vector<otb::ogr::DataSource::Pointer> SP_points;
        
        std::vector<LabelType> labelList;
        //~ iterate over inputs
        for (size_t index = 0; index < nbImages; ++index)
        {
            auto index_string = std::to_string(index);
            otbAppLogINFO("Processing images at index : " << index_string);
            
            VectorImageType::Pointer imageIn = in_img_list->GetNthElement(index);
            imageIn->UpdateOutputInformation();
            
            std::string in_seg = in_seg_list[index];
            
            otbAppLogINFO("Processing segmentation : " << in_seg);
            
            LabelReaderType::Pointer lreader = LabelReaderType::New();
            lreader->SetFileName(in_seg);
            lreader->UpdateOutputInformation();
            LabelImageType::Pointer imageSeg = lreader->GetOutput();

            std::string in_ref = in_ref_list[index];

            otbAppLogINFO("Processing reference data : " << in_ref);
            
            //~ About streaming
            RegionType largestRegion = imageSeg->GetLargestPossibleRegion();
            RAMDrivenAdaptativeStreamingManagerType::Pointer
                      streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
            int availableRAM = GetParameterInt("ram");
            streamingManager->SetAvailableRAMInMB(availableRAM);
            float bias = 2.0; // empiric value;
            streamingManager->SetBias(bias);
            streamingManager->PrepareStreaming(imageSeg, largestRegion);

            otbAppLogINFO("streamingManager : OK");
            
            CastFilterType::Pointer castFilter = CastFilterType::New();
            castFilter->SetInput(imageSeg);
            castFilter->UpdateOutputInformation();

            otbAppLogINFO("Preparing data");
            // Vectorize segmentation
            otb::ogr::DataSource::Pointer SPDataSource = vectorizeNoStreaming(imageSeg,tmpdir);
            otb::ogr::Layer SPLayer = SPDataSource->GetLayerChecked(0);

            //Intersection with reference data samples
            otb::ogr::DataSource::Pointer refLabelDataSource = otb::ogr::DataSource::New(in_ref, otb::ogr::DataSource::Modes::Read);

            //Create list of labels
            if (index==0)
            {
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
            }
            std::string tempName = tmpdir+ "/featExtract.sqlite";

            otb::ogr::DataSource::Pointer inter = extractFeatures<VectorImageType>(castFilter->GetOutput(),
                                                                                   refLabelDataSource,
                                                                                   tempName, "SPID",
                                                                                   field, ram,
                                                                                   threadsNumber);

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
                                                  tmpdir,
                                                  "fullSampleExtraction_"+index_string+".shp",
                                                  ram, interSet, streamingManager,
                                                  threadsNumber);
            SP_points.push_back(outSamples);
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

            //~ auto refRasterCast = CastFilterType::New();
            //~ refRasterCast->SetInput(rasterFilter->GetOutput());
            //~ refRasterCast->UpdateOutputInformation();

            //~ //Set up image concatenation
            //~ ConcatenateImageFilter::Pointer concatImRefData = ConcatenateImageFilter::New();
            //~ concatImRefData->SetInput1(imageIn);
            //~ concatImRefData->SetInput2(refRasterCast->GetOutput());

            //Extract features only on labeled samples for training
            
            const std::string initTrainSamples_s = tmpdir + "/initTrainSamples_"+index_string+".sqlite";
            ref_paths.push_back(initTrainSamples_s);
            otbAppLogINFO("Start sample Extraction : initialize training sample-set");
            auto initTrainSamples = extractFeatures<VectorImageType>(imageIn, inter,
                                                                     initTrainSamples_s,
                                                                     "feature",
                                                                     field, availableRAM);
            ref_extracted.push_back(initTrainSamples);
            otbAppLogINFO("Start sample Extraction : initialize training sample-set : DONE");
        }//for iterate over inputs

        otbAppLogINFO("Merge refererence");
        const std::string initTrainSamples_s = tmpdir + "/initTrainSamples_full.sqlite";
        auto init_train_sample_full = merge_vectors(ref_extracted, initTrainSamples_s);


        std::vector<std::string> featureList;
        for (unsigned i = 0; i < in_img_list->GetNthElement(0)->GetNumberOfComponentsPerPixel(); i++) {
          std::stringstream s;
          s << "feature" << i;
          featureList.push_back(s.str());
        }

        //Setup first iteration of training
        
        std::transform(field.begin(), field.end(), field.begin(), [](unsigned char c){ return std::tolower(c); });
    
        otbAppLogINFO("Start training firts iteration");
        auto VectorTrainer = GetInternalApplication("train");

        std::string modelName = GetParameterString("out") + "model_it_0.rf";
        VectorTrainer->SetParameterString("io.out",modelName);
        VectorTrainer->SetParameterStringList("io.vd",{initTrainSamples_s});
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterStringList("feat",featureList);
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterStringList("cfield",{field});
        VectorTrainer->SetParameterString("classifier","rf");
        VectorTrainer->SetParameterString("rand","0");
        UpdateInternalParameters("train");
        VectorTrainer->ExecuteAndWriteOutput();
        otbAppLogINFO("Training firts iteration : DONE");

        //Extract and classify features for all samples
        //~ auto trainSamples = otb::ogr::DataSource::New();
        std::vector<otb::ogr::DataSource::Pointer> trainSamples_tiles;
        std::vector<std::vector<std::string>> histoNames_tiles;
        //initialize trainSamples_tiles
        for (size_t index=0; index < nbImages; ++index)
        {
            trainSamples_tiles.push_back(otb::ogr::DataSource::New());
            histoNames_tiles.push_back(std::vector<std::string>());
        }

        for (int it=1; it <= this->GetParameterInt("nit"); it++)
        {
            otbAppLogINFO("Start iteration " << it);
            const unsigned labelListSize=labelList.size();
            for (size_t index=0; index < nbImages; ++index)
            {
                auto histoNames = histoNames_tiles[index];
                //~ std::vector<std::string> histoNames;
                VectorImageType::Pointer imageIn = in_img_list->GetNthElement(index);
                imageIn->UpdateOutputInformation();
                
                auto outSamples = SP_points[index];
                auto ref_samples = ref_extracted[index];

                if(it==1)
                {
                    //Use "outSamples" which doesn't contain histogram fields
                    extractFeaturesAndClassify<VectorImageType>(imageIn, outSamples,
                                                                trainSamples_tiles[index], modelName,
                                                                "predicted", histoNames,
                                                                "label", this->GetParameterInt("ram"));
                    for (unsigned i = 0; i < labelListSize; i++){
                        std::stringstream s;
                        s << "histo" << labelList[i];
                        featureList.push_back(s.str());
                        histoNames.push_back(s.str());
                    }
                }
                else
                {
                    //Use "trainSamples" which contains histogram fields
                    otbAppLogINFO("Extract SuperPixels features, then Classify pixels");
                    otb::ogr::DataSource::Pointer trainSamplesNew = otb::ogr::DataSource::New();
                    extractFeaturesAndClassify<VectorImageType>(imageIn, trainSamples_tiles[index],
                                                                trainSamplesNew, modelName,
                                                                "predicted", histoNames,
                                                                "label", this->GetParameterInt("ram"));
                    otbAppLogINFO("Extract SuperPixels features, then Classify pixels : DONE");
                    trainSamples_tiles[index]=trainSamplesNew;
                }
                //Create histograms
                otbAppLogINFO("Calculate histograms");
                LabeledVectorMapType histos;
                LabeledIntMapType counts;

                auto classifiedPointsLayer = trainSamples_tiles[index]->GetLayerChecked(0);
                otb::ogr::Layer::feature_iter<otb::ogr::Feature> classifiedPointsIt;
                for (classifiedPointsIt=classifiedPointsLayer.begin();classifiedPointsIt!=classifiedPointsLayer.end(); classifiedPointsIt++)
                {
                    const int predictedLabel=(*classifiedPointsIt)["predicted"].GetValue<int>();
                    const int spLabel=(*classifiedPointsIt)["label"].GetValue<int>();
                    auto histoIt = histos.find(spLabel);
                    if (histoIt != histos.end())
                    {
                        //Add to exisiting histogram
                        histoIt->second[std::find(labelList.begin(),labelList.end(),predictedLabel)-labelList.begin()]++;
                        counts.find(spLabel)->second++;
                    }
                    else
                    {
                        //Create new histogram
                        histos.insert(std::pair<int,std::vector<double> >(spLabel,std::vector<double>(labelList.size(),0.0)));
                        counts.insert(std::pair<LabelType,unsigned int>(spLabel,0));
                    }
                }
                otbAppLogINFO("Write histograms init : first iteration");
                //For immediate training of the new model
                //~ writeHistograms(ref_samples, labelList, histos, counts,"SPID0", (it > 1));
                writeHistograms(ref_samples, labelList, histos, counts,"spid0", (it > 1));
                ref_samples->SyncToDisk();
                
                otbAppLogINFO("Write histograms all");
                //For next iteration, avoid doing on last iteration
                if (it < this->GetParameterInt("nit"))
                {
                    writeHistograms(trainSamples_tiles[index], labelList, histos, counts, "label", (it > 1));
                }
            }//for iterate over inputs
            
            //Update model name
            otbAppLogINFO("Restart training");
            std::stringstream modelName_s;
            modelName_s << GetParameterString("out") << "model_it_" << it << ".rf";
            modelName=modelName_s.str();
            VectorTrainer->SetParameterString("io.out",modelName);
            VectorTrainer->SetParameterStringList("io.vd", ref_paths);//contains new histo features

            //~ //use sharkrf could improve results and computational time
            UpdateInternalParameters("train");
            VectorTrainer->SetParameterStringList("feat", featureList);
            UpdateInternalParameters("train");
            VectorTrainer->SetParameterStringList("cfield",{field});
            VectorTrainer->SetParameterString("classifier","rf");
            VectorTrainer->SetParameterString("rand","0");
            UpdateInternalParameters("train");
            ExecuteInternal("train");
            otbAppLogINFO("End training iteration : " << it);
        }// iterations

        //TODO cleanup temp dir
        otbAppLogINFO("DoExecute");
    }// DoExecute()

    otb::ogr::DataSource::Pointer merge_vectors(std::vector<otb::ogr::DataSource::Pointer> vectors_to_merge,
                                                std::string output_path)
    {
        // TODO pass vectors_to_merge by ref
        
        //~ Create output vector
        std::string layername = itksys::SystemTools::GetFilenameName(output_path.c_str());
        std::string extension = itksys::SystemTools::GetFilenameLastExtension(output_path.c_str());

        auto proj_ref = vectors_to_merge[0]->GetLayer(0).GetSpatialRef();
        OGRSpatialReference oSRS(*proj_ref);
        std::vector<std::string> options;
        
        otb::ogr::DataSource::Pointer output_ds = otb::ogr::DataSource::New(output_path,
                                                                            otb::ogr::DataSource::Modes::Overwrite);
        layername = layername.substr(0, layername.size() - (extension.size()));
        otb::ogr::Layer layer_out(ITK_NULLPTR, false);
        layer_out = output_ds->CreateLayer(layername, &oSRS, wkbPoint, options);
        
        //~ Create output fields
        OGRFeatureDefn &poFDefn =  vectors_to_merge[0]->GetLayer(0).GetLayerDefn();
        for( int iField=0; iField < poFDefn.GetFieldCount(); iField++ )
        {
            OGRFieldDefn fieldDefn(poFDefn.GetFieldDefn(iField));
            layer_out.CreateField(fieldDefn);
        }

        //~ read input and fill output vector
        for (size_t ind=0; ind < vectors_to_merge.size(); ++ind)
        {
            auto layer = vectors_to_merge[ind]->GetLayer(0);
            otb::ogr::Layer::const_iterator featIt = layer.begin();
            for(; featIt!=layer.end(); ++featIt)
            {
                otb::ogr::Feature dstFeature(layer_out.GetLayerDefn());
                dstFeature.SetFrom( *featIt, TRUE );
                layer_out.CreateFeature( dstFeature );
            }

            //~ layer_out.ogr().CommitTransaction();
        }
        output_ds->SyncToDisk();
        return output_ds;
    }   

    typename otb::ogr::DataSource::Pointer
    vectorizeNoStreaming(typename LabelImageType::Pointer labelIn, std::string tmpdir){
        labelIn->UpdateOutputInformation();

        auto projRef = labelIn->GetProjectionRef();
        otb::ogr::DataSource::Pointer ogrDS;
        otb::ogr::Layer layer(ITK_NULLPTR, false);

        OGRSpatialReference oSRS(projRef.c_str());
        std::vector<std::string> options;

        std::stringstream shapefile;
        shapefile << tmpdir << "/tempShapeFile.sqlite";

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

        //~ layer.ogr().CommitTransaction();

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
                                                      std::string file_name,
                                                      unsigned ram,
                                                      const std::unordered_set<int> & SP_id,
                                                      RAMDrivenAdaptativeStreamingManagerType::Pointer streamingManager,
                                                      unsigned int threadsNumber=1)
    {
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
    tempName << tmpdir << "/" << file_name;

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

    void writeHistograms(otb::ogr::DataSource::Pointer & classifiedPoints,
                         std::vector<LabelType> labelList,
                         LabeledVectorMapType & histos,
                         LabeledIntMapType & counts,
                         std::string fieldName,
                         bool overwrite=false)
      {
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
        //~ classifiedPointsLayer.ogr().CommitTransaction();
        // classifiedPoints->SyncToDisk();
      }//writeHistograms
    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainAutoContext)