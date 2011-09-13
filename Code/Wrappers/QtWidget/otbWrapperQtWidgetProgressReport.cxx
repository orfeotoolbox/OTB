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

#include "otbWrapperQtWidgetProgressReport.h"
#include "otbWrapperOutputImageParameter.h"
#include "itksys/SystemTools.hxx"

namespace otb
{
namespace Wrapper
{

QtWidgetProgressReport::QtWidgetProgressReport(QtWidgetModel * model) : m_IsProgressReportGuiAlreadyBuilt(false)
{
  m_Model = model;
  connect(model, SIGNAL(SetProgressReportBegin()), this, SLOT(ReportProgress()) ); 
  connect(model, SIGNAL(SetProgressReportDone()), this, SLOT(close()) );
}

QtWidgetProgressReport::~QtWidgetProgressReport()
{

}

void QtWidgetProgressReport::SetApplication(Application::Pointer app)
{
  m_Application = app;
}

void QtWidgetProgressReport::ReportProgress()
{
  // show the widget if closed due to a previous execution
  this->show();

  if(!m_IsProgressReportGuiAlreadyBuilt)
    {  
    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);
  
    if( m_Application->GetInternalProcessList().size() != m_Application->GetInternalProcessListName().size())
      {
      itkGenericExceptionMacro ("Internal process list and list name size mismatch...");
      }

    // Build the window : First internal process
    for (unsigned int ii=0; ii < m_Application->GetInternalProcessList().size(); ii++)
      {
      // Create a itk::QtProgressBar, observing the event ProgressEvent
      itk::QtProgressBar * bar =  new itk::QtProgressBar(this);
      connect( bar, SIGNAL(SetValueChanged(int)), bar, SLOT(setValue(int)) );
      connect( m_Model, SIGNAL(SetProgressReportDone()), bar, SLOT(reset()) );
      bar->Observe(m_Application->GetInternalProcessList()[ii]);

      // label
      QLabel *label = new QLabel(QString(m_Application->GetInternalProcessListName()[ii].c_str()));
    
      // Build the layout and store the pointers
      layout->addWidget(label);
      layout->addWidget(bar);
      m_BarListIntern.push_back(bar);
      m_LabelListIntern.push_back(label);
      }
  
    // Build the window : then writers
    unsigned int nbOutput = 0;
    std::vector<std::string> paramList = m_Application->GetParametersKeys(true);
    for (std::vector<std::string>::const_iterator it = paramList.begin();
         it != paramList.end();
         ++it)
      {
      if ( m_Application->GetParameterType(*it) == ParameterType_OutputImage)
        {
        Parameter* param =  m_Application->GetParameterByKey(*it);
        OutputImageParameter* outputParam = dynamic_cast<OutputImageParameter*>(param);

        // create the label including the output description
        itk::OStringStream oss;
        oss << "Writer "<< nbOutput << ": ";
        oss << outputParam->GetName() <<".";
        QLabel *label = new QLabel(QString(oss.str().c_str()));

        // Create a itk::QtProgressBar, observing the event ProgressEvent
        itk::QtProgressBar * bar =  new itk::QtProgressBar(this);
        connect( bar, SIGNAL(SetValueChanged(int)), bar, SLOT(setValue(int)) );
        connect( m_Model, SIGNAL(SetProgressReportDone()), bar, SLOT(reset()) );
      
        bar->Observe(outputParam->GetWriter());
      
        // Build the layout and store the pointers
        bar->setToolTip( QString( outputParam->GetDescription()) );
        layout->addWidget(label);
        layout->addWidget(bar);
        m_BarListWriter.push_back(bar);
        m_LabelListWriter.push_back(label);
        }
      }
    
    m_IsProgressReportGuiAlreadyBuilt = true;
    }
}

}
}
