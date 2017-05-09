#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "itkVariableLengthVector.h"

//#include "AutoencoderModel.h"

#include "otbSharkUtils.h"
#include "otbMachineLearningModel.h"
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

#include "otbMachineLearningModelFactory.h"

#include "cbLearningApplicationBaseDR.h"

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
class CbDimensionalityReductionTrainer : public cbLearningApplicationBaseDR<float,float>
{
public:
	typedef CbDimensionalityReductionTrainer Self;
	typedef cbLearningApplicationBaseDR<float, float> Superclass;
	typedef itk::SmartPointer<Self> Pointer;
	typedef itk::SmartPointer<const Self> ConstPointer;
	
	itkNewMacro(Self);
	itkTypeMacro(CbDimensionalityReductionTrainer, otb::Application);


	typedef float ValueType;
	typedef itk::VariableLengthVector<ValueType> InputSampleType;
	typedef itk::Statistics::ListSample<InputSampleType> ListSampleType;
	
	typedef itk::VariableLengthVector<ValueType> MeasurementType;
	  
	typedef otb::MachineLearningModelFactory<ValueType, ValueType>  ModelFactoryType;
		
	typedef shark::Autoencoder< shark::TanhNeuron, shark::LinearNeuron> AutoencoderType;
	typedef AutoencoderModel<ValueType,AutoencoderType> AutoencoderModelType;
typedef RandomForestsMachineLearningModel<ValueType,int> rfModelType;

private:
	void DoInit()
	{
		SetName("CbDimensionalityReductionTrainer");
		SetDescription("Trainer for the dimensionality reduction algorithms used in the cbDimensionalityReduction application.");
		/*
		AddParameter(ParameterType_InputVectorData, "train", "Name of the input training vector data");
		SetParameterDescription("train","The vector data used for training.");
	
		AddParameter(ParameterType_StringList, "feat", "Field names to be calculated."); //
		SetParameterDescription("feat","List of field names in the input vector data used as features for training."); //
		
		AddParameter(ParameterType_Int, "k","target dimension");
		SetParameterDescription("k", "Dimension of the output feature vectors");
*/
		AddParameter(ParameterType_Group, "io", "Input and output data");
		SetParameterDescription("io", "This group of parameters allows setting input and output data.");

		AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector Data");
		SetParameterDescription("io.vd", "Input geometries used for training (note : all geometries from the layer will be used)");

		AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
		SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");


		AddParameter(ParameterType_StringList, "feat", "Field names to be calculated."); //
		SetParameterDescription("feat","List of field names in the input vector data used as features for training."); //
		
		Superclass::DoInit();


		/*
		AddParameter(ParameterType_InputFilename, "model", "Dimensionality Reduction model file");
		SetParameterDescription("model", "A model file (produced by cbDimensionalityReduction application.");

		AddParameter(ParameterType_InputFilename, "normalizer", "normalizer model file");
		SetParameterDescription("normalizer", "A model file (produced by cbDimensionalityReduction application.");
*/
		
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

		std::string shapefile = GetParameterString("io.vd");

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
		/*
		std::cout << input << std::endl;
		std::vector<shark::RealVector> features;
		otb::Shark::ListSampleToSharkVector<ListSampleType>( input, features);
		shark::Data<shark::RealVector> inputSamples = shark::createDataFromRange( features );
		
		std::size_t numHidden= GetParameterInt("k");   
		std::size_t iterations = 100;
		double regularisation = 0; 
		

		shark::Normalizer<shark::RealVector> normalizer = trainNormalizer(inputSamples);
		inputSamples = normalizer(inputSamples);
	
		std::cout << "normalizer trained and training set normalized" << std::endl;

		AutoencoderType net = trainAutoencoderModel<AutoencoderType>(inputSamples,numHidden,iterations,regularisation);
		std::cout << "autoencoder trained !!!!" << std::endl;

		// save the model to the file "net.model"
		std::ofstream ofs("net.model");
		shark::TextOutArchive oa(ofs);
		net.write(oa);
		ofs.close();
	
		// save the model to the file "net.model"
		std::ofstream norm_ofs("normalizer.model");
		boost::archive::polymorphic_text_oarchive onorm(norm_ofs);
		normalizer.write(onorm);
		norm_ofs.close();
	*/

		std::cout << "Using a Machine learning model" << std::endl;
		/*
		AutoencoderModelType::Pointer dimredTrainer = AutoencoderModelType::New();
		dimredTrainer->SetNumberOfHiddenNeurons(5);
		dimredTrainer->SetNumberOfIterations(50);
		dimredTrainer->SetRegularization(0.1);
		dimredTrainer->SetInputListSample(input);
		dimredTrainer->Train();
		dimredTrainer->Save("net.model");
		std::cout << "ok" << std::endl;
		*/
		this->Train(input,GetParameterString("io.out"));
		
	}


	


};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReductionTrainer)
