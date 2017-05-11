#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

#include "itkVariableLengthVector.h"

#include "otbShiftScaleSampleListFilter.h"
#include "otbStatisticsXMLFileReader.h"

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


	typedef Superclass::SampleType              SampleType;
	typedef Superclass::ListSampleType          ListSampleType;
	typedef Superclass::SampleImageType         SampleImageType;
	  
	typedef double ValueType;
	typedef itk::VariableLengthVector<ValueType> MeasurementType;

	typedef otb::StatisticsXMLFileReader<SampleType> StatisticsReader;

	typedef otb::Statistics::ShiftScaleSampleListFilter<ListSampleType, ListSampleType> ShiftScaleFilterType;



	//typedef float ValueType;
	//typedef itk::VariableLengthVector<ValueType> InputSampleType;
	//typedef itk::Statistics::ListSample<InputSampleType> ListSampleType;
	
	//typedef itk::VariableLengthVector<ValueType> MeasurementType;
	  
	typedef otb::MachineLearningModelFactory<ValueType, ValueType>  ModelFactoryType;
		
private:
	void DoInit()
	{
		SetName("CbDimensionalityReductionTrainer");
		SetDescription("Trainer for the dimensionality reduction algorithms used in the cbDimensionalityReduction application.");
	
		AddParameter(ParameterType_Group, "io", "Input and output data");
		SetParameterDescription("io", "This group of parameters allows setting input and output data.");

		AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector Data");
		SetParameterDescription("io.vd", "Input geometries used for training (note : all geometries from the layer will be used)");

		AddParameter(ParameterType_OutputFilename, "io.out", "Output model");
		SetParameterDescription("io.out", "Output file containing the model estimated (.txt format).");

		AddParameter(ParameterType_InputFilename, "io.stats", "Input XML image statistics file");
		MandatoryOff("io.stats");
		SetParameterDescription("io.stats", "XML file containing mean and variance of each feature.");

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

		std::cout << "Appli Training!" << std::endl;

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
		
// Statistics for shift/scale
		MeasurementType meanMeasurementVector;
		MeasurementType stddevMeasurementVector;
		if (HasValue("io.stats") && IsParameterEnabled("io.stats"))
		{
			StatisticsReader::Pointer statisticsReader = StatisticsReader::New();
			std::string XMLfile = GetParameterString("io.stats");
			statisticsReader->SetFileName(XMLfile);
			meanMeasurementVector = statisticsReader->GetStatisticVectorByName("mean");
			stddevMeasurementVector = statisticsReader->GetStatisticVectorByName("stddev");
		}
		else
		{
			meanMeasurementVector.SetSize(nbFeatures);
			meanMeasurementVector.Fill(0.);
			stddevMeasurementVector.SetSize(nbFeatures);
			stddevMeasurementVector.Fill(1.);
		}
    
		ShiftScaleFilterType::Pointer trainingShiftScaleFilter = ShiftScaleFilterType::New();
		trainingShiftScaleFilter->SetInput(input);
		trainingShiftScaleFilter->SetShifts(meanMeasurementVector);
		trainingShiftScaleFilter->SetScales(stddevMeasurementVector);
		trainingShiftScaleFilter->Update();

		ListSampleType::Pointer trainingListSample= trainingShiftScaleFilter->GetOutput();
	
	
		this->Train(trainingListSample,GetParameterString("io.out"));
	}


	


};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReductionTrainer)
