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
#ifndef __otbWrapperQtWidgetOutputProcessXMLParameter_h
#define __otbWrapperQtWidgetOutputProcessXMLParameter_h

#include <QtGui>
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetOutputProcessXMLParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputProcessXMLParameter(OutputProcessXMLParameter*, QtWidgetModel*);
  virtual ~QtWidgetOutputProcessXMLParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetOutputProcessXMLParameter(const QtWidgetOutputProcessXMLParameter&); //purposely not implemented
  void operator=(const QtWidgetOutputProcessXMLParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  OutputProcessXMLParameter::Pointer m_XMLParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
