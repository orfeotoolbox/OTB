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
#ifndef __otbWrapperQtWidgetStringListParameter_h
#define __otbWrapperQtWidgetStringListParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbQtStringSelectionWidget.h"
#endif //tag=QT4-boost-compatibility

namespace otb
{
namespace Wrapper
{

/** \class QtWidgetStringListParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class ITK_ABI_EXPORT QtWidgetStringListParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetStringListParameter(StringListParameter*, QtWidgetModel*);
  virtual ~QtWidgetStringListParameter();

signals:
  void Change();

protected slots:
  void SetString( const QString& value );
  virtual void AddString();
  virtual void SuppressString();
  virtual void UpdateStringList();

private:
  QtWidgetStringListParameter(const QtWidgetStringListParameter&); //purposely not implemented
  void operator=(const QtWidgetStringListParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  StringListParameter::Pointer m_StringListParam;

  QHBoxLayout * m_HLayout;
  QVBoxLayout * m_StringLayout;
  QScrollArea * m_Scroll;

  std::vector<QtStringSelectionWidget *> m_LineEditList;
};


}
}

#endif
