#include "otbBasicApplicationController.h"
#include "otbMsgReporter.h"
#include "otbFltkFilterWatcher.h"
#include <FL/fl_ask.H>

namespace otb
{

BasicApplicationController
::BasicApplicationController(): m_View(), m_WidgetsController(),
m_ResizingHandler(), m_ChangeRegionHandler()
{
  // Build the widgets controller
  m_WidgetsController         = WidgetsControllerType::New();

  // Build the action handlers
  m_ResizingHandler           = ResizingHandlerType::New();
  m_ChangeRegionHandler       = ChangeRegionHandlerType::New();

  // Register the model to the action handlers
  m_ResizingHandler->SetModel(m_Model->GetVisualizationModel());
  m_ChangeRegionHandler->SetModel(m_Model->GetVisualizationModel());


  // Add the action handlers to the widgets controller
  m_WidgetsController->AddActionHandler(m_ResizingHandler);
  m_WidgetsController->AddActionHandler(m_ChangeRegionHandler);
}

BasicApplicationController
::~BasicApplicationController()
{
  MsgReporter::GetInstance()->Hide();
}

void BasicApplicationController::SetView(BasicApplicationView * view)
{
  m_View = view;
  m_ResizingHandler->SetView(m_View->GetImageView());
  m_ChangeRegionHandler->SetView(m_View->GetImageView());
}

void
BasicApplicationController
::OpenImage( const char * filename )
{
  try
    {
    m_Model->OpenImage(filename);
    }
  catch(itk::ExceptionObject & err)
    {
    MsgReporter::GetInstance()->SendError(err.GetDescription());
    }
}

} // end namespace otb
