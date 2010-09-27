//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Mingjie Su
//
// Description: Utility class to encapsulate parsing projection info in the xml file.
// 
//----------------------------------------------------------------------------
// $Id: ossimFgdcXmlDoc.h 1204 2010-08-02 18:48:46Z david.burken $
#ifndef ossimFgdcXmlDoc_HEADER
#define ossimFgdcXmlDoc_HEADER 1

#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimDrect.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimXmlDocument.h>
#include <ossim/projection/ossimProjection.h>

/** @brief Class for FGDC XML doc parsing. */
class OSSIMDLLEXPORT ossimFgdcXmlDoc
{
public:
   /** @brief default constructor */
   ossimFgdcXmlDoc(const ossimFilename& xmlFileName);
   
   /** @brief destructor */
   ~ossimFgdcXmlDoc();
   
   ossimProjection* getProjection();
   
private:
   
   bool isOpen();
   
   ossimString getGeoCsn();
   
   ossimString getProjCsn();
   
   ossimString getUnits();
   
   double getXRes();
   
   double getYRes();
   
   ossim_uint32 getNumberOfSamples();
   
   ossim_uint32 getNumberOfLines();
   
   ossimDrect getBoundingBox();
   
   ossimFilename                 m_xmlFilename;
   ossimRefPtr<ossimXmlDocument> m_xmlDocument;
   ossimRefPtr<ossimProjection>  m_projection;
};

#endif /* matches: #ifndef ossimFgdcXmlDoc */
