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
#ifndef __otbWrapperQtWidgetModel_h
#define __otbWrapperQtWidgetModel_h

#include <QtGui>
#include "otbWrapperApplication.h"
#include "otbWrapperChoiceParameter.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetModel : public QObject
{
  Q_OBJECT
public:
  QtWidgetModel(Application* app);

  virtual ~QtWidgetModel();

  Application* GetApplication()
  {
    return m_Application;
  }

protected slots:
  // slot called everytime one of the widget is updated
  void NotifyUpdate();

  // slot called when execution is requested
  void ExecuteAndWriteOutput();

private:
  QtWidgetModel(const QtWidgetModel&); //purposely not implemented
  void operator=(const QtWidgetModel&); //purposely not implemented

  Application::Pointer m_Application;
};


}
}

#endif
