#include <base/common/ossimDate.h>
#include <iomanip>
#include <sstream>


std::ostream& operator<< (std::ostream& out, const ossimDate& src)
{
   return src.print(out);
}

std::ostream& operator<< (std::ostream & os, ossimLocalTm const & t)
{
   return t.print(os);
}

int operator== (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) == 0);
}

int operator!= (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) != 0);
}

int operator<  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) < 0);
}

int operator<= (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) <= 0);
}

int operator>  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) > 0);
}

int operator>=  (ossimLocalTm const & t1, ossimLocalTm const & t2)
{
   return int(t1.compare(t2) >= 0);
}

char ossimLocalTm::timech = ':';
char ossimLocalTm::datech = '/';

int ossimLocalTm::datefmt = ossimLocalTm::ossimLocalTmFormatIntlShort;
int ossimLocalTm::timefmt = ossimLocalTm::ossimTimeFormatInternational;


ossimLocalTm::ossimLocalTm (time_t t)
{
    if (t == 0)
        t = time(0);
    *(tm *)this = *localtime(&t);
}

ossimLocalTm::ossimLocalTm (tm const & t)
{
    *((tm *)this) = t;
}

ossimLocalTm& ossimLocalTm::operator= (tm const & t)
{
    *((tm *)this) = t;
    return *this;
}

int ossimLocalTm::compare (ossimLocalTm const & t) const
{
    return compare ((time_t)t);
}

int ossimLocalTm::compare (time_t const tt) const
{
    time_t tx = (time_t)*this;
    return (tx == tt) ? 0 : (tx > tt) ? 1 : -1;
}

ossimLocalTm::operator time_t (void) const
{
    return mktime ((tm *)this);
}

int ossimLocalTm::isValid (void) const
{
    static int maxd[] =
    {
        31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    return (tm_year > 0 &&
            tm_mon >= 0 && tm_mon < 12 &&
            tm_mday > 0 && tm_mday <= maxd[tm_mon] &&
            tm_wday < 7 && tm_yday < 367 &&
            tm_sec < 60 && tm_min < 60 && tm_hour < 24);
}

void ossimLocalTm::dSfx (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatSepChar)
        os << datech;
    if (fmt & ossimLocalTmFormatSepSpace)
        os << ' ';
}

void ossimLocalTm::pYear (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatYear)
    {
        int year = tm_year;
        int dig;
        if (fmt & ossimLocalTmFormatYearShort)
        {
            dig = 2;
            year %= 100;
        }
        else
        {
            dig = 4;
            if (year < 200)
                year += 1900;
        }
        os << std::setw((fmt & ossimLocalTmFormatPadYear) ? dig : 0);
        os << std::setfill((fmt & ossimLocalTmFormatZeroYear) ? '0' : ' ');
        os << year;
        if ((fmt & ossimLocalTmFormatYearFirst))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}

