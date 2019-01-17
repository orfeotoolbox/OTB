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


#ifndef RadarSat2NoiseLevel_h
#define RadarSat2NoiseLevel_h

#include <ossim/base/ossimConstants.h>
#include <ossimPluginConstants.h>
#include <otb/JSDDateTime.h>
#include <otb/ImageNoise.h>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup RadarSat2NoiseLevel
 * @brief This class represents an Noise
 */
class OSSIM_PLUGINS_DLL RadarSat2NoiseLevel
{
public:
   /**
    * @brief Constructor
    */
   RadarSat2NoiseLevel();

   /**
    * @brief Destructor
    */
   virtual ~RadarSat2NoiseLevel();

   /**
    * @brief Copy constructor
    */
   RadarSat2NoiseLevel(const RadarSat2NoiseLevel& rhs);

   /**
    * @brief Affectation operator
    */
   RadarSat2NoiseLevel& operator=(const RadarSat2NoiseLevel& rhs);

   /**
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, const char* prefix=0) const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, const char *prefix=0);
   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   
   void set_incidenceAngleCorrectionName(const ossimString&  value)
   {
    	_incidenceAngleCorrectionName = value;
   }  
   void set_pixelFirstNoiseValue(const ossim_uint32&  value)
   {
    	_pixelFirstNoiseValue = value;
   }  
   void set_stepSize(const ossim_uint32&  value)
   {
    	_stepSize = value;
   }  
   void set_numberOfNoiseLevelValues(const ossim_uint32&  value)
   {
    	_numberOfNoiseLevelValues = value;
   }  
   void set_noiseLevelValues(const std::vector<ossim_float64>&  value)
   {
    	_noiseLevelValues = value;
   }  
   void set_units(const ossimString&  value)
   {
    	_units = value;
   }  
   void set_offset(const ossim_float64&  value)
   {
    	_offset = value;
   }  
   void set_gain(const ossimString&  value)
   {
    	_gain = value;
   }  

   const ossimString & get_incidenceAngleCorrectionName() const
   {
   		return _incidenceAngleCorrectionName;
   }
   const ossimString & get_gain() const
   {
   		return _gain;
   }

protected:

      /**
       * @brief Incidence Angle correction Name
       */
      ossimString               _incidenceAngleCorrectionName; 

      /**
       * @brief pixel First Noise Value
       */
      ossim_uint32              _pixelFirstNoiseValue;     
      /**
       * @brief step Size
       */
      ossim_uint32              _stepSize;

      /**
       * @brief number Of Noise Level Values
       */
      ossim_uint32              _numberOfNoiseLevelValues;

      /**
       * @brief noise Level Values
       */
      std::vector<ossim_float64> _noiseLevelValues;

      /**
       * @brief noise Level units
       */
      ossimString               _units; 
      
      /**
       * @brief offset value
       */
      ossim_float64 _offset;

      /**
       * @brief lookup table
       */
	  ossimString   _gain;
      
private:
};
}

#endif
