/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperQtWidgetOutputVectorDataParameter_h
#define otbWrapperQtWidgetOutputVectorDataParameter_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"
#endif //tag=QT4-boost-compatibility


namespace otb
{
namespace Wrapper
{

/** \class QtWidgetOutputVectorDataParameter
 * \brief
 *
 * \ingroup OTBQtWidget
 */
class OTBQtWidget_EXPORT QtWidgetOutputVectorDataParameter : public QtWidgetParameterBase
{
  Q_OBJECT
public:
  QtWidgetOutputVectorDataParameter(OutputVectorDataParameter*, QtWidgetModel*);
  ~QtWidgetOutputVectorDataParameter() ITK_OVERRIDE;

  inline const QLineEdit* GetInput() const;
  inline QLineEdit* GetInput();

  /** Get the PixelType*/
  //itkGetMacro(PixelType, int);

public slots:
  void SetFileName( const QString& value );

protected slots:
  void SelectFile();

private:
  QtWidgetOutputVectorDataParameter(const QtWidgetOutputVectorDataParameter&); //purposely not implemented
  void operator=(const QtWidgetOutputVectorDataParameter&); //purposely not implemented

  void DoCreateWidget() ITK_OVERRIDE;

  void DoUpdateGUI() ITK_OVERRIDE;

  std::string m_FileName;
  OutputVectorDataParameter::Pointer m_OutputVectorDataParam;

  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
  QComboBox*    m_ComboBox;
  int           m_PixelType;

};



inline
const QLineEdit*
QtWidgetOutputVectorDataParameter
::GetInput() const
{
  return m_Input;
}

inline
QLineEdit*
QtWidgetOutputVectorDataParameter
::GetInput()
{
  return m_Input;
}

}
}

#endif
