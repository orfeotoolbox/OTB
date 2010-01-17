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
//  $Id: ossimImageWriter.h 15798 2009-10-23 19:15:20Z gpotts $

#ifndef ossimImageWriter_HEADER
#define ossimImageWriter_HEADER

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimIrect.h>

class ossimImage;
class ossimImageSource;
class ossimImageWriter;
class ossimImageWriterFactory;
class ossimKeywordlist;
class ossimFilename;

#include <ossim/imaging/ossimImageSourceSequencer.h>


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
