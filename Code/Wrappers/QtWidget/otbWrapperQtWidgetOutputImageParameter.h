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
#ifndef __otbWrapperQtWidgetOutputImageParameter_h
#define __otbWrapperQtWidgetOutputImageParameter_h

#include <QtGui>
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetOutputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputImageParameter(OutputImageParameter*, QtWidgetModel*);
  virtual ~QtWidgetOutputImageParameter();

  /** Get the PixelType*/
  //itkGetMacro(PixelType, int);

public slots:
  void SetFileName( const QString& value );

protected slots:
  void SelectFile();
  void SetPixelType(int pixelType);

private:
  QtWidgetOutputImageParameter(const QtWidgetOutputImageParameter&); //purposely not implemented
  void operator=(const QtWidgetOutputImageParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  std::string m_FileName;
  OutputImageParameter::Pointer m_OutputImageParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
  QComboBox*    m_ComboBox;
  int           m_PixelType;

};


}
}

#endif
