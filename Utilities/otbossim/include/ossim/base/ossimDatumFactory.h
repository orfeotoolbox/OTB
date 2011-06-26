//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts
//
// Description:
//
// This is the DatumFactory class.  You give it a code and it will
// construct a Datum class.  It returns NULL if no code was found.
//*******************************************************************
//  $Id: ossimDatumFactory.h 19682 2011-05-31 14:21:20Z dburken $

#ifndef ossimDatumFactory_HEADER
#define ossimDatumFactory_HEADER 1

#include <ossim/base/ossimDatumFactoryInterface.h>
#include <ossim/base/ossimEllipsoidFactory.h>
#include <map>
#include <string>
#include <vector>

class ossimFilename;
class ossimDatum;
class ossimWgs84Datum;
class ossimWgs72Datum;
class ossimString;

class OSSIM_DLL ossimDatumFactory : public ossimDatumFactoryInterface
{
public:

   /** virtual destructor */
   virtual ~ossimDatumFactory();

   /**
    * create method
    *
    * Implements pure virtual ossimDatumFactoryInterface::create.
    *
    * @return const pointer to a datum.
    */
   virtual const ossimDatum* create(const ossimString &code)const;
   virtual const ossimDatum* create(const ossimKeywordlist& kwl, const char *prefix=0)const;

   const ossimDatum* create(const ossimDatum* copy) const;

   static ossimDatumFactory* instance();
   const ossimDatum* wgs84()const{return theWgs84Datum;}
   const ossimDatum* wgs72()const{return theWgs72Datum;}
   std::vector<ossimString> getList()const;
   virtual void getList(std::vector<ossimString>& list)const;
   void writeCStructure(const ossimFilename& file);
protected:
   static ossimDatumFactory*       theInstance;
   const ossimDatum*               theWgs84Datum;
   const ossimDatum*               theWgs72Datum;
   std::map<std::string, const ossimDatum*> theDatumTable;
   

   ossimDatumFactory()
      :theWgs84Datum(0),
       theWgs72Datum(0)
       {}

   void deleteAll();
   void initializeDefaults();
};
#endif
