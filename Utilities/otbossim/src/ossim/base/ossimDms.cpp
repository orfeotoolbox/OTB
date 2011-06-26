//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description:
//
// Contains class definition for Degrees Minutes Seconds (ossimDms)
//*******************************************************************
//  $Id: ossimDms.cpp 19682 2011-05-31 14:21:20Z dburken $

#include <cctype> /* for isspace */
#include <cmath>
#include <cstring> /* for strcpy */
#include <cstdio>
#include <iomanip>
#include <sstream>

#include <ossim/base/ossimDms.h>
#include <ossim/base/ossimCommon.h>

const ossim_uint8 ossimDms::theDegreeSign = 176;
const char* ossimDms::DEFAULT_FORMAT = "ddd mm.mmC";
const char* ossimDms::SPACES = "                   ";

ossimDms::ossimDms()
   : theDegrees(0.0),
     theLatFlag(true),
     theDecDegs(0.0),
     theAfterDot(false),
     theDoingSeconds(true),
     theIntDegs(0),
     theSign(1),
     theWorking(0.0)
{
}

ossimDms::ossimDms(double someDegrees, bool latFlag)
   : theDegrees(someDegrees),
     theLatFlag(latFlag),
     theDecDegs(0.0),
     theAfterDot(false),
     theDoingSeconds(true),
     theIntDegs(0),
     theSign(1),
     theWorking(0.0)
{
}

ossimDms::ossimDms(const std::string& value)
   : theDegrees(0.0),
     theLatFlag(false),
     theDecDegs(0.0),
     theAfterDot(false),
     theDoingSeconds(true),
     theIntDegs(0),
     theSign(1),
     theWorking(0.0)
{
   if (value != "")
   {
      theDegrees = string_to_degree(value);
   }
   else
   {
      theDegrees = 0.0;
   }

}

ossimDms& ossimDms::setDegrees(double degrees)
{
   theDegrees = degrees;
   
   return *this;
}

bool ossimDms::setDegrees(const std::string& cdegrees)
{
   bool status = true;
   
   ossimString copy = cdegrees;
   
   double degrees, factor, dividend, units;
   int i;
   bool afterdelim;
   char* cptr = const_cast<char*>(copy.c_str());
   char c;

   theAfterDot = false;
   afterdelim = false;
   degrees = 0.0;
   units = 0.0;
   factor = 1.0;
   dividend = 60.0;
   theSign = 1;

   /* get rid of leading spaces */

   while (*cptr == ' ')
   {
      cptr++;
   }

   while(*cptr != '\0') 
   {
      switch (*cptr) 
      {

         // north, west and + values will change sign of degrees to plus.

         case '+':
         {
            theSign = 1;
            cptr++;
         }
         case 'n':
         case 'N':
         {
            theSign = 1;
            theLatFlag = true;
            cptr++;
            break;
         }
         case 'e':
         case 'E':
         {
            theLatFlag = false;
            theSign = 1;
            cptr++;
            break;
         }

         // south, east and - values will change sign of degrees to minus
         
         case '-':
         {
            theSign = -1;
            cptr++;
            break;
         }
         case 's':
         case 'S':
         {
            theSign = -1;
            cptr++;
            theLatFlag = true;
            break;
         }
         case 'w':
         case 'W':
         {
            theSign = -1;
            cptr++;
            theLatFlag = false;
            break;
         }
         
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9': 
         {
            c = *cptr;
            i = (c - '0');
            
            if (afterdelim == true) 
            {
               if (theAfterDot == true) 
               {
                  units = units + (((double)i * factor) / dividend);
               }
               else
               {
                  units = (units * factor) + ((double)i / dividend);
               }
            }
            else 
            {
               if (theAfterDot == true)
               {
                  degrees = degrees + ((double)i * factor);
               }
               else
               {
                  degrees = (degrees * factor) + (double)i;
               }
            }
            if (theAfterDot == true)
            {
               factor = factor * .10;
            }
            else
            {
               factor = 10.;
            }
            
            cptr++;
            break;
         }

         //---
         // a decimal point indicates a change in the factor used
         // to calculate degrees or units (minutes or seconds)
         //---
         case '.': 
         {
            factor = .10;
            theAfterDot = true;
            cptr++;
            break;
         }

         // after a delimiter the value contains minutes, first time through
         case ' ': 
         case '\"':
         case '\'':
         {
            while (*(cptr + 1) == ' ')
            {
               cptr++;
            }
            
            degrees = degrees + units;
            units = 0.0;
            
            if (afterdelim == true) 	/* must be seconds */
            {
               dividend = dividend * dividend;
            }
            else
            {
               afterdelim = true;
            }
            
            factor = 1.;
            
            cptr++;
            
            // skip any leading zeroes after delimiter */
            
            while (*cptr == '0')
            {
               cptr++;
            }
            
            break;
         }

         //---
         //  check for a delimiter that is allowable:
         // isspace allows: space, tab, cr, nl, vt, ff
         // ispunct allows: punctuation char != space or control or letter
         //---
         default: 
         {
            if(isspace(*cptr) || 
               ispunct(*cptr) || 
               ((ossim_uint8)(*cptr) == theDegreeSign))
            {
               *cptr = ' ';
            }
            else
            {
               status = false;
               ++cptr;
            }
         }
         
      }	// end switch

      if (status == false)
      {
         break;
      }
      
   } //  end while loop

   // add any units that may have been calculated (minutes or seconds)
   degrees     = degrees + units;
   
   theDegrees = (degrees * (double)theSign);
   
   return status;
}

