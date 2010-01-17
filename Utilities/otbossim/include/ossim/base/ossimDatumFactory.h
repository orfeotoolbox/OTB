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
//  $Id: ossimDatumFactory.h 13302 2008-07-25 19:10:39Z gpotts $

#ifndef ossimDatumFactory_HEADER
#define ossimDatumFactory_HEADER
#include <vector>
#include <map>

#include <ossim/base/ossimDatumFactoryInterface.h>
#include <ossim/base/ossimEllipsoidFactory.h>
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
   static std::map<ossimString, ossimDatum*> theDatumTable;
   

   ossimDatumFactory()
      :ossimDatumFactoryInterface(),
       theWgs84Datum(0),
       theWgs72Datum(0)
       {}
   ossimDatumFactory(const ossimDatumFactory&)
      :ossimDatumFactoryInterface(),
       theWgs84Datum(0),
       theWgs72Datum(0)
       {}
   void deleteAll();
   void initializeDefaults();
};
#endif
