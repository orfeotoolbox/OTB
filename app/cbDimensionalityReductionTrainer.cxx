#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"

#include "otbOGRDataSourceWrapper.h"
#include "otbOGRFeatureWrapper.h"

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
	itkTypeMacro(CbDimensionalityReductionTrainer, otb::Wrapper::Application);
private:
	void DoInit()
	{
		SetName("CbDimensionalityReductionTrainer");
		SetDescription("Trainer for the dimensionality reduction algorithms used in the cbDimensionalityReduction application.");

		
		AddParameter(ParameterType_InputVectorData, "train", "Name of the input training vector data");
		SetParameterDescription("train","The vector data used for training.");
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
		//ListSampleType::Pointer input = ListSampleType::New();
		//const int nbFeatures = GetSelectedItems("feat").size();
		//input->SetMeasurementVectorSize(nbFeatures);

	}

};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::CbDimensionalityReductionTrainer)
