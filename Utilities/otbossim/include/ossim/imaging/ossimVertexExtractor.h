//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: David Burken (dburken@imagelinks.com)
//
//*************************************************************************
// $Id: ossimVertexExtractor.h 17207 2010-04-25 23:21:14Z dburken $

#ifndef ossimVertexExtractor_HEADER
#define ossimVertexExtractor_HEADER

#include <fstream>

#include <ossim/base/ossimOutputSource.h>
#include <ossim/base/ossimProcessInterface.h>
#include <ossim/base/ossimProcessProgressEvent.h>
#include <ossim/base/ossimIrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/imaging/ossimImageSource.h>

class ossimImageSource;


//! Class ossimVertexExtractor

/*!
 *  Class designed to scan the area of interest and detect the valid vertices
 *  of non null image data.
 */
class OSSIMDLLEXPORT ossimVertexExtractor : public ossimOutputSource,
    public ossimProcessInterface
{
public:
   ossimVertexExtractor(ossimImageSource* inputSource=NULL);


  virtual ossimObject* getObject()
  {
    return this;
  }
  virtual const ossimObject* getObject()const
  {
    return this;
  }
   /*!
    *  Sets the area of interest in the source to extract the vertices from.
    *  @param rect ossimIrec representing source area of interest.
    */
   void setAreaOfInterest(const ossimIrect& rect);

   /*!
    *  Sets the name of the output keyword list file which the vertices will
    *  be dumped to.
    *  @param filename ossimFilename representing the output file.
    *
    *  Note:
    *  - If "theFileStream" is open it will be closed.
    */
   virtual void setOutputName(const ossimString& filename);

   /*!
    *  Returns true if "theFileStream" is open, false if not.
    */
   virtual bool isOpen() const;

   /*!
    * Opens "theFilename" for output.
    * Returns true on success, false on error.
    * Notes:
    * - If the file was previously open then it will close it and re-opens the
    *   file.
    * - Returns false if "theFilename" has not been set.
    */
   virtual bool open();

   /*!
    *  Closes "theFileStream".
    */
   virtual void close();

   /*!
    *  Calls protected methods scanForEdges, extractVertices, and
    *  writeVertices.  Will error out if "theFilename" or "theAreaOfInterest
    *  has not been set or "theFilename" could not be opened.
    */
   virtual bool execute();

   virtual ossimObject* getObjectInterface() { return this; }

   virtual ossimListenerManager* getListenerManagerInterface()
      {
         return this;
      }
   
   
   virtual void setPercentComplete(double percentComplete)
      {
         ossimProcessInterface::setPercentComplete(percentComplete);
         ossimProcessProgressEvent event(this,
                                         percentComplete);
         fireEvent(event);      
      }

   bool canConnectMyInputTo(ossim_int32 /* inputIndex */,
                            const ossimConnectableObject* object)const
      {
         
         return (object&& PTR_CAST(ossimImageSource, object));
      }

   vector<ossimIpt> getVertices() { return theVertice; }
   
protected:
   virtual ~ossimVertexExtractor();
   /*!
    *  Walks each line from left and right side detecting first non null pixel.
    *  Returns true on success, false on error.
    */
   bool scanForEdges();

   /*!
    *  Extracts the vertices of the source.  Uses "theLeftEdge" and
    *  "theRightEdge" data members.
    *  Returns true on success, false on error.
    */
   bool extractVertices();

   template<class T1>
   void getMinAndIndex(T1* start, T1* end,
                       T1& minValue, ossim_int32& offsetFromStart);
   template<class T2>
   void getMaxAndIndex(T2* start, T2* end,
                       T2& maxValue, ossim_int32& offsetFromStart);
   
   /*!
    *  Writes the result to the output file (theFilename).
    *  Returns true on success, false on error.
    */

   
   bool writeVertices();

   ossimIrect       theAreaOfInterest;
   ossimFilename    theFilename;
   std::ofstream    theFileStream;
   vector<ossimIpt> theVertice;
   ossim_int32*     theLeftEdge;
   ossim_int32*     theRightEdge;

   //! Disallow copy constructor and operator=
   ossimVertexExtractor(const ossimVertexExtractor&) {}
   const ossimVertexExtractor& operator=(const ossimVertexExtractor& rhs)
      {return rhs;}

TYPE_DATA
};

#endif
