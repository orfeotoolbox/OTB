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

#include <otb/RadarSat2NoiseLevel.h>
#include <ossim/base/ossimDpt3d.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>


namespace ossimplugins
{
static const char NOISE[] = "referenceNoiseLevel";

static const char BETA_NOUGHT[] ="BetaNought";
static const char SIGMA_NOUGHT[] ="SigmaNought";
static const char GAMMA[] ="Gamma";

static const char INCIDENCE_ANGLE_CORRECTION_NAME_KW[] = "incidenceAngleCorrectionName"; 
static const char PIXEL_FIRST_NOISE_VALUE_KW[] = "pixelFirstNoiseValue";
static const char STEP_SIZE_KW[] = "stepSize";
static const char NUMBER_OF_NOISE_LEVEL_VALUE_KW[] = "numberOfNoiseLevelValues";
static const char NOISE_LEVEL_VALUES_KW[] = "noiseLevelValues";
static const char UNITS_KW[] = "units";
static const char OFFSET_KW[] = "offset";
static const char GAIN_KW[] = "gain";
static const char SIZE_KW[] = "size";

RadarSat2NoiseLevel::RadarSat2NoiseLevel():
  _incidenceAngleCorrectionName("Unknown"), 
  _pixelFirstNoiseValue(0),
  _stepSize(0),
  _numberOfNoiseLevelValues(0),
  _noiseLevelValues(),
  _units("Unknown"),
  _offset(0.0),
  _gain()
{
}

RadarSat2NoiseLevel::~RadarSat2NoiseLevel()
{
}


RadarSat2NoiseLevel::RadarSat2NoiseLevel(const RadarSat2NoiseLevel& rhs):
  _incidenceAngleCorrectionName(rhs._incidenceAngleCorrectionName), 
  _pixelFirstNoiseValue(rhs._pixelFirstNoiseValue),     
  _stepSize(rhs._stepSize),
  _numberOfNoiseLevelValues(rhs._numberOfNoiseLevelValues),
  _noiseLevelValues(rhs._noiseLevelValues),
  _units(rhs._units), 
  _offset(rhs._offset),
  _gain(rhs._gain)
{
}

RadarSat2NoiseLevel& RadarSat2NoiseLevel::operator=(const RadarSat2NoiseLevel& rhs)
{
  _incidenceAngleCorrectionName = rhs._incidenceAngleCorrectionName; 
  _pixelFirstNoiseValue = rhs._pixelFirstNoiseValue;     
  _stepSize = rhs._stepSize;
  _numberOfNoiseLevelValues = rhs._numberOfNoiseLevelValues;
  _noiseLevelValues = rhs._noiseLevelValues;
  _units = rhs._units; 
  _offset = rhs._offset;
  _gain = rhs._gain;
  return *this;
}

bool RadarSat2NoiseLevel::saveState(ossimKeywordlist& kwl, const char* prefix) const
{
   std::string pfx;
   if (prefix)
   {
      pfx = prefix;
   }
   pfx += NOISE;
   
   if(_incidenceAngleCorrectionName == "Gamma")
   {
   		pfx = pfx + "["+ GAMMA +"]";
   }
   if(_incidenceAngleCorrectionName == "Sigma Nought")
   {
   		pfx = pfx + "["+ SIGMA_NOUGHT +"]";
   }
   if(_incidenceAngleCorrectionName == "Beta Nought")
   {
   		pfx = pfx + "["+ BETA_NOUGHT +"]";
   }

   pfx +=".";
   
   std::string s = pfx + INCIDENCE_ANGLE_CORRECTION_NAME_KW;
   kwl.add(prefix, s.c_str(), _incidenceAngleCorrectionName);
   
   s = pfx + PIXEL_FIRST_NOISE_VALUE_KW;
   kwl.add(prefix, s.c_str(), _pixelFirstNoiseValue);
   
   s = pfx + STEP_SIZE_KW;
   kwl.add(prefix, s.c_str(), _stepSize);

   s = pfx + NUMBER_OF_NOISE_LEVEL_VALUE_KW;
   kwl.add(prefix, s.c_str(), _numberOfNoiseLevelValues);
   
   s = pfx + UNITS_KW;
   kwl.add(prefix, s.c_str(), _units);
   
   for (unsigned int i = 0; i < _noiseLevelValues.size(); ++i)
   {
      	s = pfx + NOISE_LEVEL_VALUES_KW + "[" + ossimString::toString(i).chars() + "]";
   		kwl.add(prefix, s.c_str(), _noiseLevelValues[i]);
   }

   s = pfx + OFFSET_KW;
   kwl.add(prefix, s.c_str(), _offset);

   s = pfx + GAIN_KW;
   kwl.add(prefix, s.c_str(), _gain);

   return true;
}

bool RadarSat2NoiseLevel::loadState(const ossimKeywordlist& kwl, const char* prefix)
{
   static const char MODULE[] = "RadarSat2NoiseLevel::loadState";

   //bool         result = true;
   ossimString  s;
   const char*  lookup = 0;

   std::string pfx("");
   if (prefix)
   {
      pfx = prefix;
   }

   pfx += NOISE;

   if(_incidenceAngleCorrectionName == "Gamma")
   {
   		pfx = pfx + "["+ GAMMA +"]";
   }
   if(_incidenceAngleCorrectionName == "Sigma Nought")
   {
   		pfx = pfx + "["+ SIGMA_NOUGHT +"]";
   }
   if(_incidenceAngleCorrectionName == "Beta Nought")
   {
   		pfx = pfx + "["+ BETA_NOUGHT +"]";
   }
   
   

  std::string s1 = pfx + "["+ GAMMA +"]";
  lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
  if (!lookup)
  {
  		std::string s1 = pfx + "["+ SIGMA_NOUGHT +"]";
  		lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
  		if (!lookup)
  		{
  			std::string s1 = pfx + "["+ BETA_NOUGHT +"]";
  			lookup = kwl.find(s1.c_str(), INCIDENCE_ANGLE_CORRECTION_NAME_KW);
  			if (!lookup)
  			{
    				ossimNotify(ossimNotifyLevel_WARN)
         				<< MODULE << " Keyword not found: " << INCIDENCE_ANGLE_CORRECTION_NAME_KW << "\n";
    				return false;
  			}
  		}
  }

  _incidenceAngleCorrectionName = lookup;
  pfx= s1;

  lookup = kwl.find(s1.c_str(), PIXEL_FIRST_NOISE_VALUE_KW);
  if (lookup)
  {
    s = lookup;
    _pixelFirstNoiseValue = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << PIXEL_FIRST_NOISE_VALUE_KW << "\n";
    return false;
  }

  lookup = kwl.find(s1.c_str(), STEP_SIZE_KW);
  if (lookup)
  {
    s = lookup;
    _stepSize = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << STEP_SIZE_KW << "\n";
    return false;
  }

  lookup = kwl.find(s1.c_str(), NUMBER_OF_NOISE_LEVEL_VALUE_KW);
  if (lookup)
  {
    s = lookup;
    _numberOfNoiseLevelValues = s.toUInt32();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << NUMBER_OF_NOISE_LEVEL_VALUE_KW << "\n";
    return false;
  }


  lookup = kwl.find(s1.c_str(), UNITS_KW);
  if (lookup)
  {
    _units = lookup;
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << UNITS_KW << "\n";
    return false;
  }
   
   _noiseLevelValues.clear();
   for (unsigned int i = 0; i < _numberOfNoiseLevelValues; ++i)
   {
      	s = pfx + NOISE_LEVEL_VALUES_KW + "[" + ossimString::toString(i) + "]";
  	  	lookup = kwl.find(s.c_str(), "");
  	  	if (lookup)
  		{
    		s = lookup;
    		_noiseLevelValues.push_back( s.toDouble() );
  		}      
   }
   
   if( _numberOfNoiseLevelValues != _noiseLevelValues.size() )
   {
    	ossimNotify(ossimNotifyLevel_WARN)
        		 	<< MODULE
        		  	<< " Keyword " << NOISE_LEVEL_VALUES_KW 
        		  	<< " is different with the number of noise level values"
        		  	<< std::endl;
        return false;		  	
   }

  lookup = kwl.find(s1.c_str(), GAIN_KW);
  if (lookup)
  {
    _gain = lookup;;
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << GAIN_KW << "\n";
    return false;
  }

  lookup = kwl.find(s1.c_str(), OFFSET_KW);
  if (lookup)
  {
    s = lookup;
    _offset = s.toFloat64();
  }
  else
  {
    ossimNotify(ossimNotifyLevel_WARN)
         << MODULE << " Keyword not found: " << OFFSET_KW << "\n";
    return false;
  }
   
  if( _numberOfNoiseLevelValues != _noiseLevelValues.size() )
  {
    	ossimNotify(ossimNotifyLevel_WARN)
        		 	<< MODULE
        		  	<< " Keyword " << NOISE_LEVEL_VALUES_KW 
        		  	<< " is different with the number of noise level values"
        		  	<< std::endl;
        return false;		  	
  }


  return true;
}


std::ostream& RadarSat2NoiseLevel::print(std::ostream& out) const
{
  std::ios::fmtflags f(out.flags());

   out << setprecision(15) << setiosflags(ios::fixed)
       << "\n RadarSat2NoiseLevel class data members:\n";

   //const char* prefix = 0;
   //ossimKeywordlist kwl;
   //ossimString pfx;

   /* TODO Add print method*/
   
   //out << kwl;

   out.flags(f);

   return out;
}

}
