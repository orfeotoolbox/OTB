#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include <iostream>

#include "otbImage.h"
#include "otbVectorImage.h"

#include <shark/Models/Autoencoder.h>//normal autoencoder model
#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>
#include "encode_filter.h"

#include "dummy_filter.h"
#include "otbMultiChannelExtractROI.h"

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
	

	using image_type = FloatVectorImageType;
		
	typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
	using FilterType = EncodeFilter<image_type, AutoencoderType, shark::Normalizer<shark::RealVector>> ;
	typedef otb::MultiChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType> ExtractROIFilterType;
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
		std::cout << "Appli" << std::endl;
		image_type::Pointer inImage = GetParameterImage("in");
		std::string encoderPath = GetParameterString("model");
		std::string normalizerPath = GetParameterString("normalizer");
		
		

		filter_dim_reduc = FilterType::New();
		filter_dim_reduc->SetAutoencoderModel(encoderPath);
		filter_dim_reduc->SetNormalizerModel(normalizerPath);
		filter_dim_reduc->SetInput(inImage);
		
		//SetParameterOutputImage("out", filter_dim_reduc->GetOutput());

		m_ExtractROIFilter = ExtractROIFilterType::New();
		m_ExtractROIFilter->SetInput(filter_dim_reduc->GetOutput());
		for (unsigned int idx = 1; idx <= filter_dim_reduc->GetDimension(); ++idx)
		{
		m_ExtractROIFilter->SetChannel(idx );
		}

		SetParameterOutputImage("out", m_ExtractROIFilter->GetOutput());

		//SetParameterOutputImage("out", inImage); // copy input image

	}

	FilterType::Pointer filter_dim_reduc;
	ExtractROIFilterType::Pointer m_ExtractROIFilter;

};
}
}
OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReduction)
