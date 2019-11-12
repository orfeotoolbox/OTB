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

      using ValueType = float;
      using LabelSampleType     = itk::FixedArray<LabelType, 1>;
      using MachineLearningModelType        = otb::MachineLearningModel<ValueType, LabelType>;
      using InputSampleType      = itk::VariableLengthVector<ValueType>;
      using ListSampleType       = itk::Statistics::ListSample<InputSampleType>;
      using MeasurementType  = itk::VariableLengthVector<ValueType>;
      using ModelPointerType                = typename MachineLearningModelType::Pointer;
      using MachineLearningModelFactoryType = otb::MachineLearningModelFactory<ValueType, LabelType>;
      using LabelListSampleType = itk::Statistics::ListSample<LabelSampleType>;
      
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

        AddParameter(ParameterType_StringList, "refdata", "Reference data list");
        SetParameterDescription( "refdata", "SQLite file containing labeled points for training." );

        AddParameter(ParameterType_String, "reffield", "Label field");
        SetParameterDescription( "reffield", "Field containing the class integer label for supervision" );

        AddParameter(ParameterType_StringList, "superpixdata", "superPixel data list");
        SetParameterDescription( "superpixdata", "SQLite file containing superpixels samples position with features");

        AddParameter(ParameterType_String, "superpixdatafield", "Label field");
        SetParameterDescription( "superpixdatafield", "SQLite file containing superpixels samples position with features");

        AddParameter(ParameterType_StringList, "feat", "features list");
        SetParameterDescription( "feat", "Field names for training features");
        
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

    void create_field(const std::vector<std::string> & in_seg_list, std::string FIELD_NAME, OGRFieldType data_type)
    {
        //~ TODO : check if the field allready exists
        for(auto in_seg : in_seg_list)
        {
            otb::ogr::DataSource::Pointer in_seg_data_source = otb::ogr::DataSource::New(in_seg,
                                                                                         otb::ogr::DataSource::Modes::Update_LayerUpdate);
            auto in_seg_layer = in_seg_data_source -> GetLayerChecked(0);
            
            std::vector<std::string> fields_name;
            //~ OGRFeatureDefn fields_defn = in_seg_layer.GetLayerDefn();
            OGRFeatureDefn &fields_defn =  in_seg_layer.GetLayerDefn();
            for( int field_ind=0; field_ind < fields_defn.GetFieldCount(); field_ind++ )
            {
                fields_name.push_back(fields_defn.GetFieldDefn(field_ind)->GetNameRef());
            }
        
            if ( std::find(fields_name.begin(), fields_name.end(), FIELD_NAME) == fields_name.end() )
            {
                OGRFieldDefn predicted_field(FIELD_NAME.c_str(), data_type);
                in_seg_layer.CreateField(predicted_field);
            }
        }
    }
    std::vector<LabelType> getLabels(const std::vector<std::string> & pixel_features_list,
                                     std::string data_field_name)
    {
        std::vector<LabelType> labels;
        for(auto pixel_features_file : pixel_features_list)
        {
            otb::ogr::DataSource::Pointer ref_label_data_source = otb::ogr::DataSource::New(pixel_features_file,
                                                                                            otb::ogr::DataSource::Modes::Read);
            auto ref_label_layer = ref_label_data_source -> GetLayerChecked(0);
            otb::ogr::Layer::feature_iter<otb::ogr::Feature> ref_label_it;
            for (ref_label_it=ref_label_layer.begin(); ref_label_it!=ref_label_layer.end(); ref_label_it++)
            {
                labels.push_back((*ref_label_it)[data_field_name].GetValue<long long>());
            }
        }
        std::sort(labels.begin(), labels.end());
        auto last = std::unique(labels.begin(), labels.end());
        labels.erase(last, labels.end());
        return labels;
    }


