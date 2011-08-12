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
#ifndef __otbWrapperEventsListener_h
#define __otbWrapperEventsListener_h

namespace otb
{
namespace Wrapper
{

/**
 * \class WrapperEventsListener
 *
 *  Vectorization view interface specification for the model
 *
 *  \ingroup Visualization
 */

template <class TEvent> class EventsListener
{
public:
  /** Standard class typedefs */
  typedef EventsListener Self;

  // Update the display
  virtual void Notify(const TEvent& event) = 0;

protected:
  /** Constructor */
  EventsListener() {}
  /** Destructor */
  virtual ~EventsListener() {}

private:
  EventsListener(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

}
} // end namespace otb
#endif
