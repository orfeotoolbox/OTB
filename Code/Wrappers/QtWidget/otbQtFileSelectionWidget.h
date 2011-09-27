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
#ifndef __otbQtFileSelectionWidget_h
#define __otbQtFileSelectionWidget_h

#include <QtGui>
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperQtWidgetParameterBase.h"


namespace otb
{
namespace Wrapper
{

/** \class
 * \brief
 */
class QtFileSelectionWidget : public QWidget
{
  Q_OBJECT
public:
  QtFileSelectionWidget();
  QtFileSelectionWidget( InputImageListParameter * il );
  virtual ~QtFileSelectionWidget();

  InputImageListParameter * GetInputList()
  {
    return m_InputList;
  }

  void SetInputList(InputImageListParameter * il)
  {
    m_InputList = il;
  }

  unsigned int GetIndex()
  {
    return m_Index;
  }

  unsigned int SetIndex( unsigned int id )
  {
    m_Index = id;
  }


  bool IsChecked()
  {
    return m_Checkbox->isChecked();
  }

  void SetChecked( bool val )
  {
    return m_Checkbox->setChecked( val );
  }


protected slots:
  //void SetFileName( const QString& value );
  void SelectFile();

private:
  QtFileSelectionWidget(const QtFileSelectionWidget&); //purposely not implemented
  void operator=(const QtFileSelectionWidget&); //purposely not implemented

  virtual void DoCreateWidget();

  virtual void DoUpdateGUI();


  QHBoxLayout * m_HLayout;
  QLineEdit*    m_Input;
  QPushButton * m_Button;
  QCheckBox *   m_Checkbox;
  InputImageListParameter::Pointer m_InputList;
  unsigned int m_Index;
  bool m_AsValue;
};


}
}

#endif
