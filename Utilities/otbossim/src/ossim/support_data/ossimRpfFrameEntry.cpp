//*******************************************************************
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
// 
// Author: Garrett Potts
//
// Description: Rpf support class
// 
//********************************************************************
// $Id: ossimRpfFrameEntry.cpp 14241 2009-04-07 19:59:23Z dburken $

#include <ostream>
#include <ossim/support_data/ossimRpfFrameEntry.h>

std::ostream& operator<<(std::ostream& out,
                         const ossimRpfFrameEntry& data)
{
   data.print(out);

   return out;
}

ossimRpfFrameEntry::ossimRpfFrameEntry(const ossimFilename& rootDirectory,
                                       const ossimFilename& pathToFrameFileFromRoot)
   :theExists(false),
    theRootDirectory(rootDirectory),
    thePathToFrameFileFromRoot(pathToFrameFileFromRoot),
    theFullValidPath()
{
   setEntry(rootDirectory,
            pathToFrameFileFromRoot);
}

ossimRpfFrameEntry::ossimRpfFrameEntry(const ossimRpfFrameEntry& rhs)
   :theExists(rhs.theExists),
    theRootDirectory(rhs.theRootDirectory),
    thePathToFrameFileFromRoot(rhs.thePathToFrameFileFromRoot),
    theFullValidPath(rhs.theFullValidPath)
{}

void ossimRpfFrameEntry::setEntry(const ossimFilename& rootDirectory,
                                  const ossimFilename& pathToFrameFileFromRoot)
{
   //---
   // We must check for case combinations:
   //---
   theRootDirectory           = rootDirectory;
   thePathToFrameFileFromRoot = pathToFrameFileFromRoot;
   theFullValidPath = theRootDirectory.dirCat(thePathToFrameFileFromRoot);

   // Check as supplied:
   if(theFullValidPath.exists())
   {
      theExists = true;
   }
   else // Check root/downcased_path
   {
      thePathToFrameFileFromRoot = thePathToFrameFileFromRoot.downcase();
      theFullValidPath = theRootDirectory.dirCat(thePathToFrameFileFromRoot);

      if(theFullValidPath.exists())
      {
         theExists = true;
      }
      else // Check root/upcased_path
      {
         thePathToFrameFileFromRoot = thePathToFrameFileFromRoot.upcase();
         theFullValidPath =
            theRootDirectory.dirCat(thePathToFrameFileFromRoot);
         if(theFullValidPath.exists())
         {
            theExists = true;
         }
         else
         {
            thePathToFrameFileFromRoot = pathToFrameFileFromRoot;
            theFullValidPath =
               theRootDirectory.dirCat(thePathToFrameFileFromRoot);
            theExists = false;
         }
      }
   }
}
std::ostream& ossimRpfFrameEntry::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "exists:       " << theExists << "\n"
       << prefix << "root_directory: " << theRootDirectory << "\n"
       << prefix << "relative_path: "
       << thePathToFrameFileFromRoot << "\n"
       << prefix << "full_path:     " << theFullValidPath << "\n";

   return out;
}
