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
#ifndef __otbWrapperAddProcessToWatchEvent_h
#define __otbWrapperAddProcessToWatchEvent_h

#include "itkEventObject.h"
#include "itkProcessObject.h"

namespace otb
{
namespace Wrapper
{

/** \class AddProcessToWatchEvent
 *  \brief This class implements an event storing a pointer to
 *  itk::ProcessObject and a string describing the process.
 *
 */
class ITK_ABI_EXPORT AddProcessToWatchEvent: public itk::EventObject
{
public:

  typedef AddProcessToWatchEvent Self;
  typedef itk::EventObject       Superclass;

  AddProcessToWatchEvent(){}
  AddProcessToWatchEvent(const Self& s) :itk::EventObject(s){};
  virtual ~AddProcessToWatchEvent() {}

  /** Set/Get the process to watch */
  virtual void SetProcess(itk::ProcessObject * process)
  {
    m_Process = process;
  }
  virtual itk::ProcessObject * GetProcess() const
  {
    return m_Process;
  }

  /** Set/Get the process description */
  virtual void SetProcessDescription(const std::string desc)
  {
    m_ProcessDescription = desc;
  }

  virtual  std::string GetProcessDescription() const
  {
    return m_ProcessDescription;
  }

  /** Virtual pure method to implement */
  virtual itk::EventObject* MakeObject() const
  {
    return new Self;
  }

  virtual const char* GetEventName() const
  {
    return "AddProcess";
  }
  virtual bool CheckEvent(const itk::EventObject* e) const
  {
    return dynamic_cast<const Self*>(e);
  }

private:
  itk::ProcessObject::Pointer m_Process;
  std::string                 m_ProcessDescription;
};

}
}

#endif
