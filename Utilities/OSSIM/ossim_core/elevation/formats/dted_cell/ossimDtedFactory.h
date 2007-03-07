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
// $Id: ossimDtedFactory.h,v 1.6 2005/11/18 20:24:49 gpotts Exp $
#ifndef ossimDtedFactory_HEADER
#define ossimDtedFactory_HEADER

#include <elevation/ossimElevSourceFactory.h>

/** CLASS:  ossimDtedFactory */
class OSSIM_DLL ossimDtedFactory : public ossimElevSourceFactory
{
public:

   ossimDtedFactory();
   ossimDtedFactory(const ossimFilename& dir);
      
   virtual ~ossimDtedFactory();

   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt) const;
   virtual void createIndex();
protected:

TYPE_DATA
};

#endif /* End of "#ifndef ossimDtedFactory_HEADER" */
