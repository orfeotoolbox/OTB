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

#ifndef JulianData_h
#define JulianData_h

#include <ossimPluginConstants.h>

namespace ossimplugins
{


class CivilDateTime;
class GMSTDateTime;
class JSDDateTime;
/**
 * @brief This class represents a date in the julian referential
 */
class OSSIM_PLUGINS_DLL JulianDate
{
public:
  /**
   * @brief constructor
   */
  JulianDate();
  /**
   * @brief Destructor
   */
  ~JulianDate();
  /**
   * @brief Copy constructor
   */
  JulianDate(const JulianDate& rhs);

  /**
   * @brief Constructor with date initialisation using a CivilDateTime
   */
  JulianDate(CivilDateTime& rhs);

  /**
   * @brief Constructor with date initialisation using a JSDDateTime
   */
  JulianDate(JSDDateTime& rhs);
  /**
   * @brief Constructor with date initialisation
   */
  JulianDate(double julianDate);
  /**
   * @brief Affectation operator
   */
  JulianDate& operator=(const JulianDate& rhs);
  /**
   * @brief Summation operator
   */
  JulianDate& operator+(const JulianDate& rhs);

  void set_julianDate(double julianDate)
  {
    _julianDate = julianDate;
  }

  double get_julianDate() const
  {
    return _julianDate;
  }

  /**
   * @brief This function converts the current JulianDate in JSDDateTime
   * @return 0 if no error or an int greater than 0 indicating the error
   * @param JSDdate [out] The classe that receives the result of the conversion of the current JulianDate in JSDDateTime
   */
  int AsJSDDateTime(JSDDateTime* JSDdate);

  /**
   * @brief This function converts the current JulianDate into GMSTDateTime
   * @return 0 if no error or an int greater than 0 indicating the error
   * @param GMSTDateTime [out] The class that receives the result of the conversion of the current JulianDate into GMSTDateTime
   */
  int AsGMSTDateTime(GMSTDateTime* GMST);

  operator JSDDateTime() ;
  operator GMSTDateTime() ;


protected:

  double _julianDate;

private:
};
}

#endif

