//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// DESCRIPTION:
//   Contains declaration of class ossimElevCellHandlerFactory. This factory
//   class instantiates the proper ossimElevCellHandler given a file name.
//
// SOFTWARE HISTORY:
//>
//   26Apr2001  Oscar Kramer (okramer@imagelinks.com)
//              Initial coding.
//<
//*****************************************************************************

#ifndef ossimElevCellHandlerFactory_HEADER
#define ossimElevCellHandlerFactory_HEADER

#include <list>
#include <ossim/base/ossimFactoryBaseTemplate.h>

class ossimElevCellHandler;
class ossimGpt;

/*!****************************************************************************
 *
 * CLASS:  ossimElevCellHandlerFactory
 *
 *****************************************************************************/
class ossimElevCellHandlerFactory :
   public ossimFactoryBase <ossimElevCellHandler>
{
 public:
   static ossimElevCellHandlerFactory* instance();
   
   virtual ossimElevCellHandler* create(const ossimString&) const;
   virtual ossimElevCellHandler* create(const ossimKeywordlist& kwl,
                                        const char* prefix) const;
   
   virtual std::list<ossimString> getList() const;
   
 protected:
   ossimElevCellHandlerFactory();

   static ossimElevCellHandlerFactory*  theInstance;
};

#endif
