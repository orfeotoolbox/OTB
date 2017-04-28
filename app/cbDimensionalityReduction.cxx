#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"

#include <iostream>

#include "otbImage.h"
//#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"


#include <shark/Models/Autoencoder.h>//normal autoencoder model

#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>

#include "encode_filter.h"

namespace otb
{
namespace Wrapper
{

class CbDimensionalityReduction : public otb::Wrapper::Application
{
public:
	typedef CbDimensionalityReduction Self;
	typedef itk::SmartPointer<Self> Pointer;
	itkNewMacro(Self);
	itkTypeMacro(CbDimensionalityReduction, otb::Wrapper::Application);
private:
	void DoInit()
	{
		SetName("CbDimensionalityReduction");
		SetDescription("Perform dimensionality reduction on the input image");

		AddParameter(ParameterType_InputImage, "in", "Input Image");
		SetParameterDescription( "in", "The input image to perform dimensionality reduction on.");

		AddParameter(ParameterType_InputFilename, "model", "Model file");
		SetParameterDescription("model", "A model file (produced by the cbDimensionalityReductionTrainer application).");

		AddParameter(ParameterType_InputFilename, "normalizer", "Normalizer model file");
		SetParameterDescription("normalizer", "A normalizer model file (produced by the cbDimensionalityReductionTrainer application).");

		AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    		SetParameterDescription("out", "Output image");

		AddRAMParameter();
	}
	
	void DoUpdateParameters()
	{
	}
	
	void DoExecute()
	{	
		typedef shark::TiedAutoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
	
		using image_type = otb::VectorImage<double, 2>;
		using FilterType = EncodeFilter<image_type, AutoencoderType, shark::Normalizer<shark::RealVector>> ;
		
		std::cout << "Appli !" << std::endl;
		FloatVectorImageType::Pointer inImage = GetParameterImage("in");
		std::string encoderPath = GetParameterString("model");
		std::string normalizerPath = GetParameterString("normalizer");
		
		//inImage->UpdateOutputInformation();
		
		using ReaderType = otb::ImageFileReader<image_type>;
	
		ReaderType::Pointer reader = ReaderType::New();
		reader->SetFileName("/mnt/data/home/vincenta/features_2014/SL_MultiTempGapF_Brightness_NDVI_NDWI__.tif");
	
		FilterType::Pointer filter = FilterType::New();
		filter->SetAutoencoderModel(encoderPath);
		filter->SetNormalizerModel(normalizerPath);
		filter->SetInput(reader->GetOutput());
		//filter->SetInput(inImage);
		
		filter->UpdateOutputInformation();
		
		SetParameterOutputImage("out", filter->GetOutput());

	}

};
}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReduction)
