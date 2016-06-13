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
#ifndef otbQtStringSelectionWidget_h
#define otbQtStringSelectionWidget_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperStringListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtStringSelectionWidget
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtStringSelectionWidget : public QWidget
{
  Q_OBJECT

  Q_PROPERTY( QString text READ GetText WRITE SetText RESET ClearText );

signals:
  void InternalQLineEditEditionFinished();

public:
  QtStringSelectionWidget();
  ~QtStringSelectionWidget() ITK_OVERRIDE;

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
