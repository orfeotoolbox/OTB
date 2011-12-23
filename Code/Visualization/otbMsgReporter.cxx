/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/

#include "otbMsgReporter.h"
#include <FL/Fl_Text_Buffer.H>

namespace otb
{
/** Initialize the singleton */
MsgReporter::Pointer MsgReporter::m_Instance = NULL;

/// Constructor
MsgReporter
::MsgReporter()
{
  this->Build();
  wMainWindow->show();
  Fl_Text_Buffer * buffer = new Fl_Text_Buffer();
  this->textArea->buffer(buffer);
}


/** Manage the singleton */
MsgReporter::Pointer
MsgReporter
::GetInstance()
{
  if (!m_Instance)
  {
    m_Instance = MsgReporter::New();
  }
  return m_Instance;
}
//Show
void
MsgReporter
::Show()
{
  this->wMainWindow->show();
}
//Hide
void
MsgReporter
::Hide()
{
  this->wMainWindow->hide();
}
//Set title
void
MsgReporter
::SetTitle(const std::string & title)
{
  std::string str(title);
  str = str + " message reporter window";
  this->wMainWindow->label(str.c_str());
}
//Send Msg
void
MsgReporter
::SendMsg(const std::string & msg)
{
  this->textArea->insert(msg.c_str());
  this->textArea->show_insert_position();
  this->Show();
  Fl::check();
}

//Send Error
void
MsgReporter
::SendWarning(const std::string & msg)
{
  this->textArea->insert("WARNING: ");
  this->textArea->insert(msg.c_str());
  this->textArea->show_insert_position();
  this->Show();
  Fl::check();
}

//Send Error
void
MsgReporter
::SendError(const std::string & msg)
{
  this->textArea->insert("ERROR: ");
  this->textArea->insert(msg.c_str());
  this->textArea->show_insert_position();
  this->Show();
  Fl::check();
}

}
