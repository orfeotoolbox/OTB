//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimGridRemapEngineFactory
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGridRemapEngineFactory.h 9968 2006-11-29 14:01:53Z gpotts $

#ifndef ossimGridRemapEngineFactory_HEADER
#define ossimGridRemapEngineFactory_HEADER

class ossimGridRemapEngine;

/*!****************************************************************************
 *
 * CLASS:  ossimGridRemapEngineFactory (SIMPLETON)
 *
 *****************************************************************************/
class ossimGridRemapEngineFactory
{
public:
   /*!
    * Attempts to create an instance of the Product given a Specifier. Returns
    * successfully constructed product or NULL.
    */
   static ossimGridRemapEngine* create(const char* spec);
   
protected:
   ossimGridRemapEngineFactory() {}
};

#endif
