//*****************************************************************************
// FILE: ossimIkonosRpcModel.h
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
// DESCRIPTION: Contains declaration of class ossimIkonosRpcModel. This 
//    derived class implements the capability of reading Ikonos RPC support
//    data.
//
// LIMITATIONS: None.
//
//*****************************************************************************
//  $Id: ossimIkonosRpcModel.h,v 1.2 2003/12/31 13:25:07 gpotts Exp $

#ifndef ossimIkonosRpcModel_HEADER
#define ossimIkonosRpcModel_HEADER

#include "ossimRpcModel.h"

class ossimFilename;

/*!****************************************************************************
 *
 * CLASS:  ossimIkonosRpcModel
 *
 *****************************************************************************/
class ossimIkonosRpcModel : public ossimRpcModel
{
public:
   ossimIkonosRpcModel(const ossimFilename& geom_file);

   ossimIkonosRpcModel(const ossimFilename& metadata,
                       const ossimFilename& rpcdata);

  virtual bool saveState(ossimKeywordlist& kwl,
			 const char* prefix=0)const;
   /*!
    * STATIC METHOD: writeGeomTemplate(ostream)
    * Writes a template of an ossimIkonosRpcModel geometry file.
    */
   static void writeGeomTemplate(ostream& os);
   
protected:
   void finishConstruction();
   void parseMetaData(const ossimFilename& metadata);
   void parseRpcData (const ossimFilename& rpcdata);

   TYPE_DATA
};

#endif
