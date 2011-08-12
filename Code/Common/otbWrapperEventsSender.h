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
#ifndef __otbWrapperEventsSender_h
#define __otbWrapperEventsSender_h

#include "itkObject.h"

#include "otbWrapperEventsListener.h"

#include <set>

namespace otb
{
namespace Wrapper
{

/** \class EventsSender
 *
 * Base class for events sending. Listener may register to this class
 * to receive the events.
 */
template <class TEvent = void>
class EventsSender
//   : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef EventsSender                  Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Define the event type
  typedef TEvent EventType;

  // Define the listener type
  typedef EventsListener<EventType> ListenerType;

  /** PreprocessingViewInterface storage */
  typedef std::set<ListenerType *> ListenersSetType;

  /** Register a new listener */
  virtual bool RegisterListener(ListenerType * listener)
  {
    return (m_RegisteredListeners.insert(listener).second);
  }

  /** Unregister a listener */
  virtual void UnRegisterListener(ListenerType * listener)
  {
    m_RegisteredListeners.erase(listener);
  }

  virtual void UnRegisterAllListeners()
  {
    m_RegisteredListeners.clear();
  }

  /** Notify changes to all registered listeners */
  virtual void NotifyAll(const EventType& event = NULL)
  {
    for (typename ListenersSetType::iterator it = m_RegisteredListeners.begin();
         it != m_RegisteredListeners.end();
         ++it)
      {
      this->NotifyListener(*it, event);
      }
  }

  /** Notify changes to a given listener */
  virtual void NotifyListener(ListenerType * listener, const EventType& event = NULL)
  {
    listener->Notify(event);
  }

protected:
  /** Constructor */
  EventsSender() {}
  /** Destructor */
  virtual ~EventsSender() {}

private:
  EventsSender(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Registered listeners */
  ListenersSetType m_RegisteredListeners;
};
}
}
#endif