ListSampleType::Pointer ReadInputListSample(ogr::DataSource::Pointer source,
                                            const std::vector<std::string> & field_list,
                                            const std::string field_super_pixel,
                                            std::vector<int> & super_pixel_labels)
{
  auto layer  = source->GetLayer(0);
  typename ListSampleType::Pointer input = ListSampleType::New();

  const unsigned int nbFeatures = field_list.size();
  input->SetMeasurementVectorSize(nbFeatures);

  ogr::Feature             feature = layer.ogr().GetNextFeature();
  std::vector<int> featureFieldIndex(nbFeatures, -1);
  for (unsigned int i = 0; i < nbFeatures; i++)
  {
    featureFieldIndex[i] = feature.ogr().GetFieldIndex(field_list[i].c_str());
    if (featureFieldIndex[i] < 0)
      otbAppLogFATAL("The field name for feature " << field_list[i].c_str() << " has not been found" << std::endl);
  }
  int super_pixel_field_index = feature.ogr().GetFieldIndex(field_super_pixel.c_str());
  layer.ogr().ResetReading();
  int nb_samples = layer.ogr().GetFeatureCount(true);
  super_pixel_labels.reserve(nb_samples);
  
  for (auto const& feature : layer)
  {
    MeasurementType mv(nbFeatures);
    for (unsigned int idx = 0; idx < nbFeatures; ++idx)
    {
      auto field = feature[featureFieldIndex[idx]];
      switch (field.GetType())
      {
      case OFTInteger:
      case OFTInteger64:
        mv[idx] = static_cast<ValueType>(field.template GetValue<int>());
        break;
      case OFTReal:
        mv[idx] = static_cast<ValueType>(field.template GetValue<double>());
        break;
      default:
        itkExceptionMacro(<< "incorrect field type: " << field.GetType() << ".");
      }
    }
    input->PushBack(mv);
    super_pixel_labels.push_back(feature[super_pixel_field_index].template GetValue<int>());
  }
  return input;
}

    void compute_autoContext(std::string model_path,
                             const std::vector<std::string> & in_seg_list,
                             const std::string PREDICTED_FIELD_NAME,
                             const std::string SUPERPIX_FIELD_NAME,
                             const std::vector<std::string> & features_list,
                             const std::vector<std::string> & in_ref_list, 
                             const std::vector<std::string> &  histo_names)
    {
        ModelPointerType model = MachineLearningModelFactoryType::CreateMachineLearningModel(model_path, MachineLearningModelFactoryType::ReadMode);
        model->SetRegressionMode(false);
        model->Load(model_path);
        
        for(unsigned int i = 0; i < in_seg_list.size(); i++)
        {
            std::string in_seg = in_seg_list[i];
            std::string in_ref = in_ref_list[i];
            auto source = otb::ogr::DataSource::New(in_seg, otb::ogr::DataSource::Modes::Read);
            auto layer  = source->GetLayer(0);
            std::vector<int> super_pixel_labels;
            auto input = ReadInputListSample(source, features_list,
                                             SUPERPIX_FIELD_NAME,
                                             super_pixel_labels);
            typename LabelListSampleType::Pointer target;
            otbAppLogINFO("Start prediction of " + in_seg);
            target = model->PredictBatch(input);
            otbAppLogINFO("Prediction done");
            otbAppLogINFO("Compute histograms");
            compute_histograms(target, super_pixel_labels, in_ref, SUPERPIX_FIELD_NAME);            
        }
    }

