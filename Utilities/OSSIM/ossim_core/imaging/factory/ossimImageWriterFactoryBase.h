//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
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
// Author:  Garrett Potts (gpotts@imagelinks.com)
//
//*******************************************************************
//  $Id: ossimImageWriterFactoryBase.h,v 1.7 2005/05/20 20:25:19 dburken Exp $
#ifndef ossimImageWriterFactoryBase_HEADER
#define ossimImageWriterFactoryBase_HEADER
#include "base/factory/ossimObjectFactory.h"
#include "base/data_types/ossimString.h"

#include <vector>
class ossimImageFileWriter;
class ossimKeywordlist;

class OSSIM_DLL ossimImageWriterFactoryBase : public ossimObjectFactory
{   
public:
   virtual ~ossimImageWriterFactoryBase() {}
   virtual ossimImageFileWriter *createWriterFromExtension(const ossimString& fileExtension)const=0;
   virtual ossimImageFileWriter* createWriter(const ossimKeywordlist& kwl,
                                          const char *prefix=0)const=0;
   virtual ossimImageFileWriter* createWriter(const ossimString& typeName)const=0;

   virtual void getExtensions(std::vector<ossimString>& result)const=0;
   
   /*!
    * getImageTypeList.  This is the actual image type name.  So for
    * example, ossimTiffWriter has several image types.  Some of these
    * include tiff_tiled, tiff_tiled_band_separate ... etc.  The ossimGdalWriter
    * may include gdal_imagine_hfa,gdal_nitf_rgb_band_separate, ... etc
    * A writer should be able to be instantiated by this name as well as a class name
    */
   virtual void getImageTypeList(std::vector<ossimString>& imageTypeList)const=0;

TYPE_DATA
};

#endif
