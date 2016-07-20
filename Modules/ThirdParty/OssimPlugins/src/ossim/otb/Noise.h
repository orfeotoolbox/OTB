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
#include <otb/ImageNoise.h>
#include <iosfwd>
#include <string>

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
    * @brief Method to save object state to a keyword list.
    * @param kwl Keyword list to save to.
    * @param prefix added to keys when saved.
    * @return true on success, false on error.
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix="") const;

   /**
    * @brief Method to the load (recreate) the state of the object from a
    * keyword list. Return true if ok or false on error.
    * @return true if load OK, false on error
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix="");
   /*!
    * METHOD: print()
    * Fulfills base-class pure virtual. Dumps contents of object to ostream.
    */
   std::ostream& print(std::ostream& out) const;

   // TODO: the numberOfNoiseRecords is redundant with m_tabImageNoise.size()
   void set_numberOfNoiseRecords(ossim_int32  numberOfNoiseRecords)
   {
      m_numberOfNoiseRecords = numberOfNoiseRecords;
   }
   void set_imageNoise(const std::vector<ImageNoise>& image_noise)
   {
      m_tabImageNoise = image_noise;
   }

   const std::vector<ImageNoise> & get_imageNoise() const
   {
      return m_tabImageNoise;
   }
   void set_imagePolarisation(const std::string&  polarisation)
   {
      m_polarisation = polarisation;
   }

   const std::string & get_imagePolarisation() const
   {
      return m_polarisation;
   }

private:

   /**
    * @brief Image Noise.
    */
   ossim_uint32 m_numberOfNoiseRecords;
   /**
    * @brief Image Noise.
    */
   std::vector<ImageNoise> m_tabImageNoise;

   /**
    * @brief Noise Polarisation Layer.
    */
   std::string m_polarisation;

};
}

#endif
