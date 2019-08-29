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
#include "ogr_api.h"

#include "otbHistogramCalculationFilter.h"
#include "otbHistogramImageWriterFilter.h"

#include <limits>
#include <unordered_set>


namespace otb
{
  namespace Wrapper
  {
    class ClassifyAutoContext : public CompositeApplication
    {
    public:
      typedef ClassifyAutoContext Self;
      typedef Application Superclass;
      typedef itk::SmartPointer<Self> Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      typedef int                                                                  LabelType;
      typedef otb::Image<LabelType>                                                LabelImageType;
      typedef otb::ImageFileReader<LabelImageType>                                 LabelReaderType;
      typedef otb::ImageFileWriter<LabelImageType>                                 LabelWriterType;

      typedef otb::VectorImage<LabelType>                                          LabelVectorImageType;
      typedef otb::VectorImage<LabelType>::PixelType                               LabelVectorImagePixelType;
      typedef otb::ImageFileWriter<LabelVectorImageType>                           LabelVectorWriterType;
      typedef otb::ImageFileReader<LabelVectorImageType>                           LabelVectorReaderType;

      typedef float                                                                ComponentType;
      typedef otb::VectorImage<ComponentType>                                      FloatVectorImageType;
      typedef otb::ImageFileReader<FloatVectorImageType>                           FloatVectorImageReaderType;
      typedef otb::ImageFileWriter<FloatVectorImageType>                           FloatVectorImageWriterType;

      typedef otb::StreamingStatisticsImageFilter<LabelImageType>                  StatisticsImageFilterType;
      typedef otb::ExtractROI<LabelType,LabelType>                                 ExtractROIFilterType;
      typedef itk::ImageRegionConstIterator<LabelImageType>                        LabelImageIterator;
      typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType>                 LabelImageToOGRDataSourceFilterType;
      typedef otb::VectorDataFileWriter<VectorDataType>                            VectorDataFileWriterType;

      typedef otb::OGRDataToSamplePositionFilter<LabelImageType, LabelImageType,otb::RandomSampler> RandomSamplerType;

      typedef otb::SamplingRateCalculator                                          RateCalculatorType;
      typedef otb::OGRDataSourceToLabelImageFilter<LabelImageType>                 OGRDataSourceToMapFilterType;
      typedef std::map<std::string, unsigned long>                                 ClassCountMapType;
      typedef RateCalculatorType::MapRateType                                      MapRateType;
      typedef otb::OGRDataToClassStatisticsFilter<LabelImageType,LabelImageType>   ClassStatFilterType;

      typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > ConcatenateImageFilter;

      typedef otb::ImageToVectorImageCastFilter< LabelImageType, LabelVectorImageType > CastFilterType;
      typedef otb::StreamingHistogramCalculationFilter<LabelImageType,LabelImageType>   HistogramCalculationFilterType;
      typedef otb::HistogramImageWriterFilter<LabelImageType,FloatVectorImageType>      HistogramImageWriterFilterType;
      typedef typename HistogramCalculationFilterType::SampleType                       SampleType;
      typedef typename HistogramCalculationFilterType::LabeledSampleContainerType       LabeledVectorMapType;
      typedef typename HistogramCalculationFilterType::LabelSamplePairType              LabelSamplePairType;

      itkNewMacro(Self);
      itkTypeMacro(ClassifyAutoContext, otb::CompositeApplication);

    private:

      std::vector<HistogramCalculationFilterType::Pointer> m_HistoCalculationFilters;

      std::vector<HistogramImageWriterFilterType::Pointer> m_HistoImageWriterFilters;

      std::vector<ConcatenateImageFilter::Pointer> m_ConcatenateImageFilters;

      LabelReaderType::Pointer m_Segreader;

    void DoInit()
    {
        ClearApplications();
        SetName("ClassifyAutoContext");
        SetDescription("");

        SetDocLongDescription(" ");
        SetDocLimitations(" ");
        SetDocAuthors(" ");
        SetDocSeeAlso(" ");

        AddParameter(ParameterType_InputImage,  "in",    "Input image");
        SetParameterDescription( "in", "The input image." );
        AddParameter(ParameterType_String, "inseg", "Input segmentation");
        SetParameterDescription( "inseg", "Input raster containing the segment label IDs" );
        AddParameter(ParameterType_StringList, "models", "Input models");
        SetParameterDescription("models","List of models from auto context trainer");
        AddParameter(ParameterType_StringList, "lablist", "List of labels");
        SetParameterDescription("lablist","List of labels to take into account in histograms");
        AddParameter(ParameterType_Directory,  "tmpdir",    "Temporary directory");
        AddParameter(ParameterType_String, "out", "output path");
        SetParameterDescription( "out", "Path for storing results" );
        AddRAMParameter();
        GDALSetCacheMax(0);
    }

