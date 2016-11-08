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
#ifndef otbThreads_h
#define otbThreads_h

namespace otb
{

/** \class Threads
 * \brief Operation from open threads that are used in Monteverdi
 *
 * Implement the sleep operation from openthreads to remove Monteverdi
 * dependence to openthreads at compile time.
 *
 *
 * \ingroup OTBOpenThreadsAdapters
 */
class Threads
{
public:

  static int Sleep(unsigned int microsec);

};

} // namespace otb

#endif
