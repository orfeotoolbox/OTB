//*******************************************************************
//
// License:  See top LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for Degrees Minutes Seconds (ossimDms)
//*******************************************************************
//  $Id: ossimDms.h 19682 2011-05-31 14:21:20Z dburken $
#ifndef ossimDms_HEADER
#define ossimDms_HEADER
// #include <iostream>

#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <ossim/base/ossimString.h>

class OSSIM_DLL ossimDms
{
public:
   // friend std::ostream& operator << (std::ostream& out, const ossimDms &dms);

   /**
    * Default constructor.  Initializes to 0.0 degrees latitude.
    */
   ossimDms();

   /**
    *
    * Constructor ossimDms(double, bool)
    *
    * You can construct a DMS class with a decimal
    * degree and specify if its for the lat or lon.
    *
    * latFlag          true   its for the lat
    *                  false  its for the lon
    *
    */
   ossimDms(double someDegrees, bool latFlag = true);

   /**
    *` Constructor.  You can specify the value in a formated
    *  string.  Example:
    *  "45N"     will set the decimal degree to 45 and will
    *            set the lat flag to true
    *
    *  "45W"     will set the internal degree to negative and
    *            set the lat flag to false.
    */
   ossimDms(const std::string& value);

   /**
    *  You can specify a number of different formats.
    *  special characters:
    *
    *      @    is replaced with a degree character.
    *      '    minutes character
    *      "    seconds character
    *      d    replaced with degree portion
    *      m    replaced with minute portion
    *      s    replaced with second portion
    *      c or C replaces with N/S or E/W
    *
    *  examples for format string
    *
    *
    */
   ossimString toString(const ossimString& formatString=ossimString(""))const;

   double getDegrees() const { return theDegrees; }
   bool   getLatFlag() const { return theLatFlag; }

   /**
    * setDegrees(char*).  Will allow you to set the internal
    * paramters through a string.  examples:
    *
    * "45 5 6"  will assume degrees minutes seconds
    * "45"      will assume degrees.
    * "-45"
    * "45W"
    *
    * @return true on success, false if unhandled characters are found.
    */
   bool setDegrees(const std::string& value);
   
   ossimDms& setDegrees(double degrees);

   ossimDms& setLatFlag(bool latFlag){theLatFlag = latFlag;return *this;}
   static const ossim_uint8 theDegreeSign;
   
private:
   static const char* DEFAULT_FORMAT;
   static const char* SPACES;
   
   double theDegrees;
   bool   theLatFlag;
   
   // This is a port of the dts written by Christine Hammond
   // into the OSSIM dms class.  We will use this to aid in
   // formatting DMS data.
   mutable double theDecDegs;/* decimal part of degrees */
   mutable bool   theAfterDot;/* decimal values flag */
   mutable bool   theDoingSeconds;
   mutable int    theIntDegs;/* integer part of double degrees */
   mutable int    theSign;/* what sign is degrees */
   mutable double theWorking;/* degrees value to use during calc */

   /**
    *  char * function degree_to_string
    *					
    *	requires:	dts.h		
    *	input:		double precision, string, integer params
    *	returns:	pointer to a string
    *									
    *	purpose:  to convert a double precision number  "degrees"
    *	to a character string based on the input "format" and a
    *	flag indicating whether or not the degrees are latitude. 
    *	If 'lat_flag' is TRUE, then degrees are latitude, otherwise
    *	they are longitude.	default format: ddd mm.mmC 	
    *									
    *	returns:  a pointer to a string which contains the formatted
    *	degree value.  							
    *									
    * written as part of the DIAL facility, G&G, WHOI 
    * April 1992   	
    * Christine L. Hammond						       
    ************************************************************************/

   ossimString degree_to_string(double degrees,
                                char format[],
                                bool lat_flag)const;
   
   void calc_mins_or_secs(double *dd,
                          std::string::const_iterator& formatIter,
                          std::string& result)const;
   /**
    * integer function calc_mins_or_secs				*
    *	calculates minutes from degrees, seconds from minutes,	
    *	based on "format", returning the resultant string	
    *	in "res".  "dd" is the part of 'degrees' that provides	
    *	the value to manipulate.  the integer returned as the 	
    *	value of this function is the number of places in the 	
    *	format that have been parsed here, to be used in the	
    *	main function to advance the format-parsing pointer. 
    */

   int calc_mins_or_secs(double *dd,
                         const char *format,
                         char *res)const;
   
   /**
    * function setup_printf
    * 	the C library function sprintf takes as its second
    * 	argument, a format specifier.  this function constructs
    *	that specifier based on 'ival', an integer indicating
    *	the number of d's or m's or s's seen in the group
    *	just parsed.
    */
   void setup_printf(int ival, char *fmt)const;

   /**
    * function set_default						*
    *	assigns a default format to be used as the template for	
    *	the conversion of the double precision value, degrees.	
    *	either an error was detected in parsing the format that	
    *	the user entered, or no format was entered at all.	
    *	also, clears the result string of any characters which	
    *	may have been placed therein prior to the call here.	
    */
   void set_default(char *fp, char *rp)const;

   /**
    * function init_values
    *	initializes the global values used throughout the
    *	function.  see notes in header file.
    */
   void init_values(double d)const;

   /**
    * double function string_to_degree
    *
    *	requires:	dts.h
    *	input:		string variable	parameter
    *	returns:	double precision number
    *									
    *	purpose: to calculate a double precision number, representing
    *	degrees latitude or longitude, given a string "cdegrees".
    *	valid input consists of the following characters:
    * 
    *		+, -, N, n, S, s, E, e, W, w -- direction indicators	
    *		any digit [0-9]						
    *		a decimal point, '.' -- says decimal degrees follow
    *		an ascii delimiter, for example ' ', TAB, '/' to 
    *		   indicate that either minutes or seconds follow.
    * 
    *	output is a double precision number, which will be either
    *	latitude or longitude degrees and decimal degree
    *
    * written as part of the DIAL facility, G&G, WHOI 
    * April 1992 
    * Christine L. Hammond 
    */

   
   double string_to_degree(const std::string& cdegrees);

};

#endif