ossimString ossimDms::toString(const ossimString& formatString)const
{
   ossimString result="";
   std::string::const_iterator stringIter;
   ossimString tempFormatString = formatString;
   if(formatString == ossimString(""))
   {
      tempFormatString = ossimDms::DEFAULT_FORMAT;
   }

   stringIter = tempFormatString.begin();
   int i, d_s;
   
/* assign a default format if none is given */
      
   init_values(theDegrees);
   
   while(stringIter != tempFormatString.end())
   {
      switch(*stringIter)
      {
         case '-':
         {
            result +=(theWorking < 0.0) ? ('-') : (' ');
            stringIter++;
            break;
         }
         
         case 'c':
         case 'C':
         {
            char temp;
            if (theWorking < 0.0)
            {
               temp = (theLatFlag == true) ? ('S') : ('W');
            }
            else
            {
               temp= (theLatFlag == true) ? ('N') : ('E');
            }
            
            if(*stringIter == 'c')
            {
               temp = tolower(temp);
            }
            result += temp;
            stringIter++;
            break;
         }
         
         case 'd':
         case 'D':
         {			/* how many 'd's until not 'd' */
            d_s = 1;
            ++stringIter;
            while (*stringIter == 'd' || *stringIter == 'D')
            {
               ++d_s;
               ++stringIter;
            }
            if (theAfterDot == true)
            {		/* beyond the decimal point */
               double fractionalDegrees = std::abs(theDegrees-(int)theDegrees);
               i = d_s;
               while (i > 0)
               {
                  fractionalDegrees = fractionalDegrees * 10.0;
                  result += ossimString::toString((int)fractionalDegrees);
                  fractionalDegrees = fractionalDegrees - (int)fractionalDegrees;
                  --i;
               }
               theIntDegs = (int)(theDecDegs);
            }
            else
            {
               theIntDegs = static_cast<int>( std::abs(theDegrees) );
               ossimString temp = ossimString::toString(theIntDegs);
               ossimString prefix;
               d_s -= (int)temp.length();
               while(d_s > 0)
               {
                  prefix += '0';
                  --d_s;
               }
               temp = prefix + temp;
               result += temp;
            }
//             // fill the rest with blanks
//             ossimString temp = ossimString::toString(theIntDegs);
//             d_s -= temp.length();
//             if(d_s>0)
//             {
//                if(theAfterDot)
//                {
                  
//                   while(d_s > 0)
//                   {
//                      temp+='0';
//                      --d_s;
//                   }
//                }
//                else
//                {
//                   ossimString prefix;
//                   while(d_s > 0)
//                   {
//                      prefix += '0';
//                      --d_s;
//                   }
//                   temp = prefix + temp;
//                }
//             }
            
            break;
         }
 
         case ' ':
         {			
            result += *stringIter;
            
            while (*stringIter == ' ')
            {
               result += *stringIter;
               stringIter++;
            }
            break;
         }
         
         case '.': 
         {
            theAfterDot = true;              
            result +=  *stringIter++;
            break;
         }
         
         case 'm':
         case 'M':
         case 's':
         case 'S':
         {
            if((*stringIter == 's') ||
               (*stringIter == 'S'))
            {
               theDoingSeconds = true;
            }
            
            calc_mins_or_secs(&theDecDegs, stringIter, result);
            break;
         }
         
         // This is code that I added so you can do additional
         // formatting.
         //
         case '\'':
         case '"':
         {
            result += *stringIter++;
            break;
         }
         case '@':
         {
            result += (char)theDegreeSign;
            stringIter++;
            break;
         }
         default:
         {
            if(theDoingSeconds)
            {
               theAfterDot = true;
            }
            result += *stringIter++;
         }
      }	/* end switch statement */
   }	/* end while loop */
   
   return result;   
}

