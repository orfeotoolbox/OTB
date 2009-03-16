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
#ifndef __otbMVCModelBase_h
#define __otbMVCModelBase_h

#include <set>

namespace otb
{
/** \class MVCModelBase
 *
 * Interface class for MVC based modelling. Implements the registration and
 * Unregistration of listeners, as well as the NotifyAll method. A valid listener must
 * implement a Notify() method.
 */
template <class TListener>
class MVCModelBase
{
public:
  // Define the listener type
  typedef TListener ListenerType;

  /** PreprocessingViewInterface storage */
  typedef std::set<ListenerType *> ListenersSetType;

  /** Register a new listener */
  virtual bool RegisterListener(ListenerType * listener)
  {
    return(m_RegisteredListeners.insert(listener).second);
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
  virtual void NotifyAll()
  {
    for (typename ListenersSetType::iterator it = m_RegisteredListeners.begin();
         it!=m_RegisteredListeners.end();
         ++it)
    {
      this->Notify(*it);
    }
  }

  /** Notify changes to a given listerner */
  virtual void Notify(ListenerType * listener) {};

protected:
  /** Constructor */
  MVCModelBase() {}
  /** Destructor */
  virtual ~MVCModelBase() {}

private:
  /** Registered liteners */
  ListenersSetType m_RegisteredListeners;
};
}
#endif
