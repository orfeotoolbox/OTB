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
#ifndef __otbWrapperQtWidgetProgressReport_h
#define __otbWrapperQtWidgetProgressReport_h

#include <QtGui>
#include "otbWrapperQtWidgetModel.h"
#include "itkQtProgressBar.h"
#include "itkProcessObject.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetProgressReport
 * \brief Create a widget reporting the progress of the application
 *        process.
 */
class ITK_ABI_EXPORT QtWidgetProgressReport : public QWidget
{
  Q_OBJECT
public:
  QtWidgetProgressReport(QtWidgetModel * model);
  virtual ~QtWidgetProgressReport();

  void SetApplication(Application::Pointer app);

  typedef itk::MemberCommand< QtWidgetProgressReport >  AddProcessCommandType;

  void ProcessEvent( itk::Object * caller, const itk::EventObject & event );

  std::string GetCurrentDescription()
  {
    return m_CurrentDescription;
  }

/*
  itk::ProcessObject* GetCurrentProcess()
  {
    return m_CurrentProcess;
  }

  QtWidgetModel * GetModel()
  {
    m_Model;
  }

  QVBoxLayout * GetLayout()
  {
    return m_Layout;
  }
*/
public slots:
  void RemoveLayout();
  /*virtual */void ReportProcess();

  signals:
  void AddNewProcessToReport();

private:
  QtWidgetProgressReport(const QtWidgetProgressReport&); //purposely not implemented
  void operator=(const QtWidgetProgressReport&); //purposely not implemented

  Application::Pointer              m_Application;
  QtWidgetModel *                   m_Model;
  QVBoxLayout *                     m_Layout;

  AddProcessCommandType::Pointer    m_AddProcessCommand;
  itk::ProcessObject*               m_CurrentProcess;
  std::string                       m_CurrentDescription;
};

}
}

#endif
