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
// Author: Garrett Potts
//
//*************************************************************************
// $Id: ossimImageSourceHistogramFilter.h,v 1.7 2004/05/19 17:53:29 dburken Exp $
#ifndef ossimImageSourceHistogramFilter_HEADER
#define ossimImageSourceHistogramFilter_HEADER
#include <imaging/tile_sources/ossimImageSourceFilter.h>
#include <base/data_types/ossimFilename.h>
class ossimMultiResLevelHistogram;

class OSSIMDLLEXPORT ossimImageSourceHistogramFilter : public ossimImageSourceFilter
{
public:
   ossimImageSourceHistogramFilter();
   ossimImageSourceHistogramFilter(ossimImageSource* inputSource,
                        ossimMultiResLevelHistogram* histogram);
   virtual ~ossimImageSourceHistogramFilter();
   virtual void setHistogram(ossimMultiResLevelHistogram* histogram);
   virtual bool setHistogram(const ossimFilename& filename);
   virtual const ossimFilename& getHistogramFilename()const;
   virtual ossimMultiResLevelHistogram* getHistogram();
   virtual const ossimMultiResLevelHistogram* getHistogram()const;
   
   bool canConnectMyInputTo(ossim_int32 inputIndex,
                            const ossimConnectableObject* object)const;

   virtual void connectInputEvent(ossimConnectionEvent& event);
   
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix = NULL);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix = NULL)const;

   virtual std::ostream& print(std::ostream& out) const;

protected:
   long                         theCurrentResLevel;

private:
   ossimMultiResLevelHistogram* theHistogram;
   ossimFilename                theFilename;
   
TYPE_DATA
};

#endif /* #ifndef ossimImageSourceHistogramFilter_HEADER */
