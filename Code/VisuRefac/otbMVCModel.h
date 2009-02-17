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
#ifndef __otbMVCModel_h
#define __otbMVCModel_h

#include <set>
#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class MVCModel
 *
 * Base class for MVC based modelling. Implements the registration and
 * Unregistration of listeners, as well as the NotifyAll method. A valid listener must
 * implement a Notify() method.
 */
template <class TListener>
class MVCModel
      : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef MVCModel    Self;
  typedef itk::Object Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(MVCModel,Object);
  itkNewMacro(Self);

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
  MVCModel() {}
  /** Destructor */
  ~MVCModel() {}

private:
  /** Registered liteners */
  ListenersSetType m_RegisteredListeners;
};
}
#endif