// The functions that follow are all dts stuff

ossimString ossimDms::degree_to_string(double degrees,
                                       char format[],
                                       bool lat_flag)const
{
   char cdegrees[64];
   char str_fmt[10];
   char *rptr, *fptr, *sptr;
   int i, d_s;
   
/* assign a default format if none is given */
   
   if (format[0] == '\0')
   {
      set_default(format, cdegrees);
   }
   else
   {
      memset(cdegrees, ' ', 64);
//       strcpy(cdegrees, SPACES);
   }
   
   init_values(degrees);
   
   rptr = cdegrees;
   fptr = format;
   sptr = str_fmt;
   
/* cycle through characters of the format and plug in values */
   
   while (*fptr != '\0') {
      
	 switch (*fptr) {

	   case '-': {
		(theWorking < 0.0) ? (*rptr = '-') : (*rptr = ' ');
		rptr++, fptr++;
		break;
	   }

	   case 'c':
	   case 'C': {
		if (theWorking < 0.0)
		   (lat_flag == true) ? (*rptr = 'S') : (*rptr = 'W');
		else
		   (lat_flag == true) ? (*rptr = 'N') : (*rptr = 'E');
		
		rptr++, fptr++;
		break;
	   }

	   case 'd':
	   case 'D': {			/* how many 'd's until not 'd' */
	   	d_s = 1, fptr++;
	   	while (*fptr == 'd') {
		   d_s++, fptr++;
	   	}
		setup_printf(d_s, sptr);	/* printf's fmt will be %x.xd */

		if (theAfterDot == true) {		/* beyond the decimal point */
		   i = d_s;
		   while (i-- > 0)
			theDecDegs = theDecDegs * 10.0;
//		   theIntDegs = (int)(theDecDegs + 0.5);
		   theIntDegs = (int)(theDecDegs);
		 }

		sprintf(rptr, str_fmt, theIntDegs);

		if (*rptr == '0' && !theAfterDot)	/* remove leading zero */
		   *rptr = ' ';
	   	rptr += d_s;
		break;
	   }

	   case ' ': {			/* compress multiple spaces in format */
		*rptr = *fptr;
		while (*fptr == ' ')
			fptr++;
		rptr++;
		break;
	   }

	   case '.': {
		if (theAfterDot == true) {		/* a second decimal point? */
		   set_default(format, cdegrees);
		   fptr = format;
		   rptr = cdegrees;
		   init_values(degrees);
		}
		else {
		   *rptr = *fptr;
		   rptr++, fptr++;
		   theAfterDot = true;
		}
		break;
	   }

         case 'm':
         case 'M':
         case 's':
         case 'S':
         {
		i = calc_mins_or_secs(&theDecDegs, fptr, rptr);
		fptr += i, rptr += i;		/* i == num to advance format */
		break;
	   }

           // This is code that I added so you can do additional
           // formatting.
           //
         case '\'':
         case '"':
         {
            *rptr = *fptr;
            rptr++;
            fptr++;
            break;
         }
         case '@':
         {
            *rptr = (char)theDegreeSign;
            rptr++;
            fptr++;
            break;
         }
	   default: {
		set_default(format, cdegrees);
		fptr = format;
		rptr = cdegrees;
		init_values(degrees);
	   }
	 }	/* end switch statement */

	}	/* end while loop */

	*rptr = '\0';
	return(cdegrees);
}


