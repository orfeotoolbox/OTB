//*****************************************************************************
// FILE: ossimDtedFactory.cc
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author:  David Burken
//
// Description:
//
// Class declaration for ossimDtedFactory.
// This is the base class interface for elevation cell factories.
//*****************************************************************************
// $Id: ossimDtedFactory.h 15766 2009-10-20 12:37:09Z gpotts $
#ifndef ossimDtedFactory_HEADER
#define ossimDtedFactory_HEADER

#include <ossim/elevation/ossimElevSourceFactory.h>

/** CLASS:  ossimDtedFactory */
class OSSIM_DLL ossimDtedFactory : public ossimElevSourceFactory
{
public:

   ossimDtedFactory();
   ossimDtedFactory(const ossimFilename& dir);
      

   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt) const;
   virtual void createIndex();
protected:
   virtual ~ossimDtedFactory();

TYPE_DATA
};

#endif /* End of "#ifndef ossimDtedFactory_HEADER" */
