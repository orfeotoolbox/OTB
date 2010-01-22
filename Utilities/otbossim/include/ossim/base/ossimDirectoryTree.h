//*******************************************************************
//
// License:  See top level LICENSE.txt file.
//
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimDirectoryTree.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimDirectoryTree_HEADER
#define ossimDirectoryTree_HEADER
#include <queue>
#include <vector>
using namespace std;

#include <ossim/base/ossimDirectory.h>

class OSSIMDLLEXPORT ossimDirectoryTree
{
public:
   ossimDirectoryTree();
   ~ossimDirectoryTree();

   bool open(const ossimFilename& dir);

   bool isOpened() const;
   
   bool getFirst(ossimFilename &filename,
                 int flags = ossimDirectory::OSSIM_DIR_DEFAULT);
   
   // get next file in the enumeration started with either GetFirst() or
   // GetFirstNormal()
   bool getNext(ossimFilename &filename) ;

  void findAllFilesThatMatch(std::vector<ossimFilename>& result,
			     const ossimString& regularExpressionPattern,
			     int flags = ossimDirectory::OSSIM_DIR_DEFAULT);
private:
   class OSSIMDLLEXPORT ossimDirData
   {
   public:
      ~ossimDirData()
         {
            if(theDirectory)
            {
               delete theDirectory;
               theFilename = "";
            }
         }
      ossimDirData(ossimDirectory *aDir,
                         const ossimFilename &filename)
                         :theDirectory(aDir),
                         theFilename(filename)
         {
            
         }
      ossimDirectory* theDirectory;
      ossimFilename   theFilename;
   };
   
   queue<ossimFilename> theDirectoryQueue;
   ossimDirData*        theCurrentDirectoryData;
   int                  theFlags;

   void deleteAll();

   void checkToPushDirectory(const ossimFilename &filename);
   // for now we will hide copy
   ossimDirectoryTree(const ossimDirectoryTree &rhs);
   const ossimDirectoryTree& operator =(const ossimDirectoryTree& rhs);
};

#endif
