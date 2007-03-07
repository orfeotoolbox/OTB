//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  See top level LICENSE.txt file.
//
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageWriter.h,v 1.19 2006/01/03 15:43:40 gpotts Exp $

#ifndef ossimImageWriter_HEADER
#define ossimImageWriter_HEADER

#include <base/common/ossimConstants.h>
#include <base/common/ossimOutputSource.h>
#include <base/common/ossimCommon.h>
#include <base/data_types/ossimIrect.h>

class ossimImage;
class ossimImageSource;
class ossimImageWriter;
class ossimImageWriterFactory;
class ossimKeywordlist;
class ossimFilename;

#include <imaging/ossimImageSourceSequencer.h>


class OSSIM_DLL ossimImageWriter :  public ossimOutputSource
{   
public:
   ossimImageWriter(ossimObject* owner=NULL);
   
   ossimImageWriter(ossimObject* owner,
                    ossim_uint32 numberOfInputs,
                    ossim_uint32 numberOfOutputs,
                    bool inputListIsFixed,
                    bool outputListIsFixed);
   
   virtual ~ossimImageWriter();

   /**
    * @param aRect the rectangle to write.
    * 
    * @return true on success, false if the rectangle is invalid.
    */
   virtual bool setViewingRect(const ossimIrect &aRect);

   /**
    * @return Returns theAreaOfInterest.
    */
   virtual ossimIrect getAreaOfInterest() const;
 
   virtual void setAreaOfInterest(const ossimIrect& inputAreaOfInterest);

   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;

   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);
      
protected:

   ossimIrect theAreaOfInterest;
TYPE_DATA
};

#endif