    void DoUpdateParameters(){}

    void DoExecute()
    {
        const std::vector<std::string> modelList = GetParameterStringList("models");
        for (unsigned i = 0; i < modelList.size(); i++)
        {
            std::stringstream clname;
            clname << "classifier" << i;
            AddApplication("ImageClassifier", clname.str(), "Image classifier");
        }

        const std::vector<std::string> labellist = GetParameterStringList("lablist");
        std::unordered_map<LabelType,unsigned> labelPositions;
        for (unsigned i = 0; i < labellist.size(); i++)
        {
            labelPositions.insert(std::pair<LabelType,unsigned>(atoi(labellist[i].c_str()),i));
        }

        FloatVectorImageType::Pointer imageIn = GetParameterImage("in");
        imageIn->UpdateOutputInformation();

        m_Segreader = LabelReaderType::New();
        m_Segreader->SetFileName(GetParameterString("inseg"));
        m_Segreader->UpdateOutputInformation();
        auto seg = m_Segreader->GetOutput();

        LabelImageType::Pointer prevImage;

        for (unsigned i = 0; i < modelList.size(); i++)
        {
            otbAppLogINFO("Starting iteration " << i);
            std::stringstream clname;
            clname << "classifier" << i;

            auto imageClassifier = GetInternalApplication(clname.str());

            if (i==0)
            {
                imageClassifier->SetParameterInputImage("in", imageIn);
            }
            else
            {
                std::stringstream pclname;
                pclname << "classifier" << i-1;

                auto prevImageClassifier = GetInternalApplication(pclname.str());
                prevImage=dynamic_cast<LabelImageType*>(prevImageClassifier->GetParameterOutputImage("out"));
                prevImage->UpdateOutputInformation();

                //Invoke histogram filter
                otbAppLogINFO("Calculating histograms");
                auto histoCalculationFilter = HistogramCalculationFilterType::New();
                m_HistoCalculationFilters.push_back(histoCalculationFilter);
                histoCalculationFilter->SetInput(prevImage);
                histoCalculationFilter->SetImageSeg(seg);
                histoCalculationFilter->SetLabelPositions(labelPositions);
                histoCalculationFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
                histoCalculationFilter->GetFilter()->SetNumberOfThreads(1);

                histoCalculationFilter->Update();

                otbAppLogINFO("Writing histogram image");
                auto m_HistoImageWriterFilter = HistogramImageWriterFilterType::New();
                otbAppLogINFO("After New");
                m_HistoImageWriterFilters.push_back(m_HistoImageWriterFilter);
                otbAppLogINFO("1");
                m_HistoImageWriterFilter->SetInput(seg);
                otbAppLogINFO("2");
                // std::cout << m.begin()->second.size() << "\n";
                m_HistoImageWriterFilter->SetMeans(histoCalculationFilter->GetFilter()->GetMeans());
                otbAppLogINFO("3");
                m_HistoImageWriterFilter->UpdateOutputInformation();
                otbAppLogINFO("Setup histo done");

                ConcatenateImageFilter::Pointer m_conc = ConcatenateImageFilter::New();
                m_ConcatenateImageFilters.push_back(m_conc);

                m_conc->SetInput1(imageIn);
                m_conc->SetInput2(m_HistoImageWriterFilter->GetOutput());
                m_conc->UpdateOutputInformation();
                otbAppLogINFO("setup conc done");
                imageClassifier->SetParameterInputImage("in",m_conc->GetOutput());
            }

            imageClassifier->SetParameterString("model",modelList[i]);

            imageClassifier->SetParameterInt("ram", GetParameterInt("ram"));
            imageClassifier->UpdateParameters();
            if (i == modelList.size()-1)
            {
                //Last iteration
                std::stringstream outName_s;
                outName_s << GetParameterString("out") << "result_it_" << i << ".tif";
                imageClassifier->SetParameterString("out",outName_s.str());
                imageClassifier->ExecuteAndWriteOutput();
            }
            else
            {
                otbAppLogINFO("befroe execute");
                imageClassifier->Execute();
                otbAppLogINFO("after execute");
                // imageClassifier->RegisterPipeline();
                auto image = imageClassifier->GetParameterImageBase("out");
                image->UpdateOutputInformation();
            }
        }
        otbAppLogINFO("Finished");
        //TODO cleanup temp dir
    }//doExecute()
    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassifyAutoContext)
