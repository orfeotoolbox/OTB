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
#ifndef __otbWrapperQtWidgetFilenameParameter_h
#define __otbWrapperQtWidgetFilenameParameter_h

#include <QtGui>
#include "otbWrapperFilenameParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetFilenameParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetFilenameParameter(FilenameParameter*, QtWidgetModel*);
  virtual ~QtWidgetFilenameParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetFilenameParameter(const QtWidgetFilenameParameter&); //purposely not implemented
  void operator=(const QtWidgetFilenameParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  FilenameParameter::Pointer m_FilenameParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
