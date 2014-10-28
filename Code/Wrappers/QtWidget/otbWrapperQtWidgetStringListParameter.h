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
#include "otbQtStringSelectionWidget.h"

namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
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
  virtual void SupressString();
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
