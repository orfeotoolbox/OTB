//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Support data class for a Shuttle Radar Topography Mission (SRTM) file.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmSupportData.h 17602 2010-06-20 19:12:24Z dburken $
#ifndef ossimSrtmSupportData_HEADER
#define ossimSrtmSupportData_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimIoStream.h>
#include <ossim/projection/ossimProjection.h>

/**
 * @class ossimStrmSupportData
 * Support data class for a Shuttle Radar Topography Mission (SRTM) file.
 * Provides lines, samples, post spacing and geometry info given a valid
 * srtm file.  This class will set an error if the file is not deemed to
 * be valid.
 */
class OSSIMDLLEXPORT ossimSrtmSupportData : public ossimObject
{
public:
   /** default constructor */
   ossimSrtmSupportData();

   /** destructor */
   virtual ~ossimSrtmSupportData();

   /**
    * Initializes object from file.  File is expected to be in a format like
    * N27W081.hgt or n27w081.hgt where "N27" is the South West latitude of
    * the cell and "W081" is the South West longitude of the cell.
    *
    * @param srtmFile File to set.
    *
    * @param scanForMinMax If true and there is no ossim meta data file with
    * the min and max keywords in it, this will call computeMinMax which will
    * scan the file for min and max.
    *
    * @return Returns true if file is deemed to be an srtm, false if not.
    *
    * @note This will write an ossim meta data (.omd) if one does not exists.
    */
   bool setFilename(const ossimFilename& srtmFile,
                    bool scanForMinMax = false);

   /** @return Returns the srtm file name. */
   ossimFilename getFilename() const;

   /**
    * @return Returns the number of lines or rows.  Returns  OSSIM_UINT_NAN
    * if not initialized or file was invalid.
    */
   ossim_uint32 getNumberOfLines() const;
   
   /**
    * @return Returns the number of samples or columns. Returns
    * OSSIM_UINT_NAN if not initialized or file was invalid.
    */
   ossim_uint32 getNumberOfSamples() const;

   /**
    *  Populates the keyword list with image geometry information.  This
    *  method is used to relay projection information to users.
    *
    *  @param kwl Keyword list that will be initialized with geometry info.
    *  Returns true if geometry info is present, false if not.
    *
    *  @param prefix The prefix is added to the resulting keyword list
    *  keywords.  So is you if you pass in a prefix of "image01.", the
    *  keyword ul_lat will be like:  "image01.ul_lat:  -40.00000"
    */
   virtual bool getImageGeometry(ossimKeywordlist& kwl, const char* prefix=0);

   /**
    * @brief Gets the projection for srtm cell.
    * @return  The projection for srtm cell.
    */
   ossimRefPtr<ossimProjection> getProjection() const;

   /**
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   /**
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /**
    * @return Returns the Southwest latitude in decimal degrees. Returns
    * OSSIM_DBL_NAN if not initialized or file was invalid.
    */
   ossim_float64 getSouthwestLatitude() const;

   /**
    * @return Returns the Southwest longitude in decimal degrees.  Returns
    * OSSIM_DBL_NAN if not initialized or file was invalid.
    */
   ossim_float64 getSouthwestLongitude() const;
   
   /**
    * @return Returns the latitude spacing in decimal degrees. Returns
    * OSSIM_DBL_NAN if not initialized or file was invalid.
    */
   ossim_float64 getLatitudeSpacing() const;

   /**
    * @return Returns the longitude spacing in decimal degrees.  Returns
    * OSSIM_DBL_NAN if not initialized or file was invalid.
    */
   ossim_float64 getLongitudeSpacing() const;

   /**
    * @return Returns the minimum pixel value.  This is also
    * the minimum height above sea level.  Returns
    * OSSIM_SSHORT_NAN if not initialized or file was invalid.
    */
   ossim_float64 getMinPixelValue() const;

   /**
    * @return Returns the maximum pixel value.  This is also
    * the minimum height above sea level.  Returns
    * OSSIM_SSHORT_NAN if not initialized or file was invalid.
    */
   ossim_float64 getMaxPixelValue() const;

   ossimScalarType getScalarType()const;
   /**
    * Prints data members for debug.
    * 
    * @return std::ostream&
    */
   virtual std::ostream& print(std::ostream& out) const;

private:

   /**
    * This is a loadState called by setFilename that does not reset "theFile".
    */
   bool loadOmd(const ossimKeywordlist& kwl, const char* prefix=0);

   /** Sets all data members to an unitialized state. */
   void clear();

   /**
    * Sets corner points from "theFile".
    *
    * @return true on success, false on error.
    */
   bool setCornerPoints();
   
   /**
    * Sets lines and samples from "theFile" size.
    *
    * @return true on success, false on error.
    */
   bool setSize();

   /**
    * Scans file for min and max pixels.
    *
    * @retun true on success, false on error.
    */
   bool computeMinMax();
  
   ossimFilename theFile;
   ossim_uint32  theNumberOfLines;
   ossim_uint32  theNumberOfSamples;
   ossim_float64 theSouthwestLatitude;  //< decimal degrees
   ossim_float64 theSouthwestLongitude; //< decimal degrees
   ossim_float64 theLatSpacing;         //< decimal degrees
   ossim_float64 theLonSpacing;         //< decimal degrees
   ossim_float64 theMinPixelValue;      //< meters
   ossim_float64 theMaxPixelValue;      //< meters

   ossimScalarType theScalarType;

   mutable ossimRefPtr<ossimIFStream> theFileStream;
   template <class T>
   bool computeMinMaxTemplate(T dummy,
                              double defaultNull);
};

#endif /* #ifndef ossimSrtmSupportData */
