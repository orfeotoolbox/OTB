#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

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
	}
	
	void DoUpdateParameters()
	{
	}
	
	void DoExecute()
	{
		int ThisDoesNothing = 0;
	}

};

OTB_APPLICATION_EXPORT(CbDimensionalityReductionTrainer)
