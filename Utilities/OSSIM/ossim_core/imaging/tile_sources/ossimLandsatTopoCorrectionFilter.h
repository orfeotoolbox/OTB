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
// Author: Garrett Potts (gpotts@imagelinks)
//
//*************************************************************************
// $Id: ossimLandsatTopoCorrectionFilter.h,v 1.5 2004/04/09 19:11:34 gpotts Exp $
#ifndef ossimLandsatTopoCorrectionFilter_HEADER
#define ossimLandsatTopoCorrectionFilter_HEADER
#include <base/data_types/ossimFilename.h>
#include <imaging/tile_sources/ossimTopographicCorrectionFilter.h>

class OSSIMDLLEXPORT ossimLandsatTopoCorrectionFilter : public ossimTopographicCorrectionFilter
{
public:
   ossimLandsatTopoCorrectionFilter();
   virtual ~ossimLandsatTopoCorrectionFilter();
   ossimLandsatTopoCorrectionFilter(ossimImageSource* colorSource,
                                    ossimImageSource* elevSource,
                                    const ossimFilename& landsatFileName=ossimFilename(""));
   virtual void initialize();
   virtual void setLandsatHeader(const ossimFilename& header);
   virtual ossimFilename getLandsatHeader()const
      {
         return theLandsatHeader;
      }
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
protected:
   ossimFilename theLandsatHeader;
   ossimFilename findLandsatHeader();
//   virtual void computeC();
TYPE_DATA
};
#endif
