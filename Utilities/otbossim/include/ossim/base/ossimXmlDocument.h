//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Oscar Kramer <okramer@imagelinks.com> (ossim port by D. Burken)
//
// Description:  
//
// Contains declaration of class ossimXmlDocument. This class provides read-only
// parsing and accessing of an XML document file.
// 
//*****************************************************************************
// $Id: ossimXmlDocument.h 21579 2012-08-31 14:24:11Z dburken $
#ifndef ossimXmlDocument_HEADER
#define ossimXmlDocument_HEADER 1

#include <ossim/base/ossimObject.h>
#include <ossim/base/ossimErrorStatusInterface.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include <vector>

class OSSIMDLLEXPORT ossimXmlDocument :
   public ossimObject, public ossimErrorStatusInterface
{
public:
   ossimXmlDocument(const ossimFilename& xmlFileName="");
   ossimXmlDocument(const ossimXmlDocument& src);
   virtual ossimObject* dup()const
   {
      return new ossimXmlDocument(*this);
   }
   virtual ~ossimXmlDocument();

   bool write(const ossimFilename& file);
   bool openFile(const ossimFilename& filename);
   bool read(std::istream& in);
   /**
    * Appends any matching nodes to the list supplied (should be empty):
    */
   void findNodes(const ossimString& xpath,
                  std::vector<ossimRefPtr<ossimXmlNode> >& nodelist) const;
   
   OSSIMDLLEXPORT friend std::ostream& operator << (std::ostream& os, const ossimXmlDocument& xml_doc);

   void initRoot(ossimRefPtr<ossimXmlNode> node);

   ossimRefPtr<ossimXmlNode> getRoot();
   const ossimRefPtr<ossimXmlNode> getRoot()const;
   ossimRefPtr<ossimXmlNode> removeRoot();
   void fromKwl(const ossimKeywordlist& kwl);
   void toKwl(ossimKeywordlist& kwl, const ossimString& prefix = "")const;
   
private:
   ossimRefPtr<ossimXmlNode>  theRootNode;
   ossimString                theXmlHeader;
   ossimFilename              theFilename;
   bool                       theStrictCheckFlag;
   bool readHeader(std::istream& in);
TYPE_DATA
};

#endif /* #ifndef ossimXmlDocument_HEADER */
