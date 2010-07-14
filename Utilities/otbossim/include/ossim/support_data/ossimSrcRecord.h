//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
//  LICENSE: See top level LICENSE.txt file.
//   AUTHOR: Oscar Kramer
//
//*************************************************************************************************
//  $Id$
#ifndef ossimSrcRecord_HEADER
#define ossimSrcRecord_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <vector>

class ossimKeywordlist;

//*************************************************************************************************
//  CLASS DESCRIPTION: 
//! Class used for parsing the command line *.src files. This is a scheme for providing input
//! file information to am ossim app such as ossim-orthoigen.
//!
//! The following keywords with image prefixes as shown are considered:
//! 
//!   image0.file: <image_filename>  
//!   image0.entry: <image_index_in_multi_image_file> (unsigned integer)
//!   image0.ovr: <full/path/to/overview_file.ovr>
//!   image0.hist: <full/path/to/histogram.his>
//!   image0.hist-op: auto-minmax | std-stretch N  (N=1|2|3)
//!   image0.support: <path_to_support_files>
//!   image0.rgb: R,G,rgbB  (unsigned integers starting with 1)
//! 
//! The "support" keyword can be specified in lieu of "ovr" and "hist" when the latter reside in
//! the same external directory. This directory can also contain external image geometry files.
//! If a support dir is provided without an ovr or hist specification, then default filenames are
//! derived from the image filename, but with the support dir as the path.
//! 
//! Multiple files can be specified by incrementing the prefix image index.
//! 
//*************************************************************************************************
class OSSIM_DLL ossimSrcRecord
{
public:
   ossimSrcRecord();
      
   //! Constructs given an in-memory KWL and record index.
   ossimSrcRecord(const ossimKeywordlist& kwl, ossim_uint32 index=0);
   
   //! @brief Initializes record from an in-memory KWL and prefix.
   //!
   //! "prefix.file" e.g. "image0.file" is required.  All other data members will be cleared
   //! if their keword is not present.
   //
   //! @param kwl Keyword list containing one or more records.
   //! @param prefix Like "image0."
   //! @return true on success, false if required keyword is not set.
   bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   //! Returns TRUE if record is valid.
   bool valid() const { return !m_filename.empty(); }

   const ossimFilename&             getFilename()    const { return m_filename; }
   ossim_int32                      getEntryIndex()  const { return m_entryIndex; }
   const ossimFilename&             getSupportDir()  const { return m_supportDir;}
   const ossimString&               getHistogramOp() const { return m_histogramOp;}
   const std::vector<ossim_uint32>& getBands()       const { return m_bandList; }
   
   //! See note below on these data members.
   const ossimFilename& getOverviewPath()  const { return m_overviewPath; }
   const ossimFilename& getHistogramPath() const { return m_histogramPath; }

   void setFilename(const ossimFilename& f)          { m_filename = f; }
   void setEntryIndex(ossim_int32 i)                 { m_entryIndex = i; }
   void setOverview(const ossimFilename& f)          { m_overviewPath = f; }
   void setHistogram(const ossimFilename& f)         { m_histogramPath = f; }
   void setHistogramOp(const ossimString& s)         { m_histogramOp = s; }
   void setBands(const std::vector<ossim_uint32>& v) { m_bandList = v; }

   //! Sets supplementary data files dir. If the OVR and/or hist dirs are undefined, they are also
   //! set to this path.
   void setSupportDir(const ossimFilename& f);

private:

   ossimFilename m_filename;
   ossim_int32   m_entryIndex;
   ossimFilename m_supportDir;
   std::vector<ossim_uint32> m_bandList;
   ossimString m_histogramOp;

   //! The following data members are usually just a copy of m_supportDir, but are provided in
   //! order to support legacy systems where paths to OVR, thumbnails and histogram files
   ossimFilename m_overviewPath;
   ossimFilename m_histogramPath;
};

#endif
