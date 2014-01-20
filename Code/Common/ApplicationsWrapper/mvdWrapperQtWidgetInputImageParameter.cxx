/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "mvdWrapperQtWidgetInputImageParameter.h"

/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"

namespace mvd
{
namespace Wrapper
{

/*
  TRANSLATOR mvd::ApplicationLauncher

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*****************************************************************************/
/* CONSTANTS                                                                 */


/*****************************************************************************/
/* STATIC IMPLEMENTATION SECTION                                             */


/*****************************************************************************/
/* CLASS IMPLEMENTATION SECTION                                              */

/*******************************************************************************/
QtWidgetInputImageParameter
::QtWidgetInputImageParameter( otb::Wrapper::InputImageParameter* param,
			       otb::Wrapper::QtWidgetModel* m ) :
  otb::Wrapper::QtWidgetParameterBase(param, m),
  m_InputImageParam(param),
  m_HLayout( NULL ),
  m_Input( NULL ),
  m_Button( NULL )
{
  // 
  //
  m_Button = new QPushButton( this );
  m_HLayout = new QHBoxLayout();

#if 0
  m_Input = new DropLineEdit( this );
#else
  m_Input = new QLineEdit( this );
#endif

  QObject::connect(
    m_Input, SIGNAL( textChanged( const QString& ) ),
    this, SIGNAL( textChanged( const QString& ) )
  );

#if 0
  //
  // set accept drops in the widget
  setAcceptDrops( true );
#endif
}

/*******************************************************************************/
QtWidgetInputImageParameter::~QtWidgetInputImageParameter()
{
}

/*******************************************************************************/
void QtWidgetInputImageParameter::DoUpdateGUI()
{
}

/*******************************************************************************/
void QtWidgetInputImageParameter::DoCreateWidget()
{
  // Set up input QLabel
  m_Input->setToolTip( m_InputImageParam->GetDescription() );
  // m_Input->setFrameShape(QFrame::Box);
  m_Input->setText(tr("Drag and drop a dataset here."));

  // TODO : QLabel does not have signals, remove connections
  QObject::connect(
    this, SIGNAL( textChanged( const QString& ) ),
    this, SLOT( SetFileName( const QString& ) )
  );
  QObject::connect(
    this, SIGNAL( textChanged( const QString& ) ),
    GetModel(), SLOT( NotifyUpdate() )
  );

  // layout the QLabel
  m_HLayout->setSpacing(0);
  m_HLayout->setContentsMargins(0, 0, 0, 0);
  m_HLayout->addWidget(m_Input);

  // Set up browse button
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setMaximumWidth(m_Button->width());
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

#if 0
/*******************************************************************************/
void
QtWidgetInputImageParameter::dragEnterEvent( QDragEnterEvent * event )
{
  // 
  // accepts all the proposed actions : checking will be done
  // dragMoveEvent method
  event->acceptProposedAction();
}

/*******************************************************************************/
void 
QtWidgetInputImageParameter::dragMoveEvent(QDragMoveEvent *event)
 {
   //
   // if the mouse is within the QLabel geometry : allow drops
   if ( event->answerRect().intersects( m_Input->geometry() ) )
     {
     event->acceptProposedAction();
     }
   else
    {
    event->ignore();
    }
 }

/*******************************************************************************/
void QtWidgetInputImageParameter::dropEvent(QDropEvent *event)
{
  //
  // get the text form the mimeData stored in the event : path
  // to the image in the dataset
  if ( event->mimeData()->hasText () ) // files from disk
    {    
    //
    // text stored in mimeData represents the filename to use
    // extract valid filename by removing %20 and file:// form the
    // string if any
    std::string ofname = ExtractValidFilename( ToStdString(event->mimeData()->text()) );
    
    //
    // set the filename
    m_Input->setText( QString( ofname.data() ) );

    // since QLabel does not emit signal when textChanged, raise a signal
    // in the code
    emit textChanged( QString( ofname.data() ) );
    }
  else if ( event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist") )
    {
    //
    // TODO : use  simpler mime type
    QString ofname(
      event->mimeData()->data("application/x-qabstractitemmodeldatalist").constData()
      );

    //
    // set the filename
    m_Input->setText( ofname);

    // since QLabel does not emit signal when textChanged, raise a signal
    // in the code
    emit textChanged( ofname );
    }
}
#endif

/*******************************************************************************/
std::string 
QtWidgetInputImageParameter::ExtractValidFilename(std::string payload)
{
  // Parse the payload stream to extract all the filename
  std::stringstream s (payload);
  std::string token;
  std::string ofname;

  while ( s >> token )
    {
    // filename may contain whitespace coded as %20. Detect
    // it and replace the 3 characters by a whitespace to be able to
    // open the file.
    while( size_t pos = token.find("%") != std::string::npos )
      {
      token.replace( pos, 3," ");
      }

#ifdef WIN32
    // In Win32 the files name are stored with no file://
    ofname = token;
#else
    // Remove the "file://" form the string : subst(7, end)
    ofname = token.substr(7, token.size() );
#endif
    }

  return ofname;
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/

bool QtWidgetInputImageParameter::SetFileName(const QString& value)
{
  bool res = true;
  // save value
  if(m_InputImageParam->SetFromFileName(static_cast<const char*>(value.toAscii())) == true )
    {
    // notify of value change
    QString key( m_InputImageParam->GetKey() );

    // TODO
    emit ParameterChanged(key);
    }
  else
    res = false;

  return res;
}

/*******************************************************************************/
void QtWidgetInputImageParameter::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    if ( this->SetFileName(fileDialog.selectedFiles().at(0)) == true )
      {
      m_Input->setText(fileDialog.selectedFiles().at(0));
      // since QLabel does not emit signal when textChanged, raise a signal
      // in the code
      emit textChanged(fileDialog.selectedFiles().at(0));
      }    
    else
      {
      std::ostringstream oss;
      oss << "The given file "
          << static_cast<const char*>(fileDialog.selectedFiles().at(0).toAscii())
          << " is not valid.";
      this->GetModel()->SendLogWARNING( oss.str() );
      }
    }
}

}
}
