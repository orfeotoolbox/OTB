//----------------------------------------------------------------------------
// Copyright (C) 2004 Intelligence Data Systems, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Matt Revelle
//          David Burken
//
// Description:
//
// Contains class declaration for ossimNitfProjectionFactory.
//
// $Id: ossimNitfProjectionFactory.h,v 1.8 2005/10/16 22:02:09 dburken Exp $
//----------------------------------------------------------------------------
#ifndef ossimNitfProjectionFactory_HEADER
#define ossimNitfProjectionFactory_HEADER

#include <vector>
#include <projections/factory/ossimProjectionFactoryBase.h>

class ossimProjection;
class ossimString;
class ossimGpt;
class ossimNitfImageHeader;
class ossimDpt;

class OSSIMDLLEXPORT ossimNitfProjectionFactory : public ossimProjectionFactoryBase
{
public:

   /**
    * METHOD: instance()
    *
    * @return  Singleton instance of this class.
    */
   static ossimNitfProjectionFactory* instance();

   /**
    * destructor
    */
   virtual ~ossimNitfProjectionFactory();

   /**
    * @param filename This filename can be an image file or
    * it can also be a ossim .geom file.  It could be other
    * forms of geom files as well.  The factories job will be to
    * determine what parser to use and return a projection if
    * successful.
    *
    * @return ossimProjection* on success or NULL if not handled.
    */
   virtual ossimProjection* createProjection(const ossimFilename& filename,
                                             ossim_uint32 entryIdx)const;
   
   /**
    * @param name This would be a projection name.
    *
    * @return ossimProjection* on success or NULL if not handled.
    */
   virtual ossimProjection* createProjection(const ossimString& name)const;

   
   /**
    * @param kwl  Keyword list to instantiate projection from.
    *
    * @return ossimProjection* on success or NULL if not handled.
    */
   virtual ossimProjection* createProjection(const ossimKeywordlist& kwl,
                                             const char* prefix = 0)const;
   /**
    * Creates an object given a type name.
    *
    * @return ossimObject on success or NULL if not handled.
    */
   virtual ossimObject* createObject(const ossimString& typeName)const;

   /**
    * Creates and object given a keyword list.
    *
    * @return ossimObject on success or NULL if not handled.
    */
   virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                     const char* prefix=0)const;
   
   /**
    * This should return the type name of all objects in all factories.
    * This is the name used to construct the objects dynamially and this
    * name must be unique.
    *
    * @param typeList List to append names to.
    */
   virtual void getTypeNameList(std::vector<ossimString>& typeList)const;

   /**
    * @note  Being depricated with getTypeNameList found in ossimObjectFactory
    */
   virtual list<ossimString> getList()const;

private:
   /**
    * @param hdr The nitf image header.
    *
    * @param coordianteSystem The coordinate system as a string.
    *
    * @param geographicLocation This should contain the four corner strings.
    *
    * @param filename The nitf file name.
    * 
    * @return Either an  ossimEquDistCylProjection or ossimBilinearProjection
    * depending upon if the corner points line up evenly.
    */
   ossimProjection* makeGeographic(const ossimNitfImageHeader* hdr,
                                   const ossimString& coordinateSysetm,
                                   const ossimFilename& filename) const;

   /**
    * @param hdr The nitf image header.
    *
    * @param coordianteSystem The coordinate system as a string.
    *
    * @param geographicLocation This should contain the four corner strings.
    *
    * @param filename The nitf file name.
    * 
    * @return ossimUtmProjection
    */
   ossimProjection* makeUtm(const ossimNitfImageHeader* hdr,
                            const ossimString& coordinateSysetm,
                            const ossimFilename& filename) const;
   
   /**
    * @param hdr The nitf image header.
    *
    * @param gpts Four geographic corner points.
    * 
    * @return ossimEquDistCylProjection* or null if a problem is found.
    */
   ossimProjection* makeEuiDistant(const ossimNitfImageHeader* hdr,
                                   const std::vector<ossimGpt>& gpts) const;
   
   /**
    * @return ossimBilinearProjection* or null if a problem is found.
    */
   ossimProjection* makeBilinear(const ossimNitfImageHeader* hdr,
                                 const std::vector<ossimGpt>& gpts) const;

   /**
    * Parses geographic stream.  This will initialize the vector of ossimGpt's
    * with the four corners.  If the string geographicLocation is not the
    * correct size the vector will be cleared.
    *
    * @param geographicLocation String containing corners.
    *
    * @param gpts Vector to initialize.
    */
   void parseGeographicString(const ossimString& geographicLocation,
                              std::vector<ossimGpt>& gpts) const;

   /**
    * Parses Decimal degree stream.  This will initialize the vector of ossimGpt's
    * with the four corners.  If the string geographicLocation is not the
    * correct size the vector will be cleared.
    *
    * @param geographicLocation String containing corners.
    *
    * @param gpts Vector to initialize.
    */
   void parseDecimalDegreesString(const ossimString& geographicLocation,
                                  std::vector<ossimGpt>& gpts) const;

   void parseUtmString(const ossimString& utmLocation,
                       ossim_uint32& zone,
                       std::vector<ossimDpt>& utmPoints)const;
   /**
    * Computes the scale in decimal degrees with scale.x being longitude,
    * y being latitude.
    *
    * @param hdr The nitf image header from the currently opened nitf file.
    *
    * @param gpts Four geographic corner points.
    *
    * @param scale Scale to initialize.
    *
    * @note scale will be set to nan's on failure.
    */
   void computeScaleInDecimalDegrees(const ossimNitfImageHeader* hdr,
                                     const std::vector<ossimGpt>& gpts,
                                     ossimDpt& scale) const;

   /**
    * Computes the scale in meters with scale.x being easting,
    * y being northing.
    *
    * @param hdr The nitf image header from the currently opened nitf file.
    *
    * @param dpts Four utm corner points.
    *
    * @param scale Scale to initialize.
    *
    * @note scale will be set to nan's on failure.
    */
   void ossimNitfProjectionFactory::computeScaleInMeters(
      const ossimNitfImageHeader* hdr,
      const std::vector<ossimDpt>& dpts,
      ossimDpt& scale) const;
      
   /**
    * @return true if corner points are parallel to each other, else false.
    */
   bool isSkewed(const std::vector<ossimGpt>& gpts) const;

   /**
    * @return true if corner points are parallel to each other, else false.
    */
   bool isSkewed(const std::vector<ossimDpt>& gpts) const;

   /**
    * @param hdr The nitf image header from the currently opened nitf file.
    * 
    * @param gpts Ground points to initialize from BLOCKA tag.  This should
    * be an empty vector.
    *
    * @param filename The nitf file name.
    * 
    * @return true if BLOCKA tag was parsed.
    */
   bool getBlockaPoints(const ossimNitfImageHeader* hdr,
                        std::vector<ossimGpt>& gpts,
                        const ossimFilename& filename) const;
   
   /**
    * Private constructor, users must go through instance() method.
    */
   ossimNitfProjectionFactory();

   /**
    * Private copy constructor to hide from use.
    */
   ossimNitfProjectionFactory(const ossimNitfProjectionFactory& obj);

   /**
    * Private operator = to hide from use.
    */
   ossimNitfProjectionFactory& operator=(const ossimNitfProjectionFactory& rhs);
   
   static ossimNitfProjectionFactory*  theInstance;

   bool isNitf(const ossimFilename& filename)const;
};

#endif /* End of "#ifndef ossimNitfProjectionFactory_HEADER" */
