#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

class MPIHelloApp : public otb::Wrapper::Application
{
public:
  typedef MPIHelloApp Self;
  typedef itk::SmartPointer<Self> Pointer; 

  itkNewMacro(Self);
  itkTypeMacro(MPIHelloApp, otb::Wrapper::Application);
private:
  void DoInit()
  {
    SetName("MPIHelloApp");
    SetDescription("Empty application.");
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {  
    int ThisDoesNothing = 0;   
  }
};

OTB_APPLICATION_EXPORT(MPIHelloApp)
