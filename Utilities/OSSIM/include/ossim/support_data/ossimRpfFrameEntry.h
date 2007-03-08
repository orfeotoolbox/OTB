//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author: Garrett Potts (gpotts@imagelinks.com)
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameEntry.h 9967 2006-11-29 02:01:23Z gpotts $
#ifndef ossimRpfFrameEntry_HEADER
#define ossimRpfFrameEntry_HEADER
#include <iostream>
using namespace std;

#include <ossim/base/ossimFilename.h>
class ossimRpfFrameEntry
{
public:
   friend ostream& operator<<(ostream& out,
                              const ossimRpfFrameEntry& data);
   ossimRpfFrameEntry(const ossimString& rootDirectory=ossimString(""),
                      const ossimString& pathToFrameFileFromRoot=ossimString(""));
   ossimRpfFrameEntry(const ossimRpfFrameEntry& rhs);
   void print(ostream& out)const;
   bool exists()const{return theExists;}
   void setEntry(const ossimString& rootDirectory,
                 const ossimString& pathToFrameFileFromRoot);
   const ossimFilename& getFullPath()const{return theFullValidPath;}
   const ossimString&   getRootDirectory()const{return theRootDirectory;}
   const ossimString    getPathToFrameFileFromRoot()const
      {
         return thePathToFrameFileFromRoot;
      }
   
private:
   bool theExists;

   ossimString theRootDirectory;

   ossimString thePathToFrameFileFromRoot;

   ossimFilename theFullValidPath;
};

#endif
