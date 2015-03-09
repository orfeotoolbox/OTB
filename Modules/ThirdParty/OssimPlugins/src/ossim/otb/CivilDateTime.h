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


#ifndef CivilDateTime_h
#define CivilDateTime_h

#include <ossimPluginConstants.h>
#include <iosfwd>

namespace ossimplugins
{


class JulianDate;
class JSDDateTime;
class GMSTDateTime;

/**
 * @brief This class represents a date and time in the civil format
 */
class OSSIM_PLUGINS_DLL CivilDateTime
{
public:

   /**
    * @brief Constructor
    */
   CivilDateTime();

   /**
    * @brief Constructor with date and time initialisation
    */
   CivilDateTime(int year, int month, int day, int second, double decimal);

   /**
    * @brief Constructor with date and time initialisation using an utc string
    */
   CivilDateTime(char* Utc);

   CivilDateTime(const char* Utc);

   /**
    * @brief Destructor
    */
   ~CivilDateTime();

   /**
    * @brief Copy constructor
    */
   CivilDateTime(const CivilDateTime& rhs);

   /**
    * @brief Affectation operator
    */
   CivilDateTime& operator=(const CivilDateTime& rhs);

   /**
    * @brief This function converts the current CivilDateTime into Julian Date
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param julianDate [out] The classe that receives the result of the conversion of the current CivilDateTime into JulianDate
    */
   int AsJulianDate(JulianDate* julianDate);

   /**
    * @brief This function converts the current CivilDateTime into JSDDateTime
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param JSDdate [out] The classe that receives the result of the conversion of the current CivilDateTime into JSDDateTime
    */
   int AsJSDDateTime(JSDDateTime* JSDdate);

   /**
    * @brief This function converts the current CivilDateTime into GMSTDateTime
    * @return 0 if no error or an int greater than 0 indicating the error
    * @param GMSTdate [out] The classe that receives the result of the conversion of the current CivilDateTime into GMSTDateTime
    */
   int AsGMSTDateTime(GMSTDateTime* GMSTdate);

   int get_year()
   {
      return _year;
   };

   int get_month()
   {
      return _month;
   };

   int get_day()
   {
      return _day;
   };

   int get_second()
   {
      return _second;
   };

   double get_decimal()
   {
      return _decimal;
   };

   void set_year(int year)
   {
      _year = year;
   };

   void set_month(int month)
   {
      _month = month;
   };

   void set_day(int day)
   {
      _day = day;
   };

   void set_second(int second)
   {
      _second = second;
   };

   void set_decimal(double decimal)
   {
      _decimal = decimal;
   };

   /**
    * @brief This function initialize the date and the the time of the class using an utc string
    * @param Utc Utc string containing the date and time
    * @return 0 if no error or an int gretter than 0 indiquating the error
    */
   int SetUtcDateTime(char* Utc);

   int SetUtcDateTime(const char* Utc);


   operator JulianDate() ;

   operator JSDDateTime() ;

   operator GMSTDateTime() ;

   std::ostream& print(std::ostream& out) const;

   friend std::ostream& operator<<(std::ostream& out,
                                   const CivilDateTime& obj);


   static const double JOURCIVIL_LENGTH;
protected:
   /**
    * @brief
    */
   int _year;
   /**
    * @brief
    */
   int _month;
   /**
    * @brief
    */
   int _day;
   /**
    * @brief Number of secondes since the beginning of the day
    */
   int _second;
   /**
    * @brief
    */
   double _decimal;


private:
};
}

#endif
