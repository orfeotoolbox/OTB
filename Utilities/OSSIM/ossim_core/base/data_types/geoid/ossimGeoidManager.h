//*****************************************************************************
// FILE: ossimGeoidManager.h
//
// Copyright (C) 2001 ImageLinks, Inc.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// AUTHOR: Oscar Kramer (okramer@imagelinks.com)
//
// DESCRIPTION: Contains declaration of class ossimGeoidManager. Maintains
//   a list of geoids.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimGeoidManager.h,v 1.7 2005/11/10 14:15:33 gpotts Exp $

#ifndef ossimGeoidManager_HEADER
#define ossimGeoidManager_HEADER

#include <vector>
#include <base/common/ossimRefPtr.h>
#include "ossimGeoid.h"

/*!****************************************************************************
 *
 * CLASS: ossimGeoidManager 
 *
 *****************************************************************************/
class OSSIMDLLEXPORT ossimGeoidManager : public ossimGeoid
{
public:


   virtual ~ossimGeoidManager();
      
   /*!
    * Implements singelton pattern:
    */
   static ossimGeoidManager* instance();

   
   /*!
    * Permits initialization of geoids from directory name. Should never be
    * called since called on specific geoid types:
    */
   virtual bool open(const ossimFilename& dir, ossimByteOrder byteOrder);

   /*!
    *  Returns the offset from the ellipsoid to the geoid.
    *  Returns OSSIM_DBL_NAN if grid does not contain the point.
    */
   virtual double offsetFromEllipsoid(const ossimGpt& gpt) const;

   /*!
    * Method to save the state of the object to a keyword list.
    * Return true if ok or false on error. DO NOTHING
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const { return true; }

   /*!
    * Method to the load (recreate) the state of the object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   /*!
    * Permits adding additional geoids to the list: 
    */
   virtual void addGeoid(ossimRefPtr<ossimGeoid> geoid);

   const ossimRefPtr<ossimGeoid> findGeoidByShortName(const ossimString& shortName,
                                                      bool caseSensitive=true)const;
private:
   /*!
    *  Private constructor.  Use "instance" method.
    */
   ossimGeoidManager();

   static ossimGeoidManager* theInstance;
   mutable std::vector< ossimRefPtr<ossimGeoid> > theGeoidList;

   TYPE_DATA
};

#endif
