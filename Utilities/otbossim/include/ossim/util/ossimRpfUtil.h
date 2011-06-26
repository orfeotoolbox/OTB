//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:  ossimRpfUtil.h
// 
// Utility class to stuff with rpf files.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimRpfUtil_HEADER
#define ossimRpfUtil_HEADER 1

#include <ossim/base/ossimReferenced.h>
#include <ossim/imaging/ossimImageGeometry.h> 

class ossimFilename;
class ossimGpt;
class ossimRpfToc;
class ossimRpfTocEntry;

class OSSIM_DLL ossimRpfUtil : public ossimReferenced
{
public:

   /** @brief default constructor */
   ossimRpfUtil();


   /**
    * @brief Write dot rpf file(s) to output directory from a.toc file.
    *
    * This creates a dot rpf file for each entry.
    *
    * @param aDotFile The a.toc file.
    *
    * @param outputDir Directory to write dot rpf file(s) to.
    *
    * throws ossimException on error.
    */
   void writeDotRpfFiles( const ossimFilename& aDotTocFile,
                          const ossimFilename& outputDir);

   void writeDotRpfFile( const ossimRpfToc* toc,
                         const ossimRpfTocEntry* tocEntry,
                         const ossimFilename& outputDir,
                         ossim_uint32 entry);

protected:

   /**
    * @brief protected virtual destructor
    *
    * Do not call directly.  Use as a ref ptr.
    */
   virtual ~ossimRpfUtil();

private:

   /**
    * @brief Method to test for 360 spread, 180.0 <--> 180.00 and set
    * leftLon to -180 if both left and right are 180.0 degrees.
    *
    * This adds a small tolerance to the test.
    * 
    * @param left Left hand side of scene.  This is the value
    * that will be flipped to -180 if test is true.
    *
    * @param right Right hand side of scene.
    */
   void checkLongitude(ossimGpt& left, ossimGpt& right) const;

   /** @brief Method to get the file for entry. */
   void getDotRfpFilenameForEntry(const ossimFilename& outputDir,
                                  ossim_uint32 entry,
                                  ossimFilename& outFile) const;
   
}; // Matches: class ossimRpfUtil

#endif /* #ifndef ossimRpfUtil_HEADER */
