//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Utility class to encapsulate parsing projection info in the xml file.
// 
//----------------------------------------------------------------------------
// $Id: ossimFgdcXmlDoc.h 2063 2011-01-19 19:38:12Z ming.su $
#ifndef ossimFgdcXmlDoc_HEADER
#define ossimFgdcXmlDoc_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/projection/ossimProjection.h>

class ossimDatum;
class ossimDrect;

/** @brief Class for FGDC XML doc parsing. */
class OSSIMDLLEXPORT ossimFgdcXmlDoc
{
public:
   /** @brief default constructor */
   ossimFgdcXmlDoc();
   
   /** @brief destructor */
   ~ossimFgdcXmlDoc();

   /**
    * @brief Open method.
    * @return true on success, false on error.
    */
   bool open(const ossimFilename& xmlFileName);

   /** @return true if m_xmlDocument is open; false if not. */
   bool isOpen() const;

   /** @brief close method. */
   void close();

   /** @brief Gets projection from document. */
   ossimRefPtr<ossimProjection> getProjection();

   /** @return Unit type. Note meters is the default if not found in doc. */
   ossimUnitType getUnitType() const;

   /**
    * @return text for path "/metadata/spref/horizsys/cordsysn/geogcsn"
    */
   bool getGeoCsn(ossimString& s) const;

   /**
    * @return text for path "/metadata/spref/horizsys/cordsysn/projcsn"
    */
   bool getProjCsn(ossimString& s) const;

   /**
    * @brief Get the Altitude Distance Units
    *
    * Path: "/metadata/spref/vertdef/altsys/altunits"
    *
    * Domain: "meters" "feet"
    * 
    * @return Text for path.
    */
   bool getAltitudeDistantUnits(ossimString& s) const;

   /**
    * @brief Get the Grid Coordinate system
    *
    * Path: "/metadata/spref/horizsys/planar/gridsys/gridsysn"
    *
    * @return Text for path.
    */
   bool getGridCoordinateSystem(ossimString& s) const;

   /**
    * @brief Gets projection from Grid Coordinate system node.
    *
    * Throws ossimException on error.
    *
    * @return Refptr with projection pointer.  Underlying pointer may be null.
    */
   ossimRefPtr<ossimProjection> getGridCoordSysProjection();

   /**
    * @brief Get the Grid Coordinate system
    *
    * Path: "/metadata/spref/horizsys/planar/gridsys/gridsysn"
    *
    * @return Text for path.
    */
   bool getHorizontalDatum(ossimString& s) const;

   /**
    * @brief Get the Planar Distant Units
    * 
    * Path: "/metadata/spref/horizsys/planar/planci/plandu"
    *
    * Domain: "meters" "international feet" "survey feet"
    *
    * @return Text for path.
    */
   bool getPlanarDistantUnits(ossimString& s) const;

   /**
    * @brief Get UTM false Northing.
    * 
    * Path: "/metadata/spref/horizsys/planar/gridsys/utm/transmer/fnorth"
    *
    * @return Text for path.
    */
   bool getUtmFalseNorthing(ossimString& s) const;
   
   /**
    * @brief Get UTM zone.
    * 
    * Path: "/metadata/spref/horizsys/planar/gridsys/utm/utmzone"
    *
    * Domain: 1 <= UTM Zone Number <= 60 for the northern hemisphere;
    * -60 <= UTM Zone Number <= -1 for the southern hemisphere
     *
    * @return Text for path.
    */
   bool getUtmZone(ossimString& s) const;

   /**
    * @brief Get Bands.
    * 
    * Path: "/metadata/spdoinfo/rastinfo/vrtcount"
    *
    * the maximum number of raster objects along the vertical (z) axis. 
    * For use with rectangular volumetric raster objects (voxels). 
     *
    * @return int for path.
    */
   ossim_uint32 getNumberOfBands();

    /**
    * @brief Gets path from doc and initializes string.
    *
    * This method errors if multiple nodes are found for path.
    * 
    * @param path Xml path to look for.
    * @param s String to initialize.
    * @return true on success and false if path is not found or if there
    * are more than one of path.
    */
   bool getPath(const ossimString& path, ossimString& s) const;

   bool getImageSize(ossimIpt& size) const;

private:

   bool getXRes(ossim_float64& v) const;
   
   bool getYRes(ossim_float64& v) const;

   void getBoundingBox(ossimDrect& rect) const;

   /**
    * @brief Gets path from doc and initializes string.
    * @param path Xml path to look for.
    * @param v Value to initialize.
    * @return true on success and false if path is not found or if there
    * are more than one of path.
    */
   bool getPath(const ossimString& path, ossim_float64& v) const;

   /**
    * Gets the ossimDatum from string.
    */
   const ossimDatum* createOssimDatum(const ossimString& s) const;
   
   ossimFilename                 m_xmlFilename;
   ossimRefPtr<ossimXmlDocument> m_xmlDocument;
   ossimRefPtr<ossimProjection>  m_projection;
   mutable bool                  m_boundInDegree;
};

#endif /* matches: #ifndef ossimFgdcXmlDoc */
