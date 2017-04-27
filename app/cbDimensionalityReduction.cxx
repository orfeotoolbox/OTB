#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

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
		AddRAMParameter();
	}
	
	void DoUpdateParameters()
	{
	}
	
	void DoExecute()
	{	
		std::cout << "Appli !" << std::endl;
		int ThisDoesNothing = 0;
	}

};

OTB_APPLICATION_EXPORT(CbDimensionalityReduction)
