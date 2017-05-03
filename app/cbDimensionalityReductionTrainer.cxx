#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "itkVariableLengthVector.h"

#include "AutoencoderModel.h"

#include "otbSharkUtils.h"

//include train function
#include <shark/ObjectiveFunctions/ErrorFunction.h>
#include <shark/Algorithms/GradientDescent/Rprop.h>// the RProp optimization algorithm
#include <shark/ObjectiveFunctions/Loss/SquaredLoss.h> // squared loss used for regression
#include <shark/ObjectiveFunctions/Regularizer.h> //L2 regulariziation

#include <fstream> // write the model file


#include <shark/Models/Autoencoder.h>//normal autoencoder model
#include <shark/Models/TiedAutoencoder.h>//autoencoder with tied weights
#include <shark/Models/Normalizer.h>

#include <shark/Algorithms/Trainers/NormalizeComponentsUnitVariance.h>


template<class AutoencoderModel>
AutoencoderModel trainAutoencoderModel(
        shark::UnlabeledData<shark::RealVector> const& data,//the data to train with
        std::size_t numHidden,//number of features in the autoencoder
        std::size_t iterations, //number of iterations to optimize
        double regularisation//strength of the regularisation
){
	//create the model
	std::size_t inputs = dataDimension(data);
	AutoencoderModel model;
	model.setStructure(inputs, numHidden);
	initRandomUniform(model,-0.1*std::sqrt(1.0/inputs),0.1*std::sqrt(1.0/inputs));
	//create the objective function

	shark::LabeledData<shark::RealVector,shark::RealVector> trainSet(data,data);//labels identical to inputs
	shark::SquaredLoss<shark::RealVector> loss;
	shark::ErrorFunction error(trainSet, &model, &loss);
	shark::TwoNormRegularizer regularizer(error.numberOfVariables());
	error.setRegularizer(regularisation,&regularizer);

	shark::IRpropPlusFull optimizer;
	error.init();
	optimizer.init(error);
	std::cout<<"Optimizing model: "+model.name()<<std::endl;
	for(std::size_t i = 0; i != iterations; ++i){
		optimizer.step(error);
		std::cout<<i<<" "<<optimizer.solution().value<<std::endl;
	}
	//std::cout<<optimizer.solution().value<<std::endl;
	model.setParameterVector(optimizer.solution().point);
	return model;
}

shark::Normalizer<shark::RealVector> trainNormalizer(const shark::UnlabeledData<shark::RealVector>& data)
{	
	bool removeMean = true;
	shark::Normalizer<shark::RealVector> normalizer;
	shark::NormalizeComponentsUnitVariance<shark::RealVector> normalizingTrainer(removeMean);
	normalizingTrainer.train(normalizer, data);
	return normalizer;
}


namespace otb
{
namespace Wrapper
{
class CbDimensionalityReductionTrainer : public otb::Wrapper::Application
{
public:
	typedef CbDimensionalityReductionTrainer Self;
	typedef itk::SmartPointer<Self> Pointer;
	itkNewMacro(Self);
	itkTypeMacro(CbDimensionalityReductionTrainer, otb::Application);


	typedef double ValueType;
	typedef itk::VariableLengthVector<ValueType> InputSampleType;
	typedef itk::Statistics::ListSample<InputSampleType> ListSampleType;
	
	typedef itk::VariableLengthVector<ValueType> MeasurementType;


private:
	void DoInit()
	{
		SetName("CbDimensionalityReductionTrainer");
		SetDescription("Trainer for the dimensionality reduction algorithms used in the cbDimensionalityReduction application.");
		AddParameter(ParameterType_InputVectorData, "train", "Name of the input training vector data");
		SetParameterDescription("train","The vector data used for training.");
	
		AddParameter(ParameterType_StringList, "feat", "Field names to be calculated."); //
		SetParameterDescription("feat","List of field names in the input vector data used as features for training."); //

/*
		AddParameter(ParameterType_InputVectorData, "val", "Name of the input validation vector data");
		SetParameterDescription("val","The vector data used for validation.");
*/
		AddRAMParameter();
	}
	
	void DoUpdateParameters()
	{
	}
	
	void DoExecute()
	{	

		std::cout << "Appli !" << std::endl;

		std::string shapefile = GetParameterString("train");

		otb::ogr::DataSource::Pointer source = otb::ogr::DataSource::New(shapefile, otb::ogr::DataSource::Modes::Read);
		otb::ogr::Layer layer = source->GetLayer(0);
		ListSampleType::Pointer input = ListSampleType::New();
		const int nbFeatures = GetParameterStringList("feat").size();

		input->SetMeasurementVectorSize(nbFeatures);
		otb::ogr::Layer::const_iterator it = layer.cbegin();
		otb::ogr::Layer::const_iterator itEnd = layer.cend();
		for( ; it!=itEnd ; ++it)
		{
			MeasurementType mv;
			mv.SetSize(nbFeatures);
			for(int idx=0; idx < nbFeatures; ++idx)
			{
				mv[idx] = (*it)[GetParameterStringList("feat")[idx]].GetValue<double>();
			}
			input->PushBack(mv);
		}
		std::cout << input << std::endl;
		std::vector<shark::RealVector> features;
		otb::Shark::ListSampleToSharkVector<ListSampleType>( input, features);
		shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
		
		std::size_t numHidden= 5;   // stoi convert string to int
		std::size_t iterations = 10;
		double regularisation = 0; // stod convert string to double
		

		shark::Normalizer<shark::RealVector> normalizer = trainNormalizer(inputSamples);
		inputSamples = normalizer(inputSamples);
	
		std::cout << "normalizer trained and training set normalized" << std::endl;

		typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
		AutoencoderType net = trainAutoencoderModel<AutoencoderType>(inputSamples,numHidden,iterations,regularisation);
		std::cout << "autoencoder trained !!" << std::endl;

		// save the model to the file "net.model"
		std::ofstream ofs("net.model");
		boost::archive::polymorphic_text_oarchive oa(ofs);
		net.write(oa);
		ofs.close();
	
		// save the model to the file "net.model"
		std::ofstream norm_ofs("normalizer.model");
		boost::archive::polymorphic_text_oarchive onorm(norm_ofs);
		normalizer.write(onorm);
		norm_ofs.close();
	
	}

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReductionTrainer)
