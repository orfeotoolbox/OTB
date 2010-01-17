//----------------------------------------------------------------------------
// FILE: ossimElevSourceFactory.cc
//
// Copyright (C) 2002 ImageLinks, Inc.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Class declaration for ossimElevSourceFactory.
//
// This is the base class interface for elevation source factories which main
// function is to return an elevation source that covers the ground point
// passed to the "getElevSource" method.
//
// Note the caller of "getElevSource" is responsible for managing the memory
// new'd from the method.
// 
//----------------------------------------------------------------------------
// $Id: ossimElevSourceFactory.h 13269 2008-07-25 14:27:36Z dburken $

#ifndef ossimElevSourceFactory_HEADER
#define ossimElevSourceFactory_HEADER

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimFilename.h>

class ossimElevSource;
class ossimGpt;

/*!****************************************************************************
 * CLASS:  ossimElevSourceFactory
 *
 *****************************************************************************/
class OSSIM_DLL ossimElevSourceFactory : public ossimObject
{
public:

   /** default constructor */
   ossimElevSourceFactory();

   /** virtual destructor */
   virtual ~ossimElevSourceFactory();

   /**
    *  Pure virtual method.
    *  return an elevation source pointer that has elevation coverage for the
    *  ground point passed in.
    *  Note the caller of this method is responsible for managing the memory
    *  allocated.
    */
   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt)const=0;

   /**
    * @return The directory the factory returns data from.
    */
   virtual ossimFilename getDirectory() const;

   /**
    * @param directory The directory to return data from.
    */
   virtual void setDirectory(const ossimFilename& directory);

protected:
   ossimFilename theDirectory;
   
TYPE_DATA
}; 

#endif /* End of "#ifndef ossimElevSourceFactory_HEADER" */