void ossimLocalTm::pMonth (std::ostream & os, int fmt) const
{

    static const char * _months[] =
    {
        "January", "February", "March", "April",
        "May", "June", "July", "August", "September",
        "October", "November", "December"
    };

    if (fmt & ossimLocalTmFormatMonth)
    {
        int mon = (tm_mon % 12);
        if (fmt & ossimLocalTmFormatMonText)
        {
            char const * tmon = _months[mon];
            if (!(fmt & ossimLocalTmFormatPadMon))
                os << tmon;
            else
                for (int x = 0; x < 3; ++x)
                    os << tmon[x];
        }
        else
        {
            ++mon;
            os << std::setw((fmt & ossimLocalTmFormatPadMon) ? 2 : 0);
            os << std::setfill((fmt & ossimLocalTmFormatZeroMon) ? '0' : ' ');
            os << mon;
        }
        if (((fmt & ossimLocalTmFormatYear) && !(fmt & ossimLocalTmFormatYearFirst)) ||
            ((fmt & ossimLocalTmFormatDay) && (fmt & ossimLocalTmFormatMonFirst)))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}


void ossimLocalTm::pDate (std::ostream & os, int fmt) const
{
    if (fmt & ossimLocalTmFormatDay)
    {
        int day = tm_mday;
        os << std::setw((fmt & ossimLocalTmFormatPadDay) ? 2 : 0);
        os << std::setfill((fmt & ossimLocalTmFormatZeroDay) ? '0' : ' ');
        os << day;
        if (!(fmt & ossimLocalTmFormatYearFirst) || !(fmt & ossimLocalTmFormatMonFirst))
        {
            fmt &= (ossimLocalTmFormatSepChar|ossimLocalTmFormatSepSpace);
            dSfx (os, fmt);
        }
    }
}

std::ostream& ossimLocalTm::print(std::ostream & os,
                                    int df,
                                    int tf) const
{
    std::ostringstream pTmp;
    printDate (pTmp, df);
    pTmp << ' ';
    printTime (pTmp, tf);

    return os << pTmp.str();
}

std::ostream& ossimLocalTm::printDate (std::ostream & os, int fmt) const
{
    std::ostringstream pTmp;

    static const char * _days[] =
    {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };

    if (fmt & ossimLocalTmFormatDayOfWeek)
    {
        int day = tm_wday % 7;
        char const * p = _days[day];
        if (fmt & ossimLocalTmFormatPadDay)
            for (int x = 0; x < 3; ++x)
                pTmp << p[x];
        else
        {
            pTmp << p;
            if (fmt & ossimLocalTmFormatDMY)
                pTmp << ',';
        }
        if ((fmt & ossimLocalTmFormatDMY) && fmt & ossimLocalTmFormatSepSpace)
            pTmp << ' ';
    }
    if (fmt & ossimLocalTmFormatYearFirst)
        pYear (pTmp, fmt);
    if (fmt & ossimLocalTmFormatMonFirst)
        pMonth (pTmp, fmt);
    pDate (pTmp, fmt);
    if (!(fmt & ossimLocalTmFormatMonFirst))
        pMonth (pTmp, fmt);
    if (!(fmt & ossimLocalTmFormatYearFirst))
        pYear (pTmp, fmt);

    return os << pTmp.str();
}
void ossimLocalTm::tSfx (std::ostream & os, int fmt, char ch) const
{
    if (fmt & ossimTimeFormatSepAbbrev)
        os << ch;
    if (fmt & ossimTimeFormatSepChar)
        os << timech;
    if (fmt & ossimTimeFormatSepSpace)
        os << ' ';
}

void ossimLocalTm::pHour (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatHour)
    {
        int hour = tm_hour;
        if (!(fmt & ossimTimeFormat24hour))
        {
            if (hour > 12)
                hour -= 12;
            else if (!hour && (fmt & ossimTimeFormatAmPm))
                hour += 12;
        }
        os << std::setw((fmt & ossimTimeFormatPadHour) ? 2 : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroHour) ? '0' : ' ');
        os << hour;
        if (!(fmt & ossimTimeFormatMins))
            fmt &= ossimTimeFormatSepAbbrev;
        tSfx (os, fmt, 'h');
    }
}

void ossimLocalTm::pMins (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatMins)
    {
        int min = tm_min;
        int dig = 2;
        if (!(fmt & ossimTimeFormatHour))
        {
            min += (tm_hour * 60);
            dig += 2;
        }
        os << std::setw((fmt & ossimTimeFormatPadMins) ? dig : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroMins) ? '0' : ' ');
        os << min;
        if (!(fmt & ossimTimeFormatSecs))
            fmt &= ossimTimeFormatSepAbbrev;
        tSfx (os, fmt, 'm');
    }
}

void ossimLocalTm::pSecs (std::ostream & os, int fmt) const
{
    if (fmt & ossimTimeFormatSecs)
    {
        int sec = tm_sec;
        int dig = 2;
        if (!(fmt & (ossimTimeFormatHour|ossimTimeFormatMins)))
        {
            sec += ((tm_hour * 60) + tm_min) + 60;
            dig += 3;
        }
        os << std::setw((fmt & ossimTimeFormatPadSecs) ? dig : 0);
        os << std::setfill((fmt & ossimTimeFormatZeroSecs) ? '0' : ' ');
        os << sec;
        if (fmt & ossimTimeFormatAmPm)
            fmt &= ~ossimTimeFormatSepChar;
        else
            fmt &= (ossimTimeFormatSepAbbrev|ossimTimeFormatSepSpace);
        tSfx (os, fmt, 's');
    }
}

std::ostream &ossimLocalTm::printTime (std::ostream & os, int fmt) const
{
    std::ostringstream pTmp;
    pHour (pTmp, fmt);
    pMins (pTmp, fmt);
    pSecs (pTmp, fmt);
    if (fmt & ossimTimeFormatAmPm)
    {
        pTmp << (tm_hour > 11 ? "pm" : "am");
    }

    return os << pTmp.str();
}


