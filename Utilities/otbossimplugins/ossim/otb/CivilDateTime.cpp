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

#include <otb/JulianDate.h>
#include <otb/CivilDateTime.h>
#include <otb/JSDDateTime.h>
#include <otb/GMSTDateTime.h>

#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ostream>

namespace ossimplugins
{


const double CivilDateTime::JOURCIVIL_LENGTH = 86400.0 ;

CivilDateTime::CivilDateTime():
  _year(0),
  _month(0),
  _day(0),
  _second(0),
  _decimal(0.0)
{
}

CivilDateTime::CivilDateTime(int year, int month, int day, int second, double decimal):
  _year(year),
  _month(month),
  _day(day),
  _second(second),
  _decimal(decimal)
{
}

CivilDateTime::CivilDateTime(char* Utc)
{
  SetUtcDateTime(Utc);
}

CivilDateTime::CivilDateTime(const char* Utc)
{
  SetUtcDateTime(Utc);
}

CivilDateTime::~CivilDateTime()
{
}

CivilDateTime::CivilDateTime(const CivilDateTime& rhs):
  _year(rhs._year),
  _month(rhs._month),
  _day(rhs._day),
  _second(rhs._second),
  _decimal(rhs._decimal)
{

}

CivilDateTime& CivilDateTime::operator=(const CivilDateTime& rhs)
{
  _year = rhs._year;
  _month = rhs._month;
  _day = rhs._day;
  _second = rhs._second;
  _decimal = rhs._decimal;

  return *this;
}

int CivilDateTime::AsJulianDate(JulianDate* julianDate)
{
  int etat , test ;
  int an , mois , i , j ;
  double b , d ;

  if (_month > 2)
  {
    an = _year ;
    mois = _month ;
  }
  else
  {
    an = _year - 1 ;
    mois = _month + 12 ;
  }

  test = _year * 10000 + _month * 100 + _day ;
  if ((test > 15821004) && (test < 15821015))
    etat = 1 ;
  else
  {
    etat = 0 ;
    d = (an < 0)  ?  0.75  :  0.0 ;
    if (test >= 15821015)
    {
      i = an / 100 ;
      j = 2 - i + i/4 ;
      b = (double)j + 1720994.5 ;
    }
    else
      b = 1720994.5 ;

    i = (int) (365.25  * (double)an - d) ;
    j = (int) (30.6001 * (double)(mois + 1)) ;
    double jourJulien =   (double)(i + j) + (double)_day
                  + ((double)_second + _decimal) / JOURCIVIL_LENGTH + b ;

    julianDate->set_julianDate(jourJulien);
  }

  return etat;
}

int CivilDateTime::SetUtcDateTime(char* Utc)
{
  char str_an[5];
  char str_day[3];
  char str_month[4];
  char str_hour[3];
  char str_min[3];
  char str_sec[3];
  char str_micro[7];
  int i;
  double exposant;

  /*
    * year extraction
    */
  for (i = 0; i < 4; i++)
  {
    str_an[i] = Utc[i+7];
  }

  str_an[4] = '\0';
  _year = atoi(str_an);
  if ( (_year > 2050) || (_year < 1950) )
    {
    return 1;
    }

  /*
    * month extraction
    */
  for (i = 0; i < 3; i++)
  {
    str_month[i] = Utc[i+3];
  }

  str_month[3] = '\0';
  if (!strcmp(str_month, "JAN"))  _month = 1;
  else if (!strcmp(str_month, "FEV"))  _month = 2;
  else if (!strcmp(str_month, "MAR"))  _month = 3;
  else if (!strcmp(str_month, "APR"))  _month = 4;
  else if (!strcmp(str_month, "MAY"))  _month = 5;
  else if (!strcmp(str_month, "JUN"))  _month = 6;
  else if (!strcmp(str_month, "JUL"))  _month = 7;
  else if (!strcmp(str_month, "AUG"))  _month = 8;
  else if (!strcmp(str_month, "SEP"))  _month = 9;
  else if (!strcmp(str_month, "OCT"))  _month = 10;
  else if (!strcmp(str_month, "NOV"))  _month = 11;
  else if (!strcmp(str_month, "DEC"))  _month = 12;
  else
  {
    return 1;
  }

  /*
    * day extraction
    */
  for (i = 0; i < 2; i++)
  {
    str_day[i] = Utc[i];
  }

  str_day[2] = '\0';
  _day = atoi(str_day);
  if ( (_day < 1) || (_day > 31) )
  {
    return 1;
  }

  /*
    * seconds extraction
    */
  for (i = 0; i < 2; i++)
    {
    str_hour[i] = Utc[i+12];
    str_min[i]  = Utc[i+15];
    str_sec[i]  = Utc[i+18];
    }

  str_hour[2] = '\0';if ( (atoi(str_hour) < 0) || (atoi(str_hour) > 23) )
  {
    return 1;
  }

  str_min[2]  = '\0';if ( (atoi(str_min)  < 0) || (atoi(str_hour) > 59) )
  {
    return 1;
  }
  str_sec[2]  = '\0';if ( (atoi(str_sec)  < 0) || (atoi(str_hour) > 59) )
  {
    return 1;
  }
  _second = 3600*atoi(str_hour) + 60*atoi(str_min) + atoi(str_sec);

  /*
    * microseconds extraction
    */
  for (i = 0; i < 6; i++)
  {
    str_micro[i] = Utc[i+21];
  }

  str_micro[6] = '\0';
  i = 0;
  exposant = 10e-6;
  while ((unsigned int)i < strlen(str_micro) )
    {
    if (str_micro[i] == '0') exposant = exposant * 0.1;
    i++;
    }

  _decimal = exposant * (double) atoi(str_micro);
  if ( (_decimal < 0) || (_decimal >= 1) )
  {
    return 1;
  }

  return 0;

}

int CivilDateTime::SetUtcDateTime(const char* Utc)
{
  char str_an[5];
  char str_day[3];
  char str_month[4];
  char str_hour[3];
  char str_min[3];
  char str_sec[3];
  char str_micro[7];
  int i;
  double exposant;

  /*
    * year extraction
    */
  for (i = 0; i < 4; i++)
  {
    str_an[i] = Utc[i+7];
  }

  str_an[4] = '\0';
  _year = atoi(str_an);
  if ( (_year > 2050) || (_year < 1950) )
    {
    return 1;
    }

  /*
    * month extraction
    */
  for (i = 0; i < 3; i++)
  {
    str_month[i] = Utc[i+3];
  }

  str_month[3] = '\0';
  if (!strcmp(str_month, "JAN"))  _month = 1;
  else if (!strcmp(str_month, "FEV"))  _month = 2;
  else if (!strcmp(str_month, "MAR"))  _month = 3;
  else if (!strcmp(str_month, "APR"))  _month = 4;
  else if (!strcmp(str_month, "MAY"))  _month = 5;
  else if (!strcmp(str_month, "JUN"))  _month = 6;
  else if (!strcmp(str_month, "JUL"))  _month = 7;
  else if (!strcmp(str_month, "AUG"))  _month = 8;
  else if (!strcmp(str_month, "SEP"))  _month = 9;
  else if (!strcmp(str_month, "OCT"))  _month = 10;
  else if (!strcmp(str_month, "NOV"))  _month = 11;
  else if (!strcmp(str_month, "DEC"))  _month = 12;
  else
  {
    return 1;
  }

  /*
    * day extraction
    */
  for (i = 0; i < 2; i++)
  {
    str_day[i] = Utc[i];
  }

  str_day[2] = '\0';
  _day = atoi(str_day);
  if ( (_day < 1) || (_day > 31) )
  {
    return 1;
  }

  /*
    * seconds extraction
    */
  for (i = 0; i < 2; i++)
    {
    str_hour[i] = Utc[i+12];
    str_min[i]  = Utc[i+15];
    str_sec[i]  = Utc[i+18];
    }

  str_hour[2] = '\0';if ( (atoi(str_hour) < 0) || (atoi(str_hour) > 23) )
  {
    return 1;
  }

  str_min[2]  = '\0';if ( (atoi(str_min)  < 0) || (atoi(str_hour) > 59) )
  {
    return 1;
  }
  str_sec[2]  = '\0';if ( (atoi(str_sec)  < 0) || (atoi(str_hour) > 59) )
  {
    return 1;
  }
  _second = 3600*atoi(str_hour) + 60*atoi(str_min) + atoi(str_sec);

  /*
    * microseconds extraction
    */
  for (i = 0; i < 6; i++)
  {
    str_micro[i] = Utc[i+21];
  }

  str_micro[6] = '\0';
  i = 0;
  exposant = 10e-6;
  while ((unsigned int)i < strlen(str_micro) )
    {
    if (str_micro[i] == '0') exposant = exposant * 0.1;
    i++;
    }

  _decimal = exposant * (double) atoi(str_micro);
  if ( (_decimal < 0) || (_decimal >= 1) )
  {
    return 1;
  }

  return 0;

}


int CivilDateTime::AsJSDDateTime(JSDDateTime* JSDdate)
{
  int etat , k ;
  JulianDate jourJulien;
  double r ;

  etat = AsJulianDate(&jourJulien );
  if (etat == 0)
  {
    k = (int)(jourJulien.get_julianDate() - 0.5) ;

    double day0hTU = (double)k + 0.5;
    JSDdate->set_day0hTU(JulianDate(day0hTU));

    r = (jourJulien.get_julianDate() - day0hTU) * JOURCIVIL_LENGTH ;
    JSDdate->set_second(floor (r)) ;
    JSDdate->set_decimal(r - JSDdate->get_second()) ;
  }

  return etat ;
}

int CivilDateTime::AsGMSTDateTime(GMSTDateTime* GMST)
{
  const double TWOPI      = 6.28318530717958647693 ;
  const double OMEGATERRE  = 6.28318530717958647693 / 86164.09054 ;
  int etat ;
  JulianDate jourJulien;
  double t=0.0;
        double tsm0hTU=0.0;
  double tsm;
  CivilDateTime date0hTU(_year, _month, _day, 0, 0.0);


  etat = date0hTU.AsJulianDate(&jourJulien );
  if (etat == 0)
  {
    if (GMST->get_origine() == GMSTDateTime::AN1900)
    {
      t = (jourJulien.get_julianDate() - 2415020.0) / 36525.0 ;
      tsm0hTU = 23925.836 + t * (8640184.542 + t * 0.0929) ;
      tsm0hTU = tsm0hTU * TWOPI / JOURCIVIL_LENGTH ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN1950)
    {
      t = jourJulien.get_julianDate() - 2433281.5 ;
      tsm0hTU = 1.72944494 + t * 6.3003880987 ;
    }
    else if (GMST->get_origine() == GMSTDateTime::AN2000)
    {
      t = (jourJulien.get_julianDate() - 2451545.0) / 36525.0 ;
      tsm0hTU =   24110.54841 + t * (8640184.812866 + t * (0.093103 - t * 6.210e-6)) ;
      tsm0hTU = tsm0hTU * TWOPI / JOURCIVIL_LENGTH ;
    }
    t = tsm0hTU + ((double)_second + _decimal) * OMEGATERRE ;
    tsm = fmod (t, TWOPI) ;

    if (tsm < 0.0)
    {
      tsm = tsm + TWOPI ;
    }
    GMST->set_tms(tsm);
  }

  return etat ;
}


CivilDateTime::operator JulianDate()
{
  JulianDate date;
  AsJulianDate(&date);
  return date;
}

CivilDateTime::operator JSDDateTime()
{
  JSDDateTime date;
  AsJSDDateTime(&date);
  return date;
}

CivilDateTime::operator GMSTDateTime()
{
  GMSTDateTime date;
  AsGMSTDateTime(&date);
  return date;
}

std::ostream& CivilDateTime::print(std::ostream& out) const
{
   out << "CivilDateTime:"
       << "\nyear:    " << _year
       << "\nmonth:   " << _month
       << "\nday:     " << _day
       << "\nsecond:  " << _second
       << "\ndecimal: " << _decimal
       << std::endl;

   return out;
}

std::ostream& operator<<(std::ostream& out, const CivilDateTime& obj)
{
   return obj.print(out);
}
}
