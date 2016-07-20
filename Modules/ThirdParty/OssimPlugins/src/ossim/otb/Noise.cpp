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

#include "otb/Noise.h"
#include "ossimKeyWordListUtilities.h"
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <iomanip>


namespace ossimplugins
{
static const char NOISE[] = "noise";
static const char NUMBER_OF_NOISE_RECORDS_KW[] = "numberOfNoiseRecords";
static const char NAME_OF_NOISE_POLARISATION_KW[] = "nameOfNoisePolarisation";

Noise::Noise():
  m_numberOfNoiseRecords(0),
  m_tabImageNoise(),
  m_polarisation("UNDEFINED")
{
}

bool Noise::saveState(ossimKeywordlist& kwl, std::string const& prefix) const
{
   const std::string pfx = prefix + NOISE;

   std::string s = pfx + "." + NAME_OF_NOISE_POLARISATION_KW;
   add(kwl, pfx, m_polarisation);

   s = pfx + "." + NUMBER_OF_NOISE_RECORDS_KW;
   add(kwl, s, m_numberOfNoiseRecords);

   for (unsigned int i = 0; i < m_tabImageNoise.size(); ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i).c_str() + "]";
      m_tabImageNoise[i].saveState(kwl, s2);
   }

   return true;
}

bool Noise::loadState(const ossimKeywordlist& kwl, std::string const& prefix)
{
   static const char MODULE[] = "Noise::loadState";

   bool result = true;

   const std::string pfx = prefix + NOISE;

   ossimString s;
   const char* lookup = 0;
   std::string s1 = pfx + ".";

#if 1
   get(kwl, s1, NAME_OF_NOISE_POLARISATION_KW, m_polarisation);
#else
   lookup = kwl.find(s1.c_str(), NAME_OF_NOISE_POLARISATION_KW);
   if (lookup)
   {
      m_polarisation = lookup;
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NAME_OF_NOISE_POLARISATION_KW << "\n";
      result = false;
   }
#endif

#if 1
   get(kwl, s1, NUMBER_OF_NOISE_RECORDS_KW, m_numberOfNoiseRecords);
#else
   lookup = kwl.find(s1.c_str(), NUMBER_OF_NOISE_RECORDS_KW);
   if (lookup)
   {
      s = lookup;
      m_numberOfNoiseRecords = s.toUInt32();
   }
   else
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_NOISE_RECORDS_KW << "\n";
      result = false;
   }
#endif

   m_tabImageNoise.clear();
   for (unsigned int i = 0; i < m_numberOfNoiseRecords; ++i)
   {
      std::string s2 = pfx + "[" + ossimString::toString(i).c_str() + "]";
      ImageNoise in;
      if (! in.loadState(kwl, s2))
      {
         return false;
      }
      m_tabImageNoise.push_back(in);
   }
   if( m_numberOfNoiseRecords != m_tabImageNoise.size() )
   {
      ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword " << NUMBER_OF_NOISE_RECORDS_KW << " is different with the number of ImageNoise nodes \n";
   }
   return result;
}


std::ostream& Noise::print(std::ostream& out) const
{
   ossimKeywordlist kwl;
   if (this->saveState(kwl))
   {
      out << setprecision(15) << setiosflags(ios::fixed)
         << "\n Noise class data members:\n";
      out << kwl;
   }
#if 0
   else
   {
      out.set_state(std::ios_base::badbit);
   }
#endif

   return out;
}

}
