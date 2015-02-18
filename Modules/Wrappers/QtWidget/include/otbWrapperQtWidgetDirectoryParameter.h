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
#ifndef __otbWrapperQtWidgetDirectoryParameter_h
#define __otbWrapperQtWidgetDirectoryParameter_h

#include <QtGui>
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetDirectoryParameter
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetDirectoryParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetDirectoryParameter(DirectoryParameter*, QtWidgetModel*);
  virtual ~QtWidgetDirectoryParameter();

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetDirectoryParameter(const QtWidgetDirectoryParameter&); //purposely not implemented
  void operator=(const QtWidgetDirectoryParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  DirectoryParameter::Pointer m_DirectoryParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
};


}
}

#endif