int ossimLocalTm::getYear()const
{
   int result = tm_year;
   if (result < 200)
   {
      result += 1900;
   }
   return result;
}

int ossimLocalTm::getShortYear()const
{
   return (getYear()%100);
}

int ossimLocalTm::getMonth()const
{
   return ((tm_mon % 12)+1); 
}

int ossimLocalTm::getDay()const
{
   return tm_mday;
}

double ossimLocalTm::getJulian()const
{
   int J = getMonth();
   int K = getDay();
   int I = getYear();

   return K-32075+1461*(I+4800+(J-14)/12)/4+367*(J-2-(J-14)/12*12)
      /12-3*((I+4900+(J-14)/12)/100)/4+
       (getHour()/24.0)+
       (getMin()/1440.0)+
       (getSec()/86400.0);
}

double ossimLocalTm::getModifiedJulian()const
{
   return getJulian() - 2400000.5;
}

ossimLocalTm& ossimLocalTm::setDay(int day)
{
   tm_mday = day;
   
   return *this;
}

ossimLocalTm& ossimLocalTm::setMonth(int month)
{
   tm_mon = month - 1;

   return *this;
}

ossimLocalTm& ossimLocalTm::setYear(int year)
{
   if(year < 2099)
   {
      tm_year = year - 1900;
   }
   else
   {
      tm_year = year;
   }

   return *this;
}

ossimLocalTm& ossimLocalTm::setDateFromJulian(double jd)
{
   double fractional = jd - (long)jd;
   long l;
   long n;
   long i;
   long j;
   long k;


   l= (long)(jd+68569);
   n= 4*l/146097;
   l= l-(146097*n+3)/4;
   i= 4000*(l+1)/1461001;
   l= l-1461*i/4+31l;
   j= 80*l/2447;
   k= l-2447*j/80;
   l= j/11;
   j= j+2-12*l;
   i= 100*(n-49)+i+l;
   setDay(k);
   setMonth(j);
   setYear(i);

   setFractionalDay(fractional);
   
   return *this;
}

ossimLocalTm& ossimLocalTm::setDateFromModifiedJulian(double mjd)
{
   setDateFromJulian(mjd + 2400000.5);

   return *this;
}

void ossimLocalTm::setFractionalDay(double fractionalDay)
{
   int h, m, s;
   extractHmsFromFractionalDay(fractionalDay, h, m, s);

   setHour(h);
   setMin(m);
   setSec(s);
}

void ossimLocalTm::extractHmsFromFractionalDay(double fractionalDay,
                                               int &h,
                                               int &m,
                                               int &s)
{
   fractionalDay *=24;
   h = (int)fractionalDay;
   fractionalDay = fractionalDay-h;
   fractionalDay*=60;
   m = (int)fractionalDay;
   fractionalDay = fractionalDay-m;
   fractionalDay*=60;
   s = (int)fractionalDay;
}

int ossimLocalTm::getHour()const
{
   return tm_hour;
}

int ossimLocalTm::getMin()const
{
   return tm_min;
}

int ossimLocalTm::getSec()const
{
   return tm_sec;
}

ossimLocalTm& ossimLocalTm::setHour(int h)
{
   tm_hour = h;

   return *this;
}

ossimLocalTm& ossimLocalTm::setMin(int m)
{
   tm_min = m;

   return *this;
}

ossimLocalTm& ossimLocalTm::setSec(int s)
{
   tm_sec = s;

   return *this;
}

std::ostream& ossimDate::print (std::ostream & os) const
{
   return printDate (os, _fmt);
}


std::ostream& operator <<(std::ostream& out, const ossimTime& src)
{
   return src.print(out);
}
ossimTime::ossimTime(int tmfmt)
      : ossimLocalTm(0), _fmt(tmfmt)
{
}
ossimTime::ossimTime(ossimTime const & t,
                      int tmfmt)
      : ossimLocalTm (t), _fmt(tmfmt)
{
}

ossimTime::ossimTime(time_t t, int tmfmt)
      : ossimLocalTm (t), _fmt(tmfmt)
{
}
      
int ossimTime::fmt(int f)
{
   return _fmt = f;
}

int ossimTime::fmt(void) const
{
   return _fmt;
}

std::ostream& ossimTime::print (std::ostream & os) const
{
   return printTime(os, _fmt);
}