void ossimDms::calc_mins_or_secs(double *dd,
                                 std::string::const_iterator& formatIter,
                                 std::string& result)const
{
	double du;
	int ufactor, ires, numunits;
	char unit;

	unit = *formatIter;			/* set to 'm' or 's' */

	ufactor = 1, numunits = 0;

/* count minutes (or seconds) desired in this part of format */

	while (*formatIter == unit)
        {
	   numunits++;
	   formatIter++, ufactor *= 10;
	}

/* different calc based on beyond decimal pt or not */

	if (theAfterDot == true)
        {
	   du = *dd * (double)ufactor;
//	   ires = (int)(du + 0.5);
	   ires = (int)(du);
	   *dd = (du - ires);
           if(*dd < 0.0) *dd = 0.0;
	}
	else
        {
	   du = (*dd * 60.0) * (double)ufactor;
//	   ires = (int)(du + 0.5) / ufactor;
	   ires = (int)(du) / ufactor;
	   *dd = (du - (ires * ufactor)) / (double)ufactor;
           if(*dd < 0.0) *dd = 0.0;
	}
        std::ostringstream out;
        
        out << std::setw(numunits)
            << std::setfill('0')
            << std::setiosflags(std::ios::right)
            << std::setprecision(numunits)
            << ires
            << std::ends;
        result+=out.str().c_str();
        

//        result += ossimString::toString(ires);
}

/****************************************************************
 * integer function calc_mins_or_secs				*
 *	calculates minutes from degrees, seconds from minutes,	*
 *	based on "format", returning the resultant string	*
 *	in "res".  "dd" is the part of 'degrees' that provides	*
 *	the value to manipulate.  the integer returned as the 	*
 *	value of this function is the number of places in the 	*
 *	format that have been parsed here, to be used in the	*
 *	main function to advance the format-parsing pointer.	*
 ****************************************************************/


int ossimDms::calc_mins_or_secs(double *dd,
                                const char *format,
                                char *res)const
{
	double du;
	int ufactor, ires, numunits;
	char unit, str_fmt[8];

	unit = *format;			/* set to 'm' or 's' */

	ufactor = 1, numunits = 0;

/* count minutes (or seconds) desired in this part of format */

	while (*format == unit) {
	   numunits++;
	   format++, ufactor *= 10;
	}

/* different calc based on beyond decimal pt or not */

	if (theAfterDot == true) {
	   du = *dd * (double)ufactor;
//	   ires = (int)(du + 0.5);
	   ires = (int)(du + 0.5);
	   *dd = (du - ires);
	}
	else {
	   du = (*dd * 60.0) * (double)ufactor;
//	   ires = (int)(du + 0.5) / ufactor;
	   ires = (int)(du) / ufactor;
	   *dd = (du - (ires * ufactor)) / (double)ufactor;
	}
	setup_printf(numunits, str_fmt);
	sprintf(res, str_fmt, ires);
        
	return(numunits);
}

/****************************************************************
 * function setup_printf					*
 * 	the C library function sprintf takes as its second 	*
 * 	argument, a format specifier.  this function constructs	*
 *	that specifier based on 'ival', an integer indicating	*
 *	the number of d's or m's or s's seen in the group	*
 *	just parsed.						*
 ****************************************************************/

void ossimDms::setup_printf(int ival, char *fmt)const
{
	char precis[3];
	
	strcpy(fmt, "%");
	sprintf(precis, "%d", ival);
	strcat(fmt,precis);
	strcat(fmt,".");
	strcat(fmt,precis);
	strcat(fmt,"d");
}

/****************************************************************
 * function set_default						*
 *	assigns a default format to be used as the template for	*
 *	the conversion of the double precision value, degrees.	*
 *	either an error was detected in parsing the format that	*
 *	the user entered, or no format was entered at all.	*
 *	also, clears the result string of any characters which	*
 *	may have been placed therein prior to the call here.	*
 ****************************************************************/

 
void ossimDms::set_default(char *fp, char *rp)const
{
	strcpy(fp, DEFAULT_FORMAT); 	/* assign default format */
	strcpy(rp, SPACES);

}

/****************************************************************
 * function init_values						*
 *	initializes the global values used throughout the	*
 *	function.  see notes in header file.			*
 ****************************************************************/

void ossimDms::init_values(double d)const
{

   theAfterDot     = false;
   theDoingSeconds = false;
   theSign = 1;
   if (d < 0.0)
      theSign = -1;
   
   theWorking = d;
   theIntDegs = (int)theWorking * theSign;
   theDecDegs = (theWorking  * theSign) - theIntDegs;
   
   return;
}