void compute_histograms(const typename LabelListSampleType::Pointer predicted_labels,
                        std::vector<int> super_pixel_labels, 
                        std::string data_ref,
                        std::string SUPERPIX_FIELD_NAME)
    {
        LabeledVectorMapType histos;
        LabeledIntMapType counts;

        for (int i=0; i<super_pixel_labels.size(); i++)
        {
            const int predicted_label = predicted_labels->GetMeasurementVector(i)[0];
            const int sp_label = super_pixel_labels[i];
            auto histo_it = histos.find(sp_label);
            if (histo_it != histos.end())
            {
                //Add to exisiting histogram
                histo_it->second[std::find(m_labelList.begin(),m_labelList.end(), predicted_label) - m_labelList.begin()]++;
                counts.find(sp_label)->second++;
            }
            else
            {
                //Create new histogram
                histos.insert(std::pair<int,std::vector<double> >(sp_label, std::vector<double>(m_labelList.size(),0.0)));
                counts.insert(std::pair<LabelType,unsigned int>(sp_label, 0));
            }
        }
            otbAppLogINFO("Add histograms to : " + data_ref);
            writeHistograms(data_ref, m_labelList, histos, counts, SUPERPIX_FIELD_NAME, true);
            otbAppLogINFO("Add histograms to : " + data_ref + "done");
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
            //~ s << "histo" << e;
            s << "histo_" << e;
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
            s << "histo_" << labelList[i];
            dstFeature[s.str()].SetValue<double>(histo[i]/count);
          }
          classifiedPointsLayer.SetFeature(dstFeature);
        }
        //~ classifiedPointsLayer.ogr().CommitTransaction();
        classifiedPoints->SyncToDisk();
      }


    void train_model(const std::vector<std::string> & in_ref_list,
                     std::string REF_FIELD_NAME,
                     const std::vector<std::string> & in_ref_features,
                     std::string output_model)
    {
        auto VectorTrainer = GetInternalApplication("train");
        VectorTrainer->SetParameterString("io.out", output_model);
        VectorTrainer->SetParameterStringList("io.vd", in_ref_list);
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterStringList("feat", in_ref_features);
        UpdateInternalParameters("train");
        VectorTrainer->SetParameterString("cfield", REF_FIELD_NAME);

        VectorTrainer->SetParameterString("classifier","sharkrf");
        VectorTrainer->SetParameterString("classifier.sharkrf.nbtrees","100");

        VectorTrainer->SetParameterString("rand","0");
        UpdateInternalParameters("train");
        VectorTrainer->ExecuteAndWriteOutput();
    }

    void DoExecute()
    {       
        const std::vector<std::string> in_ref_list = GetParameterStringList("refdata");
        std::string ref_field = GetParameterString("reffield");
        const std::vector<std::string> pixel_features_list = GetParameterStringList("feat");
        std::vector<std::string> pixel_features_list_with_histo = pixel_features_list;
        const std::vector<std::string> in_seg_list = GetParameterStringList("superpixdata");
        std::string seg_field = GetParameterString("superpixdatafield");

        const std::string PREDICTED_FIELD_NAME = "predicted";
        
        m_labelList = getLabels(in_ref_list, ref_field);

        std::vector<std::string> histo_names;
        std::string labelList_str = "";
        for(auto pixel_features_file : m_labelList)
        {
            labelList_str += std::to_string(pixel_features_file) + ", ";
            std::string histo_name = "histo_" + std::to_string(pixel_features_file);
            histo_names.push_back(histo_name);
            pixel_features_list_with_histo.push_back(histo_name);
            create_field(in_ref_list, histo_name, OFTReal);
        }
        otbAppLogINFO("Labels found : " + labelList_str);
        otbAppLogINFO("Start training firts iteration");
        
        std::string modelName = GetParameterString("out") + "/" + "model_it_0.rf";
        train_model(in_ref_list, ref_field, pixel_features_list, modelName);
        
        struct rusage r_usage;
        getrusage(RUSAGE_SELF,&r_usage);
        printf("ram usage : %ld\n",r_usage.ru_maxrss);

        for (int it=1; it <= this->GetParameterInt("nit"); it++)
        {
            otbAppLogINFO("predict superpixels labels thanks to : " + modelName);
            compute_autoContext(modelName, in_seg_list, PREDICTED_FIELD_NAME, seg_field, pixel_features_list, in_ref_list, histo_names);
            std::string iteration_str = std::to_string(it);
            std::string model_iteration = GetParameterString("out") + "/" + "model_it_" + iteration_str + ".rf";
            otbAppLogINFO("Compute model : " + model_iteration);
            
            train_model(in_ref_list, ref_field, pixel_features_list_with_histo, model_iteration);
            modelName = model_iteration;
            
            getrusage(RUSAGE_SELF,&r_usage);
            printf("ram usage : %ld\n",r_usage.ru_maxrss);
        }
    }// DoExecute()

    std::vector<LabelType> m_labelList;
    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::TrainAutoContext)
