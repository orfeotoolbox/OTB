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
#include "otbWrapperApplication.h"
#include "otbWrapperQtWidgetModel.h"
#include "itkQtProgressBar.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief Create a widget reporting the progress of the application
 *        process. 
 */
class QtWidgetProgressReport : public QWidget//QDockWidget
{
  Q_OBJECT
public:
  QtWidgetProgressReport(QtWidgetModel * model);
  virtual ~QtWidgetProgressReport();

  void SetApplication(Application::Pointer app);

public slots:
  void ReportProgress();

private:
  QtWidgetProgressReport(const QtWidgetProgressReport&); //purposely not implemented
  void operator=(const QtWidgetProgressReport&); //purposely not implemented

  Application::Pointer                 m_Application;
  QtWidgetModel *                      m_Model;
  
  std::vector< itk::QtProgressBar * >  m_BarListIntern;
  std::vector< itk::QtProgressBar * >  m_BarListWriter;
  std::vector< QLabel * >              m_LabelListIntern;
  std::vector< QLabel * >              m_LabelListWriter;
  
  bool                                 m_IsProgressReportGuiAlreadyBuilt;
};


}
}

#endif
