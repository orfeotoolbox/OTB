//*****************************************************************************
// FILE: ossimNitfMapModel.h
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
// AUTHOR: Doug Shibla (dshibla@imagelinks.com)
//
// DESCRIPTION:
//
//*****************************************************************************
//  $Id: ossimNitfMapModel.h,v 1.4 2005/05/12 19:55:13 gpotts Exp $

#ifndef ossimNitfMapModel_HEADER
#define ossimNitfMapModel_HEADER

#include <projections/sensor_modeling/ossimSensorModel.h>
#include <base/data_types/ossimFilename.h>
#include <base/data_types/ossimGpt.h>
#include <base/data_types/ossimDpt.h>
#include <iostream>

class ossimString;
class ossimMapProjection;

class OSSIMDLLEXPORT ossimNitfMapModel : public ossimSensorModel
{
public:
   /*!
    * CONSTRUCTORS:
    */
   ossimNitfMapModel();
   ossimNitfMapModel(const ossimFilename& init_file);
   ossimNitfMapModel(const ossimKeywordlist& geom_kwl);
   
   virtual ~ossimNitfMapModel();

   /*!
    * Returns pointer to a new instance, copy of this.
    */
   virtual ossimObject* dup() const { return 0; } // TBR
   
   /*!
    * Extends base-class implementation. Dumps contents of object to ostream.
    */
   virtual std::ostream& print(std::ostream& out) const;
   
   /*!
    * Fulfills ossimObject base-class pure virtuals. Loads and saves geometry
    * KWL files. Returns true if successful.
    */
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0) const;
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
   
   /*!
    * Writes a template of geom keywords processed by loadState and saveState
    * to output stream.
    */
   static void writeGeomTemplate(ostream& os);
   
   //***
   // Overrides base class pure virtual.
   //***
   virtual void lineSampleHeightToWorld(const ossimDpt& image_point,
                                        const double&   heightEllipsoid,
                                        ossimGpt&       worldPoint) const;
   
protected:
   
TYPE_DATA
};

#endif
