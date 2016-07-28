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
   /// Constructor
   Noise();

   /**
    * Saves state to a keyword list.
    * @param[in,out] kwl Keyword list to save to.
    * @param[in] prefix added to keys when saved.
    * @return true
    * @throw std::runtime_error if a field cannot be encoded
    */
   bool saveState(ossimKeywordlist& kwl, std::string const& prefix="") const;

   /**
    * Loads (recreate) the state of the object from a keyword list.
    * @return Whether the operation is a success
    * @throw std::runtime_error if a field cannot be decoded
    */
   bool loadState (const ossimKeywordlist &kwl, std::string const& prefix="");

   /// Dumps contents of object to ostream.
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

   /// Image Noise.
   ossim_uint32 m_numberOfNoiseRecords;
   /// Image Noise.
   std::vector<ImageNoise> m_tabImageNoise;

   /// Noise Polarisation Layer.
   std::string m_polarisation;
};
}

#endif
