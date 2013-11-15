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
#ifndef __otbWrapperQtWidgetOutputVectorDataParameter_h
#define __otbWrapperQtWidgetOutputVectorDataParameter_h

#include <QtGui>
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtWidgetOutputVectorDataParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputVectorDataParameter(OutputVectorDataParameter*, QtWidgetModel*);
  virtual ~QtWidgetOutputVectorDataParameter();

  /** Get the PixelType*/
  //itkGetMacro(PixelType, int);

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();

private:
  QtWidgetOutputVectorDataParameter(const QtWidgetOutputVectorDataParameter&); //purposely not implemented
  void operator=(const QtWidgetOutputVectorDataParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  std::string m_FileName;
  OutputVectorDataParameter::Pointer m_OutputVectorDataParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
  QComboBox*    m_ComboBox;
  int           m_PixelType;

};


}
}

#endif
