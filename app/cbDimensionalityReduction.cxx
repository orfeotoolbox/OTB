#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"

#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"


#include <shark/Models/Autoencoder.h>//normal autoencoder model

#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>

#include "encode_filter.h"
//#include "dummy_filter.h"

namespace otb
{
namespace Wrapper
{

class CbDimensionalityReduction : public otb::Wrapper::Application
{
public:
	/** Standard class typedefs. */
	typedef CbDimensionalityReduction Self;	
	typedef Application Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	

	/** Standard macro */
	itkNewMacro(Self);
	itkTypeMacro(CbDimensionalityReduction, otb::Application);
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
		typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
		using image_type = FloatVectorImageType;
		using FilterType = EncodeFilter<image_type, AutoencoderType, shark::Normalizer<shark::RealVector>> ;
		using DummyFilterType = DummyFilter<image_type> ;
		
		std::cout << "Appli !" << std::endl;
		FloatVectorImageType::Pointer inImage = GetParameterImage("in");
		std::string encoderPath = GetParameterString("model");
		std::string normalizerPath = GetParameterString("normalizer");
		
		FilterType::Pointer filter = FilterType::New();
		filter->SetAutoencoderModel(encoderPath);
		filter->SetNormalizerModel(normalizerPath);
		filter->SetInput(inImage);
		filter->Update();
		SetParameterOutputImage("out", filter->GetOutput());

/*
		DummyFilterType::Pointer dummy_filter = DummyFilterType::New(); // this filter simply copies the input image (do not need shark library)
		dummy_filter->SetInput(GetParameterFloatVectorImage("in"));
		dummy_filter->Update();
		dummy_filter->UpdateOutputInformation();
		SetParameterOutputImage("out", dummy_filter->GetOutput());

*/
		
		//SetParameterOutputImage("out", inImage); // copy input image

	}

};
}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReduction)
