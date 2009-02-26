//----------------------------------------------------------------------------
// Copyright (c) 2005, David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
// 
// Shuttle Radar Topography Mission (SRTM) factory to return an
// ossimSrtmElevSource given a ground point.
//
//----------------------------------------------------------------------------
// $Id: ossimSrtmFactory.h 13269 2008-07-25 14:27:36Z dburken $
#ifndef ossimSrtmFactory_HEADER
#define ossimSrtmFactory_HEADER

#include <ossim/elevation/ossimElevSourceFactory.h>

/**
 * @class ossimSrtmFactory Used by the elevation manager, this class returns
 * an ossimSrtmElevSource given a ground point and some directory with srtm
 * files in it.
 */
class OSSIM_DLL ossimSrtmFactory : public ossimElevSourceFactory
{
public:

   /** default constructor */
   ossimSrtmFactory();

   /** Constructor that takes a directory name. */
   ossimSrtmFactory(const ossimFilename& dir);

   /** destructor */
   virtual ~ossimSrtmFactory();

   /**
    * Open the appropriate ossimSrtmElevSource that covers given a
    * ground point.
    *
    * @param gpt Ground point that an elevation source is need for.
    *
    * @return Returns a pointer to an ossimElevSource if an srtm file is found
    * that can cover the ground point.  Returns NULL if no cell is found
    * for the point.
    */
   virtual ossimElevSource* getNewElevSource(const ossimGpt& gpt) const;
   
protected:

TYPE_DATA
};

#endif /* End of "#ifndef ossimSrtmFactory_HEADER" */
