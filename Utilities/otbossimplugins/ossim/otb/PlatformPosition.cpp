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

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

#include <otb/PlatformPosition.h>
#include <otb/Ephemeris.h>
#include <otb/HermiteInterpolator.h>
#include <ossim/base/ossimKeywordlist.h>

namespace ossimplugins
{

static const char NUMBER_PLATFORM_POSITIONS_KW[] = "platform_positions_count";

PlatformPosition::PlatformPosition():
   _nbrData(0),
   _data(NULL),
   _t(NULL),
   _p(NULL),
   _dp(NULL),
   _interpolator(NULL)
{
}

PlatformPosition::~PlatformPosition()
{
   Clear();
}

void PlatformPosition::Clear()
{
   if(_data != NULL)
   {
      for (int i=0;i<_nbrData;i++)
      {
         delete _data[i];
      }
      delete [] _data;
   }
   _data = NULL;
   _nbrData = 0;

   delete[] _t;
   if ((_p != NULL) && (_dp != NULL)) {
     for (int j=0; j<3; ++j) {
       delete[] _p[j];
       delete[] _dp[j];
       delete _interpolator[j];
     }
   }
   delete[] _p;
   delete[] _dp;

   delete[] _interpolator;
}

PlatformPosition::PlatformPosition(const PlatformPosition& rhs)
{
   InitData(rhs._data, rhs._nbrData);
}

PlatformPosition& PlatformPosition::operator=(const PlatformPosition& rhs)
{
   Clear();
   InitData(rhs._data, rhs._nbrData);
   return *this;
}

PlatformPosition::PlatformPosition(Ephemeris** data, int nbrData)
{
   InitData(data, nbrData);
}

void PlatformPosition::InitData(Ephemeris** data, int nbrData)
{
   _nbrData = nbrData;
   _data = new Ephemeris*[_nbrData];
   for (int i=0; i<_nbrData; i++)
   {
      _data[i] = data[i]->Clone();
   }
   InitAuxiliaryData();
}

void PlatformPosition::InitAuxiliaryData()
{
   const double JOURCIVIL_LENGTH = 86400.0;
   _t = new double[_nbrData];
   _p = new double*[3];
   _dp = new double*[3];
   _interpolator = new HermiteInterpolator*[3];
   for (int j=0; j<3; ++j) {
     _p[j] = new double[_nbrData];
     _dp[j] = new double[_nbrData];
   }

   _t[0] = 0.0;
   for (int i = 1; i < _nbrData; i++)
   {
     _t[i] =   (_data[i]->get_date().get_day0hTU().get_julianDate() - _data[0]->get_date().get_day0hTU().get_julianDate())
         * JOURCIVIL_LENGTH
         + _data[i]->get_date().get_second() - _data[0]->get_date().get_second()
         + _data[i]->get_date().get_decimal() - _data[0]->get_date().get_decimal();
   }

   for (int j = 0; j < 3; j++)
   {
     for (int i = 0; i < _nbrData; i++)
     {
       _p[j][i] = _data[i]->get_position()[j];
       _dp[j][i] = _data[i]->get_speed()[j];
     }
     _interpolator[j] = new HermiteInterpolator(_nbrData, _t, _p[j], _dp[j]);
   }

}

Ephemeris* PlatformPosition::Interpolate(JSDDateTime date) const
{
   const double JOURCIVIL_LENGTH = 86400.0;
   Ephemeris* ephem = NULL;
   if (_nbrData <= 1)
   {
      return NULL;
   }
   /*
    * The first element of the list is cloned to ensure that the
    * output ephemeris is expressed in the same coordinate system as
    * input ones
    */
   ephem = _data[0]->Clone();

   /* NORMAL CASE */
   /*------------*/
   double dt = 0.0;

   if (ephem != NULL)
   {
     ephem->set_date(date);

     dt =  (date.get_day0hTU().get_julianDate()
            - _data[0]->get_date().get_day0hTU().get_julianDate())
         * JOURCIVIL_LENGTH
         + date.get_second() - _data[0]->get_date().get_second()
         + date.get_decimal() - _data[0]->get_date().get_decimal();

     /* Computation by Everett  */
     /*---------------------*/
     double pos[3];
     double speed[3];
     for (int j = 0; j < 3; j++)
     {
       _interpolator[j]->Interpolate(dt, pos[j], speed[j]);
     }
     ephem->set_position(pos);
     ephem->set_speed(speed);
   }
   return ephem;
}

void PlatformPosition::setData(Ephemeris** data, int nbrData)
{
   Clear();
   InitData(data, nbrData);
}

Ephemeris* PlatformPosition::getData(int noData) const
{
   if(noData >=0 && noData < _nbrData)
     {
       return _data[noData];
     }
   return NULL;
}

int PlatformPosition::getNbrData() const
{
     return _nbrData;
}


bool PlatformPosition::saveState(ossimKeywordlist& kwl,
                                 const char* prefix) const
{
   kwl.add(prefix, NUMBER_PLATFORM_POSITIONS_KW, _nbrData);

   std::string s1;
   if (prefix)
   {
      s1 = prefix;
   }

   for (int i = 0; i < _nbrData; ++i)
   {
      std::string s2 = s1;
      s2 += "platform_position[";
      s2 += ossimString::toString(i).chars();
      s2+= "]";
      _data[i]->saveState(kwl, s2.c_str());
   }

   return true;
}


bool PlatformPosition::loadState(const ossimKeywordlist& kwl,
                                 const char* prefix)
{
   bool result = true;

   Clear();

   const char* lookup = 0;
   lookup = kwl.find(prefix, NUMBER_PLATFORM_POSITIONS_KW);
   if (!lookup)
   {
     return false;
   }
   ossimString s = lookup;
   _nbrData = s.toInt();

   if (_nbrData)
   {
     std::string s1;
     if (prefix)
     {
       s1 = prefix;
     }

     _data = new  Ephemeris*[_nbrData];
     for (int i = 0; i < _nbrData; ++i)
     {
       std::string s2 = s1;
       s2 += "platform_position[";
       s2 += ossimString::toString(i).chars();
       s2+= "]";

       _data[i] = new Ephemeris();
       _data[i]->loadState(kwl, s2.c_str());
     }
   }
   InitAuxiliaryData();
   return result;
}
}
