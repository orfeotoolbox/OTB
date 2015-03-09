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
#ifndef __otbOGRDriversInit_h
#define __otbOGRDriversInit_h

#include <boost/noncopyable.hpp>

namespace otb { namespace ogr {
  /**\ingroup Geometry
   * Singleton-like class to provide lazy-registering of all \c OGRDriver's.
   *
   * Call
   * \code
   * otb::ogr::Drivers::Init();
   * \endcode
   * before using classes wrapping OGR.
   *
   * <p><b>Properties</b><br>
   * - Follows Meyer's Singleton design.
   * - Implicitly initialized with a default constructor
   * - MT-Safe in C++11, and some compilers like GCC (not on old releases of
   * VC++)
   * - Non-copyable
   */
  class Drivers : private boost::noncopyable
  {
  /** \name Singleton related functions */
  //@{
public:
  static Drivers & Init();
private:
  /** Constructor.
   * Calls \c OGRRegisterAll().
   */
  Drivers();
  /** Destructor.
   * Calls \c OGRCleanupAll().
   */
  ~Drivers();
  //@}
  };

} } // end namespace otb::ogr

#endif // __otbOGRDriversInit_h
