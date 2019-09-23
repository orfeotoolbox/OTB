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

#include <sys/time.h>
#include <sys/resource.h>
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
        otbAppLogINFO("TrainAutoContext application");
        struct rusage r_usage;
        getrusage(RUSAGE_SELF,&r_usage);
        printf("-----------------> Etat de la RAM au début de DoExecute() : %ld\n",r_usage.ru_maxrss);

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
        //~ TODO : check if every input image get the same number of bands, could be long if "in memory" pipeline
        
        //~ iterate over inputs
        VectorImageType::Pointer imageIn;
        LabelReaderType::Pointer lreader = LabelReaderType::New();
        LabelImageType::Pointer imageSeg;
        CastFilterType::Pointer castFilter = CastFilterType::New();
        RAMDrivenAdaptativeStreamingManagerType::Pointer
                      streamingManager = RAMDrivenAdaptativeStreamingManagerType::New();
        RegionType largestRegion;
        otb::ogr::DataSource::Pointer SPDataSource;
        //~ otb::ogr::Layer SPLayer;
        otb::ogr::DataSource::Pointer refLabelDataSource;
        otb::ogr::DataSource::Pointer inter;
        otb::ogr::DataSource::Pointer outSamples;
        otb::ogr::DataSource::Pointer initTrainSamples;

        for (size_t index = 0; index < nbImages; ++index)
        {
            otbAppLogINFO("Index : " << index);
            auto index_string = std::to_string(index);
            getrusage(RUSAGE_SELF,&r_usage);
            std::cout<< "-----------------> extraction de primitives, image "+index_string+" : RAM = "<< r_usage.ru_maxrss << std::endl;
            otbAppLogINFO("Processing images at index : " << index_string);
            in_img_list->GetNthElement(index)->UpdateOutputInformation();
            std::string in_seg = in_seg_list[index];

            otbAppLogINFO("Processing segmentation : " << in_seg);
            lreader->SetFileName(in_seg);
            lreader->UpdateOutputInformation();
            imageSeg = lreader->GetOutput();
            std::string in_ref = in_ref_list[index];

            otbAppLogINFO("Processing reference data : " << in_ref);
            refLabelDataSource = otb::ogr::DataSource::New(in_ref, otb::ogr::DataSource::Modes::Read);
            auto refLabelLayer = refLabelDataSource->GetLayerChecked(0);
            otb::ogr::Layer::feature_iter<otb::ogr::Feature> refLabelIt;
            for (refLabelIt=refLabelLayer.begin();refLabelIt!=refLabelLayer.end(); refLabelIt++) {
              //std::set will sort and only insert if unique
              m_labelList.push_back((*refLabelIt)[field].GetValue<long long>());
            }
            //~ About streaming
            largestRegion = imageSeg->GetLargestPossibleRegion();
            int availableRAM = GetParameterInt("ram");
            streamingManager->SetAvailableRAMInMB(availableRAM);
            float bias = 2.0; // empiric value;
            streamingManager->SetBias(bias);
            streamingManager->PrepareStreaming(imageSeg, largestRegion);

            otbAppLogINFO("streamingManager : OK");
            castFilter->SetInput(imageSeg);
            castFilter->UpdateOutputInformation();

            otbAppLogINFO("Preparing data");
            // Vectorize segmentation
            SPDataSource = vectorizeNoStreaming(imageSeg, tmpdir);
            otb::ogr::Layer SPLayer = SPDataSource->GetLayerChecked(0);
           
            //Create list of labels
            std::string tempName = tmpdir+ "/featExtract.sqlite";
            inter = extractFeatures<VectorImageType>(castFilter->GetOutput(),
                                                     refLabelDataSource,
                                                     tempName, "SPID",
                                                     field, m_sampleExtractionFilters,
                                                     ram, threadsNumber);
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
            std::string sp_selection_filename = "full_SP_SampleSelecetion_"+index_string+".shp";
            fullSampleSelection(lreader->GetOutput(),
                                tmpdir,
                                sp_selection_filename,
                                ram, interSet, streamingManager,
                                threadsNumber);
            m_SP_selection.push_back(tmpdir + sp_selection_filename);
            otbAppLogINFO("sampling SuperPixels at 100% rate : Done");
            //Rasterize ref data
            //~ auto rasterFilter = OGRDataSourceToMapFilterType::New();
            //~ rasterFilter->AddOGRDataSource(refLabelDataSource);
            //~ rasterFilter->SetOutputSize(lreader->GetOutput()->GetLargestPossibleRegion().GetSize());
            //~ rasterFilter->SetOutputOrigin(lreader->GetOutput()->GetOrigin());
            //~ rasterFilter->SetOutputSpacing(lreader->GetOutput()->GetSignedSpacing());
            //~ rasterFilter->SetBackgroundValue(0);
            //~ rasterFilter->SetBurnAttributeMode(true);
            //~ rasterFilter->SetBurnAttribute(field);
            //~ rasterFilter->SetOutputProjectionRef(lreader->GetOutput()->GetProjectionRef());
            //~ rasterFilter->UpdateOutputInformation();

            //Extract features only on labeled samples for training
            const std::string initTrainSamples_s = tmpdir + "/initTrainSamples_"+index_string+".sqlite";
            m_ref_paths.push_back(initTrainSamples_s);
            otbAppLogINFO("Start sample Extraction : initialize training sample-set");
            initTrainSamples = extractFeatures<VectorImageType>(in_img_list->GetNthElement(index), inter,
                                                                initTrainSamples_s,
                                                                "feature",
                                                                field, m_sampleExtractionFilters, availableRAM);
            otbAppLogINFO("Start sample Extraction : initialize training sample-set : DONE");
        }//for iterate over inputs
        
        std::sort(m_labelList.begin(), m_labelList.end());
        auto last = std::unique(m_labelList.begin(),m_labelList.end());
        m_labelList.erase(last, m_labelList.end());
        std::cout << "Labels found : ";
        for (auto e : m_labelList) {
          std::cout << e << " ";
        }
        std::cout << "\n";
            
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
        VectorTrainer->SetParameterStringList("io.vd", m_ref_paths);
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterStringList("feat",featureList);
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterStringList("cfield",{field});
        VectorTrainer->SetParameterString("classifier","rf");

        VectorTrainer->SetParameterString("classifier.rf.min","5");
        VectorTrainer->SetParameterString("classifier.rf.max","25");

        VectorTrainer->SetParameterString("rand","0");
        UpdateInternalParameters("train");
        VectorTrainer->ExecuteAndWriteOutput();
        otbAppLogINFO("Training firts iteration : DONE");
        //Extract and classify features for all samples
        std::vector<std::vector<std::string>> histoNames_tiles;
        //initialize trainSamples_tiles
        for (size_t index=0; index < nbImages; ++index)
        {
            m_SP_samplesPredicted.push_back(tmpdir + "/full_SP_samplesPredicted_"+std::to_string(index)+".sqlite");
            histoNames_tiles.push_back(std::vector<std::string>());
        }

        std::vector<std::string> histoNames;
        for (int it=1; it <= this->GetParameterInt("nit"); it++)
        {
            otbAppLogINFO("Start iteration " << it);
            const unsigned labelListSize=m_labelList.size();

            if(it==1)
            {
                for (unsigned i = 0; i < labelListSize; i++)
                {
                    std::stringstream s;
                    s << "histo" << m_labelList[i];
                    featureList.push_back(s.str());
                    histoNames.push_back(s.str());
                }
            }
            for (size_t index=0; index < nbImages; ++index)
            {
                //~ FloatVectorImageListType::Pointer in_img_list = GetParameterImageList("il");
                VectorImageType::Pointer imageIn = in_img_list->GetNthElement(index);
                imageIn->UpdateOutputInformation();
                auto outSamples = m_SP_selection[index];
                if(it==1)
                {
                    //Use "outSamples" which doesn't contain histogram fields
                    extractFeaturesAndClassify<VectorImageType>(imageIn, outSamples,
                                                                m_SP_samplesPredicted[index], modelName,
                                                                "predicted", std::vector<std::string>(),
                                                                m_sampleExtractionClassifyFilters,
                                                                "label", this->GetParameterInt("ram"));
                }
                else
                {
                    //Use "trainSamples" which contains histogram fields
                    otbAppLogINFO("Extract SuperPixels features, then Classify pixels");
                    std::string samples_it = tmpdir + "/full_SP_samplesPredicted_"+std::to_string(index)+"_"+std::to_string(it)+".sqlite";
                    extractFeaturesAndClassify<VectorImageType>(imageIn, m_SP_samplesPredicted[index],
                                                                samples_it, modelName,
                                                                "predicted", histoNames, m_sampleExtractionClassifyFilters,
                                                                "label", this->GetParameterInt("ram"));
                    otbAppLogINFO("Extract SuperPixels features, then Classify pixels : DONE");
                    m_SP_samplesPredicted[index]=samples_it;
                }
                //Create histograms
                otbAppLogINFO("Calculate histograms");
                LabeledVectorMapType histos;
                LabeledIntMapType counts;

                //~ auto classifiedPointsLayer = m_trainSamples_tiles[index]->GetLayerChecked(0);
                otb::ogr::DataSource::Pointer classifiedPoints = otb::ogr::DataSource::New(m_SP_samplesPredicted[index], otb::ogr::DataSource::Modes::Read);
                auto classifiedPointsLayer = classifiedPoints->GetLayerChecked(0);
                otb::ogr::Layer::feature_iter<otb::ogr::Feature> classifiedPointsIt;
                for (classifiedPointsIt=classifiedPointsLayer.begin();classifiedPointsIt!=classifiedPointsLayer.end(); classifiedPointsIt++)
                {
                    const int predictedLabel=(*classifiedPointsIt)["predicted"].GetValue<int>();
                    const int spLabel=(*classifiedPointsIt)["label"].GetValue<int>();
                    auto histoIt = histos.find(spLabel);
                    if (histoIt != histos.end())
                    {
                        //Add to exisiting histogram
                        histoIt->second[std::find(m_labelList.begin(),m_labelList.end(),predictedLabel)-m_labelList.begin()]++;
                        counts.find(spLabel)->second++;
                    }
                    else
                    {
                        //Create new histogram
                        histos.insert(std::pair<int,std::vector<double> >(spLabel,std::vector<double>(m_labelList.size(),0.0)));
                        counts.insert(std::pair<LabelType,unsigned int>(spLabel,0));
                    }
                }
                otbAppLogINFO("Write histograms init : first iteration");
                //For immediate training of the new model
                writeHistograms(m_ref_paths[index], m_labelList, histos, counts,"spid0", (it > 1));
                //~ ref_samples->SyncToDisk();

                otbAppLogINFO("Write histograms all");
                //For next iteration, avoid doing on last iteration
                if (it < this->GetParameterInt("nit"))
                {
                    writeHistograms(m_SP_samplesPredicted[index], m_labelList, histos, counts, "label", (it > 1));
                }
            }//for iterate over inputss
            //Update model name
            otbAppLogINFO("Restart training");
            std::stringstream modelName_s;
            modelName_s << GetParameterString("out") << "model_it_" << it << ".rf";
            modelName=modelName_s.str();
            VectorTrainer->SetParameterString("io.out",modelName);
            VectorTrainer->SetParameterStringList("io.vd", m_ref_paths);//contains new histo features

            //use sharkrf could improve results and computational time
            UpdateInternalParameters("train");
            VectorTrainer->SetParameterStringList("feat", featureList);
            UpdateInternalParameters("train");
            VectorTrainer->SetParameterStringList("cfield",{field});
            VectorTrainer->SetParameterString("classifier","rf");

            VectorTrainer->SetParameterString("classifier.rf.min","5");
            VectorTrainer->SetParameterString("classifier.rf.max","25");

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

        //~ layer.ogr().CommitTransaction();

        return ogrDS;
    }

    template<typename TInputImageType>
    void extractFeaturesAndClassify(typename TInputImageType::Pointer im,
                                    std::string inputDS_path,
                                    std::string outputDS_path,
                                    std::string modelName,
                                    std::string classField,
                                    std::vector<std::string> extraFieldNames,
                                    std::vector<otb::ExtractClassifyFilter<VectorImageType, LabelType>::Pointer> &sampleExtractionClassifyFilters,
                                    std::string randomFieldName="label",
                                    unsigned ram=512)
    {
        typedef otb::ExtractClassifyFilter<TInputImageType, LabelType> ExtractClassifyFilterType;
        typename ExtractClassifyFilterType::Pointer filter = ExtractClassifyFilterType::New();

        otb::ogr::DataSource::Pointer const inputDS = otb::ogr::DataSource::New(inputDS_path, otb::ogr::DataSource::Modes::Read);
        otb::ogr::DataSource::Pointer outputDS = otb::ogr::DataSource::New(outputDS_path, otb::ogr::DataSource::Modes::Overwrite);

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
        sampleExtractionClassifyFilters.push_back(filter);
        outputDS->SyncToDisk();
    }

    template<typename TInputImageType>
    otb::ogr::DataSource::Pointer extractFeatures(typename TInputImageType::Pointer im,
                                                  otb::ogr::DataSource::Pointer positions,
                                                  std::string outputName,
                                                  std::string outputPrefix,
                                                  std::string randomFieldName,
                                                  std::vector<otb::ImageSampleExtractorFilter<VectorImageType>::Pointer> &extractionFilters,
                                                  unsigned ram=512,
                                                  unsigned int threadsNumber=1)
    {
        otb::ogr::DataSource::Pointer output = otb::ogr::DataSource::New(outputName, otb::ogr::DataSource::Modes::Overwrite);
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
        
        extractionFilters.push_back(filter);
        
        return output;
    }

    void fullSampleSelection(LabelImageType::Pointer inputIm,
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

	//~ return outputSamples;
      }

    void writeHistograms(std::string classifiedPoints_path,
                         std::vector<LabelType> labelList,
                         LabeledVectorMapType & histos,
                         LabeledIntMapType & counts,
                         std::string fieldName,
                         bool overwrite=false)
      {
        otb::ogr::DataSource::Pointer classifiedPoints = otb::ogr::DataSource::New(classifiedPoints_path, otb::ogr::DataSource::Modes::Update_LayerUpdate);
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
        classifiedPoints->SyncToDisk();
      }//writeHistograms

    std::vector<std::string> m_ref_paths;
    std::vector<std::string> m_SP_selection;
    std::vector<std::string> m_SP_samplesPredicted;
    std::vector<LabelType> m_labelList;
    std::vector<otb::ImageSampleExtractorFilter<VectorImageType>::Pointer> m_sampleExtractionFilters;
    std::vector<otb::ExtractClassifyFilter<VectorImageType, LabelType>::Pointer> m_sampleExtractionClassifyFilters;

    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainAutoContext)