/************************************************************************
 * double function string_to_degree					*
 *									*
 *	requires:	dts.h					*
 *	input:		string variable	parameter			*
 *	returns:	double precision number				*
 *									*
 *	purpose: to calculate a double precision number, representing	*
 *	degrees latitude or longitude, given a string "cdegrees".	*
 *	valid input consists of the following characters:		*
 *									*
 *		+, -, N, n, S, s, E, e, W, w -- direction indicators	*
 *		any digit [0-9]						*
 *		a decimal point, '.' -- says decimal degrees follow	*
 *		an ascii delimiter, for example ' ', TAB, '/' to	*
 *		   indicate that either minutes or seconds follow.	*
 *									*
 *	output is a double precision number, which will be either	*
 *	latitude or longitude degrees and decimal degrees		*
 *									*
 * written as part of the DIAL facility, G&G, WHOI			*
 * April 1992								*
 * Christine L. Hammond							*
 ************************************************************************/



double ossimDms::string_to_degree(const std::string& cdegrees)
{
   if (cdegrees.size() == 0) return 0.0;

   // We must make a non-const copy of "cdegrees".
   char* copy = new char[cdegrees.size()+1];

   //---
   // Since where going to play with the "copy" pointer we must keep the
   // original pointer so we can delete it at the bottom.
   //---
   char* cptr = copy;
   
   strcpy(cptr, cdegrees.c_str());
   
   double degrees, factor, dividend, units;
   int i;
   bool afterdelim;
   char c;
   
   theAfterDot = false;
   afterdelim = false;
   degrees = 0.0;
   units = 0.0;
   factor = 1.0;
   dividend = 60.0;
   theSign = 1;
   
   /* get rid of leading spaces */
   
   while (*cptr == ' ')
      cptr++;
   
   while(*cptr != '\0')
   {
      switch (*cptr)
      {
         
         /* north, west and + values will change sign of degrees to plus */
         case '+':
         case 'n':
         case 'N':
         case 'e':
         case 'E': 
         {
            if(toupper(*cptr) == 'N')
            {
               theLatFlag = true;
            }
            else if(toupper(*cptr) == 'E')
            {
               theLatFlag = false;
            }
            theSign = 1;
            cptr++;
            break;
         }
         
         /* south, east and - values will change sign of degrees to minus */
         
         case '-':
         case 's':
         case 'S':
         case 'w':
         case 'W': 
         {
            if(toupper(*cptr) == 'S')
            {
               theLatFlag = true;
            }
            else if(toupper(*cptr) == 'W')
            {
               theLatFlag = false;
            }
            theSign = -1;
            cptr++;
            break;
         }
         
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
         {
            c = *cptr;
            i = (c - '0');
            
            if (afterdelim == true) {
               
               if (theAfterDot == true) 
                  units = units + (((double)i * factor) / dividend);
               else
                  units = (units * factor) + ((double)i / dividend);
            }
            else {
               
               if (theAfterDot == true)
                  degrees = degrees + ((double)i * factor);
               else
                  degrees = (degrees * factor) + (double)i;
            }
            
            if (theAfterDot == true) 
               factor = factor * .10;
            else
               factor = 10.;
            
            cptr++;
            break;
         }
            
         /* a decimal point indicates a change in the factor used */
         /* to calculate degrees or units (minutes or seconds)    */
            
         case '.':
         {
            factor = .10;
            theAfterDot = true;
            cptr++;
            break;
         }
         
         /* after a delimiter the value contains minutes, first time through */
         
         case ' ':
         {
            
            while (*(cptr + 1) == ' ') 
               cptr++;
            
            degrees = degrees + units;
            units = 0.0;
            
            if (afterdelim == true) 	/* must be seconds */
               dividend = dividend * dividend;
            else 
               afterdelim = true;
            
            factor = 1.;
            
            cptr++;
            
            /* skip any leading zeroes after delimiter */
            
            while (*cptr == '0')
               cptr++;
            
            break;
         }
            
         /* check for a delimiter that is allowable:
            isspace allows: space, tab, cr, nl, vt, ff
            ispunct allows: punctuation char != space or control or letter */
         
         default:
         {
            if (isspace(*cptr) || ispunct(*cptr))
               *cptr = ' ';
         }
         
      }	/* end switch */
      
   }	/* end while loop */
   
   /* add any units that may have been calculated (minutes or seconds) */
   
   degrees = degrees + units;

   //---
   // NOTE:  local variable "cptr" has been moved so do not delete "cptr"
   // delete "copy" which points to the original allocated memory...
   //---
   delete [] copy;
   copy = 0;
   
   return(degrees * (double)theSign);
}
