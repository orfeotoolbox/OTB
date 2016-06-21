/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkQtProgressBar.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkQtProgressBar_h
#define __itkQtProgressBar_h

#include <QtGui>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829  //tag=QT4-boost-compatibility
#include "itkCommand.h"
#endif //tag=QT4-boost-compatibility

namespace itk {

class ITK_ABI_EXPORT QtProgressBar : public ::QProgressBar
{
  Q_OBJECT
public:

  /** Command Class invoked for button redraw */
  typedef itk::MemberCommand< QtProgressBar >  RedrawCommandType;

  /** Constructor */
  QtProgressBar( QWidget *parent );

  /** Destructor */
  ~QtProgressBar() ITK_OVERRIDE;

  /** Get Command */
  RedrawCommandType * GetRedrawCommand( void ) const;


  /** Manage a Progress event */
  void ProcessEvent(itk::Object * caller, const itk::EventObject & event );
  void ConstProcessEvent(const itk::Object * caller, const itk::EventObject & event );


  /** Manage a Progress event */
  void Observe( itk::Object *caller );

signals:
  void SetValueChanged(int);

private:
  itk::Object::Pointer       m_Caller;
  RedrawCommandType::Pointer m_RedrawCommand;
};


}  // end of namespace

#endif
