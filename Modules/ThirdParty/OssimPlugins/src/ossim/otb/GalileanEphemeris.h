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

#ifndef GalileanEphemeride_h
#define GalileanEphemeride_h

#include <ossimPluginConstants.h>
#include <otb/Ephemeris.h>

namespace ossimplugins
{


class GeographicEphemeris;
/**
 * @ingroup Ephemeris
 * @brief This class represents an ephemeris in Galilean coordinates system
 */
class OSSIM_PLUGINS_DLL GalileanEphemeris : public Ephemeris
{
public:
  /**
   * @brief Constructor
   */
  GalileanEphemeris();

  /**
    * @brief Destructor
    */
  ~GalileanEphemeris();

  /**
   * @brief Constructor with initialisations
   */
  GalileanEphemeris(JSDDateTime date, double pos[3], double speed[3]);

  /**
   * @brief Copy constructor
   */
  GalileanEphemeris(const GalileanEphemeris& rhs);

  GalileanEphemeris(GeographicEphemeris& rhs);
  /**
   * @brief Affectation operator
   */
  GalileanEphemeris& operator=(const GalileanEphemeris& rhs);

  /**
   * @brief This fonction converts the current ephemeris in the Geographic coordinates syst�m
   */
  void ToGeographic(GeographicEphemeris* vGeo);
  void ToGeographic(double greenwich,GeographicEphemeris* vGeo);

  operator GeographicEphemeris();

  /**
   * @brief This function creates a copy of the current class
   */
  virtual Ephemeris* Clone()
  {
    return new GalileanEphemeris(*this);
  };
protected:
  int p2nutt(int newcmb, double greenwich, double day, double p[], double pd[] );
private:
};
}

#endif
