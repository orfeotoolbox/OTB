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
#ifndef __otbQtStringSelectionWidget_h
#define __otbQtStringSelectionWidget_h

#include <QtGui>
#include "otbWrapperStringListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtStringSelectionWidget : public QWidget
{
  Q_OBJECT
public:
  QtStringSelectionWidget();
  virtual ~QtStringSelectionWidget();

  bool IsChecked()
  {
    return m_Checkbox->isChecked();
  }

  void SetChecked( bool val )
  {
    return m_Checkbox->setChecked( val );
  }

  std::string GetStringName()
  {
    return static_cast<const char*>(m_Input->text().toAscii());
  }

  void ClearStringName()
  {
    m_Input->clear();
  }

  QLineEdit* GetInput()
  {
    return m_Input;
  }

private:
  QtStringSelectionWidget(const QtStringSelectionWidget&); //purposely not implemented
  void operator=(const QtStringSelectionWidget&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QCheckBox *   m_Checkbox;
};


}
}

#endif
