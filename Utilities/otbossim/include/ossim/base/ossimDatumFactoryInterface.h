//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file
//
// Author:  David Burken
//
// Description: Interface class for datum factories.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimDatumFactoryInterface_HEADER
#define ossimDatumFactoryInterface_HEADER

#include <ossim/base/ossimConstants.h> /* for OSSIM_DLL macro */
#include <vector>

// Forward class declarations:
class ossimString;
class ossimDatum;
class ossimKeywordlist;

class OSSIM_DLL ossimDatumFactoryInterface
{
public:
   
   /** default constructor */
   ossimDatumFactoryInterface(){}
   
   /** virtual destructor */
   virtual ~ossimDatumFactoryInterface(){}

   /**
   * Pure virtual create method.
   *
   * @return const pointer to a datum.
   */
   virtual const ossimDatum* create(const ossimString& code) const = 0;
   virtual const ossimDatum* create(const ossimKeywordlist& kwl, const char *prefix=0) const = 0;

   /**
    * Pure virtual getList method.
    *
    * @param list The list to add to.
    */
   virtual void getList(std::vector<ossimString>& list) const = 0;
};

#endif /* #ifndef ossimDatumFactoryInterface_HEADER */
