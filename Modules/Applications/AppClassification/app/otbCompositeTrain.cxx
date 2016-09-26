/*=========================================================================
  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Wrappers
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperApplicationRegistry.h"
#include "otbWrapperAddProcessToWatchEvent.h"
#include "itkStdStreamLogOutput.h"

namespace otb
{
namespace Wrapper
{

class CompositeTrain : public Application
{
public:
  /** Standard class typedefs. */
  typedef CompositeTrain            Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(CompositeTrain, otb::Application);

  /** Filters typedef */
  typedef itk::MemberCommand< Self >        AddProcessCommandType;

protected:
  void LinkWatchers(itk::Object * itkNotUsed(caller), const itk::EventObject & event)
    {
    if (typeid(AddProcessToWatchEvent) == typeid( event ))
      {
      this->InvokeEvent(event);
      }
    }

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("CompositeTrain");
    SetDescription("Composite application for classifier training.");
    SetDocName("Composite Train");
    SetDocLongDescription(" TODO");
    SetDocLimitations(" TODO");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    std::string appName("PolygonClassStatistics");
    m_PolygonAnalysis = ApplicationRegistry::CreateApplication(appName);

    appName = "SampleSelection";
    m_SampleSelection = ApplicationRegistry::CreateApplication(appName);

    // share parameters with PolygonClassStatistics
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("in"));
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("vec"));
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("field"));
    this->GetParameterList()->AddParameter(m_PolygonAnalysis->GetParameterByKey("layer"));

    // share parameters with SampleSelection
    this->GetParameterList()->AddParameter(m_SampleSelection->GetParameterByKey("out"));
    this->GetParameterList()->AddParameter(m_SampleSelection->GetParameterByKey("strategy"));

    m_LogOutput = itk::StdStreamLogOutput::New();
    m_LogOutput->SetStream(m_Oss);

    m_PolygonAnalysis->GetLogger()->AddLogOutput(m_LogOutput);
    m_PolygonAnalysis->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);
    m_SampleSelection->GetLogger()->AddLogOutput(m_LogOutput);
    m_SampleSelection->GetLogger()->SetTimeStampFormat(itk::LoggerBase::HUMANREADABLE);

    // Progress
    // Add the callback to be added when a AddProcessToWatch event is invoked
    m_AddProcessCommand = AddProcessCommandType::New();
    m_AddProcessCommand->SetCallbackFunction(this, &CompositeTrain::LinkWatchers);
    m_PolygonAnalysis->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());
    m_SampleSelection->AddObserver(AddProcessToWatchEvent(), m_AddProcessCommand.GetPointer());

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "clLabeledImageQB123_1.tif");
    SetDocExampleParameterValue("io.out", "clLabeledImageQB123_1_CMR_r2_nodl_10_undl_7.tif");
    SetDocExampleParameterValue("ip.radius", "2");
    SetDocExampleParameterValue("ip.suvbool", "true");
    SetDocExampleParameterValue("ip.nodatalabel", "10");
    SetDocExampleParameterValue("ip.undecidedlabel", "7");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // copy common values
    // "in" -> "SampleSelection.in"
    // "vec" -> "SampleSelection.in"
    // "field" -> "SampleSelection.in"
    // "layer" -> "SampleSelection.in"
    m_SampleSelection->SetParameterString("in",this->GetParameterString("in"));
    m_SampleSelection->SetParameterString("vec",this->GetParameterString("vec"));
    m_SampleSelection->SetParameterString("field",this->GetParameterString("field"));
    m_SampleSelection->SetParameterInt("layer",this->GetParameterInt("layer"));

    if (HasValue("out"))
      {
      // TODO : get dirname, and prepare name of temporary output
      m_PolygonAnalysis->SetParameterString("out", std::string("foo.xml"));
      m_SampleSelection->SetParameterString("instats", std::string("foo.xml"));
      }

    // DoUpdateParameters on sub-application
    m_PolygonAnalysis->UpdateParameters();
    m_SampleSelection->UpdateParameters();
  }

  void DoExecute() ITK_OVERRIDE
  {
    otbAppLogINFO(<< "Polygon analysis...");
    m_PolygonAnalysis->Execute();
    otbAppLogINFO(<< "\n" << m_Oss.str());
    m_Oss.str(std::string(""));
    otbAppLogINFO(<< "Sample selection...");
    m_SampleSelection->Execute();
    otbAppLogINFO(<< "\n" << m_Oss.str());
    m_Oss.str(std::string(""));

  }// END DoExecute()

  Application::Pointer m_PolygonAnalysis;

  Application::Pointer m_SampleSelection;

  itk::StdStreamLogOutput::Pointer  m_LogOutput;

  std::ostringstream m_Oss;

  AddProcessCommandType::Pointer    m_AddProcessCommand;

}; // END class CompositeTrain


}// END namespace wrapper
}// END namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::CompositeTrain)
