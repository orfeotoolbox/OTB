/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <otb/Noise.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>


namespace ossimplugins
{
static const char NOISE[] = "noise";
static const char NUMBER_OF_NOISE_RECORDS_KW[] = "numberOfNoiseRecords"; 
static const char NAME_OF_NOISE_POLARISATION_KW[] = "nameOfNoisePolarisation"; 

Noise::Noise():
  _numberOfNoiseRecords(0),
  _tabImageNoise(),
  _polarisation("UNDEFINED")
{
}

Noise::~Noise()
{
}


Noise::Noise(const Noise& rhs):
  _numberOfNoiseRecords(rhs._numberOfNoiseRecords),
  _tabImageNoise(rhs._tabImageNoise),
  _polarisation(rhs._polarisation)
{
}

Noise& Noise::operator=(const Noise& rhs)
{
  _numberOfNoiseRecords = rhs._numberOfNoiseRecords;
  _tabImageNoise = rhs._tabImageNoise;
  _polarisation = rhs._polarisation;
  return *this;
}

bool Noise::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += NOISE;
   

   std::string s = pfx + "." + NAME_OF_NOISE_POLARISATION_KW;
   kwl.add(prefix, s.c_str(), _polarisation);
   
   s = pfx + "." + NUMBER_OF_NOISE_RECORDS_KW;
   kwl.add(prefix, s.c_str(), _numberOfNoiseRecords);
   
   for (unsigned int i = 0; i < _tabImageNoise.size(); ++i)
   {
     ossimString s2 = pfx + "[" + ossimString::toString(i).c_str() + "]";
      _tabImageNoise[i].saveState(kwl, s2.c_str());
   }

   return true;
}

bool Noise::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "Noise::loadState";

   bool result = true;

   std::string pfx("");
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += NOISE;

   ossimString s;
   const char* lookup = 0;
   std::string s1 = pfx + ".";

  lookup = kwl.find(s1.c_str(), NAME_OF_NOISE_POLARISATION_KW);
  if (lookup)
  {
    _polarisation = lookup;
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NAME_OF_NOISE_POLARISATION_KW << "\n";
    result = false;
  }

  lookup = kwl.find(s1.c_str(), NUMBER_OF_NOISE_RECORDS_KW);
  if (lookup)
  {
    s = lookup;
    _numberOfNoiseRecords = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_NOISE_RECORDS_KW << "\n";
    result = false;
  }

   _tabImageNoise.clear();
   for (unsigned int i = 0; i < _numberOfNoiseRecords; ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i).c_str() + "]";
      ImageNoise in;
      result = in.loadState(kwl, s2.c_str());
      _tabImageNoise.push_back(in);
   }
   if( _numberOfNoiseRecords != _tabImageNoise.size() )
   {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword " << NUMBER_OF_NOISE_RECORDS_KW << " is different with the number of ImageNoise nodes \n";
   }
   return result;
}


std::ostream& Noise::print(std::ostream& out) const
{
   out << std::setprecision(15) << std::setiosflags(std::ios::fixed)
       << "\n Noise class data members:\n";

   const char* prefix = 0;
   ossimKeywordlist kwl;
   ossimString pfx;
   pfx += NOISE;
   ossimString s = pfx + "." + NUMBER_OF_NOISE_RECORDS_KW;
   kwl.add(prefix, s.c_str(), _numberOfNoiseRecords);
   s = pfx + "." + NAME_OF_NOISE_POLARISATION_KW;
   kwl.add(prefix, s.chars(), _polarisation);
   for (unsigned int i = 0; i < _tabImageNoise.size(); ++i)
   {
     ossimString s2 = pfx + "[" + ossimString::toString(i).c_str() + "]";
     _tabImageNoise[i].saveState(kwl, s2.c_str());
   }

   out << kwl;

   return out;
}

}
