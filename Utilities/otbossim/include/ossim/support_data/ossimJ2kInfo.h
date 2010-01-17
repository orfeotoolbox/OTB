//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: J2K Info object.
// 
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimJ2kInfo_HEADER
#define ossimJ2kInfo_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/support_data/ossimInfoBase.h>
#include <ossim/base/ossimFilename.h>

class ossimEndian;

/**
 * @brief TIFF info class.
 *
 * Encapsulates the listgeo functionality.
 */
class OSSIM_DLL ossimJ2kInfo : public ossimInfoBase
{
public:
   
   /** default constructor */
   ossimJ2kInfo();

   /** virtual destructor */
   virtual ~ossimJ2kInfo();

   /**
    * @brief open method.
    *
    * @param file File name to open.
    *
    * @return true on success false on error.
    */
   virtual bool open(const ossimFilename& file);
   
   /**
    * Print method.
    *
    * @param out Stream to print to.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

private:

/** Initializes s reference.  Does byte swapping as needed. */
   void readShort(ossim_uint16& s, std::ifstream& str) const;

   /**
    * @brief Prints 0xff52 COD_MARKER (0xff52).
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "j2k." and key is "file_name" key becomes:
    * "j2k.file_name:"
    * @param str Input stream to pass to j2k record for parsing.  Should be
    * just pass the marker.
    * @return output stream.
    */
   std::ostream& printCodMarker(std::ostream& out,
                                const std::string& prefix,
                                std::ifstream& str) const;

   /**
    * @brief Prints 0xff51 SIZ_MARKER (0xff51).
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "j2k." and key is "file_name" key becomes:
    * "j2k.file_name:"
    * @param str Input stream to pass to j2k record for parsing.  Should be
    * just pass the marker.
    * @return output stream.
    */
   std::ostream& printSizMarker(std::ostream& out,
                                const std::string& prefix,
                                std::ifstream& str) const;

   /**
    * @brief Prints 0xff90 SOT_MARKER (0xff90)
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "j2k." and key is "file_name" key becomes:
    * "j2k.file_name:"
    * @param str Input stream to pass to j2k record for parsing.  Should be
    * just pass the marker.
    * @return output stream.
    */
   std::ostream& printSotMarker(std::ostream& out,
                                const std::string& prefix,
                                std::ifstream& str) const;

   /**
    * @brief Prints unhandle segment.  This will only print the marker and
    * record lenght.
    * @param out Stream to output to.
    * @param prefix This will be prepended to key.
    * e.g. Where prefix = "j2k." and key is "file_name" key becomes:
    * "j2k.file_name:"
    * @param str Input stream to pass to j2k record for parsing.  Should be
    * just pass the marker.
    * @param marker.  The marker.
    * @return output stream.
    */
   std::ostream& printUnknownMarker(std::ostream& out,
                                    const std::string& prefix,
                                    std::ifstream& str,
                                    ossim_uint16 marker) const;

   
   ossimFilename  theFile;
   ossimEndian*   theEndian;
};

#endif /* End of "#ifndef ossimJ2kInfo_HEADER" */
