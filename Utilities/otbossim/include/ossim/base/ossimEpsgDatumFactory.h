//*************************************************************************************************
//                          OSSIM -- Open Source Software Image Map
//
// License:  See top level LICENSE.txt file.
//
// Author: Oscar Kramer
//
//*************************************************************************************************
//  $Id$

#ifndef ossimEpsgDatumFactory_HEADER
#define ossimEpsgDatumFactory_HEADER 1

#include <ossim/base/ossimDatumFactoryInterface.h>
#include <map>
#include <string>

class ossimDatum;
class ossimString;

//*************************************************************************************************
//! Class for creating datum instances from EPSG codes. 
//! Presently only a small subset of EPSG datums are supported. These are hardcoded here or,
//! alternatively, the EPSG code is mapped to the OSSIM/Geotrans code and ossimDatumFactory is
//! used to produce the final datum. 
//! 
//! TO DO:
//! Eventually we should have a disk-file based database lookup of datums with associated parameters
//! similar to the EPSG projection factory and database.
//*************************************************************************************************
class OSSIM_DLL ossimEpsgDatumFactory : public ossimDatumFactoryInterface
{
public:
   //! Singleton implementation.
   static ossimEpsgDatumFactory* instance();

   //! Destructor
   virtual ~ossimEpsgDatumFactory();

   //! Creates a datum instance given an EPSG spec in the form "EPSG:<datum_code>".
   virtual const ossimDatum* create(const ossimString &epsg_spec)const;

   //! Creates a datum instance given a KWL and prefix
   virtual const ossimDatum* create(const ossimKeywordlist& kwl, const char *prefix=0)const;

   //! Returns list of all datums supported, in the form of "EPSG:<code>, <datum_name>".
   virtual void getList(std::vector<ossimString>& list)const;
   
   //! Specific to this factory only. Creates a datum instance given an EPSG integer code.
   const ossimDatum* create(ossim_uint32 epsg_code)const;

   //! Specific to this factory only. Given an alpha-code, returns equivalent EPSG datum code or
   //! 0 if not found.
   ossim_uint32 findEpsgCode(const ossimString& alpha_code) const;

   //! Specific to this factory only. Given an EPSG, returns equivalent alpha-code datum code or
   //! empty string if not found.
   ossimString findAlphaCode(ossim_uint32 epsg_code) const;

protected:
   //! Hidden constructors for singleton implementation.
   ossimEpsgDatumFactory();

   //! Presently implemented as a mapping from EPSG code to OSSIM/Geotrans alpha-codes found in 
   //! ossimDatumFactory. Eventually should be independent (and deprecate) ossimDatumFactory.
   std::map<ossim_uint32, std::string> m_epsgToAlphaMap;
   
   static ossimEpsgDatumFactory* m_instance;
};
#endif
