/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkQtProgressBar.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkProcessObject.h"
#include "itkQtProgressBar.h"


namespace itk {


/** Constructor */
QtProgressBar::QtProgressBar( QWidget *parent):QProgressBar(parent)
{
  m_RedrawCommand = RedrawCommandType::New();
  m_RedrawCommand->SetCallbackFunction( this, &QtProgressBar::ProcessEvent );
  m_RedrawCommand->SetCallbackFunction( this, &QtProgressBar::ConstProcessEvent );

  m_Caller = itk::Object::New();
  this->setMaximum( 100 );
  this->reset();
}


QtProgressBar::~QtProgressBar()
{
  m_Caller->RemoveAllObservers();
}

/** Get Command */
QtProgressBar::RedrawCommandType *
QtProgressBar::GetRedrawCommand( void ) const
{
  return m_RedrawCommand.GetPointer();
}

/** Manage a Progress event */
void
QtProgressBar::ProcessEvent( itk::Object * caller,
                             const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent ) == typeid( event ) )
    {
    ::itk::ProcessObject::Pointer  process =
      dynamic_cast< itk::ProcessObject *>( caller );

    const int value = static_cast<int>(
      process->GetProgress() * this->maximum() );

    emit SetValueChanged( value );
    }
}

void
QtProgressBar::ConstProcessEvent( const itk::Object * caller,
                                  const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent ) == typeid( event ) )
    {
    itk::ProcessObject::ConstPointer  process =
      dynamic_cast< const itk::ProcessObject *>( caller );

    const int value = static_cast<int>(
      process->GetProgress() * this->maximum() );

    emit SetValueChanged( value );
    }
}

/** Manage a Progress event */
void
QtProgressBar::Observe( itk::Object *caller )
{
  m_Caller = caller;
  m_Caller->AddObserver(  itk::ProgressEvent(), m_RedrawCommand.GetPointer() );
}

} // end namespace fltk
