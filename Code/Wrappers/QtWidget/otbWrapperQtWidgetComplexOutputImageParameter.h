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
#ifndef __otbWrapperQtWidgetComplexOutputImageParameter_h
#define __otbWrapperQtWidgetComplexOutputImageParameter_h

#include <QtGui>
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class ITK_ABI_EXPORT QtWidgetComplexOutputImageParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetComplexOutputImageParameter(ComplexOutputImageParameter*, QtWidgetModel*);
  virtual ~QtWidgetComplexOutputImageParameter();

  /** Get the PixelType*/
  //itkGetMacro(PixelType, int);

protected slots:
  void SetFileName( const QString& value );
  void SelectFile();
  void SetPixelType(int pixelType);

private:
  QtWidgetComplexOutputImageParameter(const QtWidgetComplexOutputImageParameter&); //purposely not implemented
  void operator=(const QtWidgetComplexOutputImageParameter&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();

  std::string m_FileName;
  ComplexOutputImageParameter::Pointer m_OutputImageParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
  QComboBox*    m_ComboBox;
  int           m_ComplexPixelType;

};


}
}

#endif
