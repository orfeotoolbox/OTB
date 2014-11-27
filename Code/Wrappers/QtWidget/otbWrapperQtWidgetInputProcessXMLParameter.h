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
#ifndef __otbWrapperQtWidgetInputProcessXMLParameter_h
#define __otbWrapperQtWidgetInputProcessXMLParameter_h

#include <QtGui>
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetInputProcessXMLParameter
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetInputProcessXMLParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetInputProcessXMLParameter(InputProcessXMLParameter*, QtWidgetModel*);
  virtual ~QtWidgetInputProcessXMLParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetInputProcessXMLParameter(const QtWidgetInputProcessXMLParameter&); //purposely not implemented
  void operator=(const QtWidgetInputProcessXMLParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  InputProcessXMLParameter::Pointer m_XMLParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
