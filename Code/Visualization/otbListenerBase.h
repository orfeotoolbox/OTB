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
#ifndef __otbListenerBase_h
#define __otbListenerBase_h

namespace otb
{
/**
 * \class ListenerBase
 *
 *  Vectorization view interface specification for the model
 *
 *  \ingroup Visualization
 */

class ListenerBase
{
public:
  /** Standard class typedefs */
  typedef ListenerBase Self;

  // Update the display
  virtual void Notify() = 0;

protected:
  /** Constructor */
  ListenerBase() {};
  /** Destructor */
  virtual ~ListenerBase() {};

private:
  ListenerBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace otb
#endif
