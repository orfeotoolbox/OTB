//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Class declaration of Registry (singleton) for datum factories.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimDatumFactoryRegistry_HEADER
#define ossimDatumFactoryRegistry_HEADER

#include <vector>
#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <ossim/base/ossimDatumFactoryInterface.h> 
#include <OpenThreads/ReadWriteMutex>

// Forward class declarations.
class ossimDatumFactoryInterface;
class ossimString;
class ossimDatum;

class OSSIM_DLL ossimDatumFactoryRegistry : public ossimDatumFactoryInterface
{
public:
   
   /** destructor */
   ~ossimDatumFactoryRegistry();

   /**
    * instance method
    *
    * @return Point to the instance of the registry.
    */
   static ossimDatumFactoryRegistry* instance();

   /**
    * Method to add factory to registry.
    * @param factory Factory to register.
    */
   void registerFactory(ossimDatumFactoryInterface* factory);
   
   /**
    * create method
    *
    * Implements pure virtual ossimDatumFactoryInterface::create.
    *
    * @return const pointer to a datum.
    */
   virtual const ossimDatum* create(const ossimString& code)const;
   virtual const ossimDatum* create(const ossimKeywordlist& kwl, const char *prefix=0) const;

   /**
    * getList method to return a combined list of all datums from registered
    * datum factories.
    *
    * @param list The list to add to.
    */
   virtual void getList(std::vector<ossimString>& list) const;
   
protected:

   /** hidden from use default constructor */
   ossimDatumFactoryRegistry();

   /** Single static instance of this class. */
   static ossimDatumFactoryRegistry* theInstance;

	mutable OpenThreads::ReadWriteMutex theFactoryListMutex;
   std::vector<ossimDatumFactoryInterface*> theFactoryList;
};

#endif /* #ifndef ossimDatumFactoryRegistry_HEADER */
