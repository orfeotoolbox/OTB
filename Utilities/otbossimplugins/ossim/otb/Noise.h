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
