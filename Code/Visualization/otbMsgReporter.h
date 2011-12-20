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
#ifndef __otbMsgReporter_h
#define __otbMsgReporter_h

#include "itkObjectFactory.h"
#include <string>
#include "itkObject.h"
#include "itkMacro.h"
#include "otbMacro.h"
#include "otbMsgReporterGUI.h"

namespace otb
{
/** \class MsgReporter
 * \brief This class implements an message reporter windows. It's used by alls graphicals applications wants to report message
 *
 * Use FLTK gui.
 *
 */
class MsgReporter
      : public MsgReporterGUI , public itk::Object
{
public:
  /** Standard class typedefs */
  typedef MsgReporter    Self;
  typedef itk::Object                 Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MsgReporter, Object);

  /** Get the unique instance of the model */
  static Pointer GetInstance();

  /** Show the viewer (Update) */
  virtual void Show();
  /** Hide the window */
  virtual void Hide();

  /** Set the title of the window */
  virtual void SetTitle(const std::string & title);
  /** Send a new message in the edit area */
  virtual void SendMsg(const std::string & msg);
  /** Send an warning message in the edit area */
  virtual void SendWarning(const std::string& msg);
  /** Send an error message in the edit area */
  virtual void SendError(const std::string & msg);

protected:
  // Constructor and destructor
  MsgReporter();
  virtual ~MsgReporter() {}

private:
  MsgReporter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The instance singleton */
  static Pointer m_Instance;

};

} // end namespace otb

#endif
