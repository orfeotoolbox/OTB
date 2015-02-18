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

/** \class QtStringSelectionWidget
 * \brief
 */
class ITK_ABI_EXPORT QtStringSelectionWidget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY( QString text READ GetText WRITE SetText RESET ClearText );

signals:
  void InternalQLineEditEditionFinished();

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

  inline const QString GetText() const
  {
    return m_Input->text();
  }

  inline void SetText( const QString& qString)
  {
    m_Input->setText(qString);
  }

  std::string ToStdString()
  {
    return m_Input->text().toAscii().constData();
  }

  void ClearText()
  {
    m_Input->clear();
  }


protected slots:
   void OnEditionFinished();

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
