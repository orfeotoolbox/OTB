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
