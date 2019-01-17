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


#ifndef Noise_h
#define Noise_h

#include <ossim/base/ossimConstants.h>
#include <otb/JSDDateTime.h>
#include <otb/ImageNoise.h>

class ossimKeywordlist;

namespace ossimplugins
{

/**
 * @ingroup Noise
 * @brief This class represents an Noise
 */
class OSSIM_PLUGINS_DLL Noise
{
public:
   /**
    * @brief Constructor
    */
   Noise();

   /**
    * @brief Destructor
    */
   virtual ~Noise();

   /**
    * @brief Copy constructor
    */
   Noise(const Noise& rhs);

   /**
    * @brief Affectation operator
    */
   Noise& operator=(const Noise& rhs);

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

  void set_numberOfNoiseRecords(const ossim_int32&  numberOfNoiseRecords)
  {
    _numberOfNoiseRecords = numberOfNoiseRecords;
  }
  void set_imageNoise(const std::vector<ImageNoise>& image_noise)
  {
    _tabImageNoise = image_noise;
  }
  
  const std::vector<ImageNoise> & get_imageNoise() const
  {
    return _tabImageNoise;
  }
  void set_imagePolarisation(const ossimString&  polarisation)
  {
    _polarisation = polarisation;
  }
  
  const ossimString & get_imagePolarisation() const
  {
    return _polarisation;
  }




protected:

      /**
       * @brief Image Noise.
       */
      ossim_uint32 _numberOfNoiseRecords;
      /**
       * @brief Image Noise.
       */
      std::vector<ImageNoise> _tabImageNoise;
      
      /**
       * @brief Noise Polarisation Layer.
       */
      ossimString _polarisation;      
      
  
private:
};
}

#endif
