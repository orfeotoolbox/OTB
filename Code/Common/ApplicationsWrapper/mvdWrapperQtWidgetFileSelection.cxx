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
#include "mvdWrapperQtWidgetFileSelection.h"

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
QtWidgetFileSelection::QtWidgetFileSelection()
  : QWidget()
{
  this->DoCreateWidget();

  //
  // set accept drops in the widget
  setAcceptDrops( true );  
}

/*******************************************************************************/
QtWidgetFileSelection::~QtWidgetFileSelection()
{
}

/*******************************************************************************/
void QtWidgetFileSelection::DoUpdateGUI()
{

}

/*******************************************************************************/
void QtWidgetFileSelection::DoCreateWidget()
{
  unsigned int sp = 2;
  // Set up input text edit
  m_HLayout = new QHBoxLayout;
  m_HLayout->setSpacing(sp);
  m_HLayout->setContentsMargins(sp, sp, sp, sp);

  m_Checkbox = new QCheckBox();
  m_HLayout->addWidget(m_Checkbox);

  m_Input = new QLabel;
  m_Input->setFrameShape(QFrame::Box);
  m_HLayout->addWidget(m_Input);

  // Set up input text edit
  m_Button = new QPushButton;
  m_Button->setText("...");
  m_Button->setToolTip("Select file...");
  m_Button->setFixedWidth(30);
 
  connect( m_Button, SIGNAL(clicked()), this, SLOT(SelectFile()) );
  m_HLayout->addWidget(m_Button);

  this->setLayout(m_HLayout);
}

/*******************************************************************************/
void
QtWidgetFileSelection::dragEnterEvent( QDragEnterEvent * event )
{
  // 
  // accepts all the proposed actions : checking will be done
  // dragMoveEvent method
  event->acceptProposedAction();
}

/*******************************************************************************/
void 
QtWidgetFileSelection::dragMoveEvent(QDragMoveEvent *event)
 {
   // if (event->mimeData()->hasFormat("text/plain")
   
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
void 
QtWidgetFileSelection::dropEvent(QDropEvent *event)
{
  //
  // TODO : need to define the mimeData format
  // TODO : the data to drop will be defined in the DataSet TreeWidget
  
  //
  // get the text form the mimeData stored in the event : path
  // to the image in the dataset
  if ( event->mimeData()->hasText () )
    {
    //
    // text stored in mimeData represents the filename to use
    // extract valid filename by removing %20 and file:// form the
    // string if any
    std::string ofname = ExtractValidFilename( ToStdString(event->mimeData()->text()) );
    
    //
    // set the filename
    m_Input->setText( QString( ofname.c_str() ) );

    // since QLabel does not emit signal when textChanged, raise a signal
    // in the code
    emit textChanged( QString( ofname.data() ) );
    }
}

/*******************************************************************************/
std::string 
QtWidgetFileSelection::ExtractValidFilename(std::string payload)
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
void QtWidgetFileSelection::SelectFile()
{
  QFileDialog fileDialog;
  fileDialog.setConfirmOverwrite(true);
  fileDialog.setFileMode(QFileDialog::ExistingFile);
  fileDialog.setNameFilter("Raster files (*)");

  if (fileDialog.exec())
    {
    QString filemane( fileDialog.selectedFiles().at(0) );
    m_Input->setText(filemane);

    // since QLabel does not emit signal when textChanged, raise a signal
    // in the code
    emit textChanged( fileDialog.selectedFiles().at(0) );
    }
}

}
}
