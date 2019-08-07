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
      


      typedef otb::StreamingStatisticsImageFilter<LabelImageType> StatisticsImageFilterType;
      typedef otb::ExtractROI<LabelType,LabelType> ExtractROIFilterType;
      typedef itk::ImageRegionConstIterator<LabelImageType> LabelImageIterator;

      typedef otb::LabelImageToOGRDataSourceFilter<LabelImageType> LabelImageToOGRDataSourceFilterType;

      typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;

      typedef otb::OGRDataToSamplePositionFilter<LabelImageType, LabelImageType,otb::RandomSampler> RandomSamplerType;

      typedef otb::SamplingRateCalculator               RateCalculatorType;

      typedef otb::OGRDataSourceToLabelImageFilter<LabelImageType> OGRDataSourceToMapFilterType;



      typedef std::map<std::string, unsigned long>      ClassCountMapType;
      typedef RateCalculatorType::MapRateType           MapRateType;

      typedef otb::OGRDataToClassStatisticsFilter<LabelImageType,LabelImageType> ClassStatFilterType;

      typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > ConcatenateImageFilter;

      typedef otb::ImageToVectorImageCastFilter< LabelImageType, LabelVectorImageType > CastFilterType;

      typedef otb::StreamingHistogramCalculationFilter<LabelImageType,LabelImageType> HistogramCalculationFilterType;
      typedef otb::HistogramImageWriterFilter<LabelImageType,FloatVectorImageType> HistogramImageWriterFilterType;

      typedef typename HistogramCalculationFilterType::SampleType                           SampleType;

      typedef typename HistogramCalculationFilterType::LabeledSampleContainerType LabeledVectorMapType;
      typedef typename HistogramCalculationFilterType::LabelSamplePairType LabelSamplePairType;

      
      // typedef std::unordered_map<LabelType,unsigned int> LabeledIntMapType;
      // typedef std::pair<LabelType,unsigned int> LabelIntPairType;

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

	SetDocName(" ");
	SetDocLongDescription(" ");
	SetDocLimitations(" ");
	SetDocAuthors(" ");
	SetDocSeeAlso(" ");

	AddParameter(ParameterType_String,  "in",    "Input image");
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

      void DoUpdateParameters()
      {


      }

      void DoExecute()
      {
	const std::vector<std::string> modelList = GetParameterStringList("models");
	for (unsigned i = 0; i < modelList.size(); i++) {
	  std::stringstream clname;
	  clname << "classifier" << i;
	  AddApplication("ImageClassifier", clname.str(), "Image classifier");
	}
	
	const std::vector<std::string> labellist = GetParameterStringList("lablist");
	std::unordered_map<LabelType,unsigned> labelPositions;
	for (unsigned i = 0; i < labellist.size(); i++) {
	  labelPositions.insert(std::pair<LabelType,unsigned>(atoi(labellist[i].c_str()),i));
	}
	  
	FloatVectorImageReaderType::Pointer imReader = FloatVectorImageReaderType::New();
	imReader->SetFileName(GetParameterString("in"));
	imReader->UpdateOutputInformation();
	
	m_Segreader = LabelReaderType::New();
	m_Segreader->SetFileName(GetParameterString("inseg"));
	m_Segreader->UpdateOutputInformation();
	auto seg = m_Segreader->GetOutput();

	LabelImageType::Pointer prevImage;

	for (unsigned i = 0; i < modelList.size(); i++) {
	  std::cout << "Starting iteration " << i << "\n";

	  std::stringstream clname;
	  clname << "classifier" << i;
	  
	  auto imageClassifier= GetInternalApplication(clname.str());

	  if (i==0) {	    
	    imageClassifier->SetParameterInputImage("in",imReader->GetOutput());
	  }
	  else{
	    
	    std::stringstream pclname;
	    pclname << "classifier" << i-1;
	  
	    auto prevImageClassifier = GetInternalApplication(pclname.str());
	    prevImage=dynamic_cast<LabelImageType*>(prevImageClassifier->GetParameterOutputImage("out"));
	    prevImage->UpdateOutputInformation();

	    //Invoke histogram filter
	    std::cout << "Calculating histograms" << "\n";
	    auto histoCalculationFilter = HistogramCalculationFilterType::New();
	    m_HistoCalculationFilters.push_back(histoCalculationFilter);
	    histoCalculationFilter->SetInput(prevImage);
	    histoCalculationFilter->SetImageSeg(seg);
	    histoCalculationFilter->SetLabelPositions(labelPositions);
	    histoCalculationFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
	    histoCalculationFilter->GetFilter()->SetNumberOfThreads(1);
	    
	    histoCalculationFilter->Update();

	    // std::cout << "here2" << "\n";

	    // LabeledIntMapType* c = histoCalculationFilter->GetFilter()->GetCount();
	    // std::cout << c->size() << "\n";
	    // std::cout << c->begin()->first << "\n";

	    // std::cout << "here3" << "\n";

	    // for (auto & e : (*m)) {
	    //   std::cout << e.first << " ";
	    //   for (unsigned i = 0; i < labellist.size(); i++) {
	    // 	std::cout << e.second[i] << " ";
	    //   }
	    //   std::cout << "\n";
	    // }

	    std::cout << "Writing histogram image" << "\n";
	    auto m_HistoImageWriterFilter = HistogramImageWriterFilterType::New();
	    std::cout << "After New" << "\n";
	    m_HistoImageWriterFilters.push_back(m_HistoImageWriterFilter);
	    std::cout << "1" << "\n";
	    m_HistoImageWriterFilter->SetInput(seg);
	    std::cout << "2" << "\n";
	    // std::cout << m.begin()->second.size() << "\n";
	    m_HistoImageWriterFilter->SetMeans(histoCalculationFilter->GetFilter()->GetMeans());
	    std::cout << "3" << "\n";
	    m_HistoImageWriterFilter->UpdateOutputInformation();
	    std::cout << "Setup histo done" << "\n";

	    // auto w = FloatVectorImageWriterType::New();
	    // w->SetFileName("test.tif");
	    // w->SetInput(histoWriterFilter->GetOutput());
	    // w->Update();

	    ConcatenateImageFilter::Pointer m_conc = ConcatenateImageFilter::New();
	    m_ConcatenateImageFilters.push_back(m_conc);
	    
	    m_conc->SetInput1(imReader->GetOutput());
	    m_conc->SetInput2(m_HistoImageWriterFilter->GetOutput());
	    m_conc->UpdateOutputInformation();
	    std::cout << "setup conc done" << "\n";

	    imageClassifier->SetParameterInputImage("in",m_conc->GetOutput());	    
	  }

	  imageClassifier->SetParameterString("model",modelList[i]);

	  imageClassifier->SetParameterInt("ram",GetParameterInt("ram"));
	  imageClassifier->UpdateParameters();
	  if (i == modelList.size()-1) {
	    //Last iteration
	    std::stringstream outName_s;
	    outName_s << GetParameterString("out") << "result_it_" << i << ".tif";
	    imageClassifier->SetParameterString("out",outName_s.str());
	    imageClassifier->ExecuteAndWriteOutput();
	  }
	  else{
	    std::cout << "bef execute" << "\n";
	    imageClassifier->Execute();
	    
	    std::cout << "af execute" << "\n";
	    // imageClassifier->RegisterPipeline();
	    auto image = imageClassifier->GetParameterImageBase("out");
	    image->UpdateOutputInformation();
	    
	  }

	}

	std::cout << "Finished" << "\n";
	
	//TODO cleanup temp dir
      }

    };
  }
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ClassifyAutoContext)
