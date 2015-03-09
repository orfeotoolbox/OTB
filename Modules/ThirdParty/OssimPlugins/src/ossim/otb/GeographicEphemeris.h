//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef GeographicEphemeride_h
#define GeographicEphemeride_h

#include <ossimPluginConstants.h>
#include <otb/Ephemeris.h>

namespace ossimplugins
{


class GalileanEphemeris;
/**
 * @ingroup Ephemeris
 * @brief This class represent an ephemeris in Geographic coordinates system
 */
class OSSIM_PLUGINS_DLL GeographicEphemeris : public Ephemeris
{
public:
  /**
   * @brief Constructor
   */
  GeographicEphemeris();
  /**
    * @brief Destructor
    */
  virtual ~GeographicEphemeris();

  /**
   * @brief Constructor with initialisations
   */
  GeographicEphemeris(JSDDateTime date, double pos[3], double speed[3]);
  /**
   * @brief Copy constructor
   */
  GeographicEphemeris(const GeographicEphemeris& rhs);

  GeographicEphemeris(GalileanEphemeris& rhs);
  /**
   * @brief Affectation operator
   */
  GeographicEphemeris& operator=(const GeographicEphemeris& rhs);

  /**
   * @brief This fonction converts the current ephemeris in the Galilean coordinates system
   */
  void ToGalilean(GalileanEphemeris* vGal);

  operator GalileanEphemeris();

  /**
   * @brief This function creates a copy of the current class
   */
  virtual Ephemeris* Clone()
  {
    return new GeographicEphemeris(*this);
  };
protected:
private:

};
}

#endif
