#include "otbBasicApplicationView.h"

#include "otbMsgReporter.h"

#include <FL/Fl_File_Chooser.H>
#include <FL/fl_draw.H>

namespace otb
{

BasicApplicationView::BasicApplicationView(): m_Controller(), m_Model(), m_ImageView()
{
  m_Model = BasicApplicationModel::GetInstance();
  m_Model->RegisterListener(this);

  // Build the visualization part
  m_ImageView = ImageViewType::New();
  m_ImageView->SetModel(m_Model->GetVisualizationModel());


}

BasicApplicationView::~BasicApplicationView()
{
  // Remove registered visualization components from the interface
  gImageViewer->remove(m_ImageView->GetFullWidget());
  gScroll->remove(m_ImageView->GetScrollWidget());
}

void BasicApplicationView::Build()
{
  if(!m_Controller)
    {
    itkExceptionMacro(<<"Controller is not set, can not build view.");
    }

  if(!m_WidgetsController)
    {
    itkExceptionMacro(<<"Widgets controller is not set, can not build view.");
    }
  // Build the fltk code
  BasicApplicationViewGUI::Build();

  // Register controllers
   m_ImageView->SetController(m_WidgetsController);

   // Remove registered visualization components from the interface
   gImageViewer->add(m_ImageView->GetFullWidget());
   gScroll->add(m_ImageView->GetScrollWidget());

   gImageViewer->resizable(m_ImageView->GetFullWidget());
   gScroll->resizable(m_ImageView->GetScrollWidget());

   m_ImageView->GetFullWidget()->resize(gImageViewer->x(),gImageViewer->y(),gImageViewer->w(),gImageViewer->h());
   m_ImageView->GetScrollWidget()->resize(gScroll->x(),gScroll->y(),gScroll->w(),gScroll->h());

   // Show and refresh the interface
    this->wMainWindow->show();

    m_ImageView->GetFullWidget()->show();
    m_ImageView->GetScrollWidget()->show();


    this->RefreshInterface();
}
void BasicApplicationView::Notify()
{
  this->RefreshInterface();
  this->RefreshVisualization();
  Fl::flush();

}

void BasicApplicationView::RefreshVisualization()
{
  m_ImageView->Update();
}

void BasicApplicationView::RefreshInterface()
{

}

void BasicApplicationView::Exit()
{
  MsgReporter::GetInstance()->Hide();
  wMainWindow->hide();
}

void BasicApplicationView::OpenImage()
{
  const char * cfname = fl_file_chooser("Select an Input Image", "*.*",".");
  Fl::check();
  wMainWindow->redraw();
  if (cfname == NULL || strlen(cfname)<1)
  {
    return;
  }
  m_Controller->OpenImage( cfname );
}

}// end namespace
