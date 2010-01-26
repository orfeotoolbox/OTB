//*******************************************************************
// Copyright (C) 2003 Storage Area Networks, Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Kenneth Melero (kmelero@sanz.com)
//
//*******************************************************************
//  $Id: ossimGeomFileWriter.h 15766 2009-10-20 12:37:09Z gpotts $

#ifndef ossimGeomFileWriter_H
#define ossimGeomFileWriter_H

#include <ossim/imaging/ossimMetadataFileWriter.h>

/** ossimGeomFileWriter */
class OSSIMDLLEXPORT ossimGeomFileWriter : public ossimMetadataFileWriter
{
public:
   /** default constructor */
   ossimGeomFileWriter();


   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * Appends the writers image types to the "metadatatypeList".
    * 
    * @param metadatatypeList stl::vector<ossimString> list to append to.
    *
    * @note Appends to the list, does not clear it first.
    */
   virtual void getMetadatatypeList(
      std::vector<ossimString>& metadatatypeList) const;

   /**
    * Satisfies pure virtual from ossimMetadataWriter base.
    *
    * @param imageType string representing image type.
    *
    * @return true if "metadataType" is supported by writer.
    */
   virtual bool hasMetadataType(const ossimString& metadataType)const;

protected:
   /** virtual destructor */
   virtual ~ossimGeomFileWriter();

   /** Write a geometry file to "theFilename". */
   virtual bool writeFile();
   
TYPE_DATA
};

#endif /* End of #ifndef ossimGeomFileWriter_H */
