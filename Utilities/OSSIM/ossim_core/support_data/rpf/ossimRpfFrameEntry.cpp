#include "ossimRpfFrameEntry.h"

ostream& operator<<(ostream& out,
                    const ossimRpfFrameEntry& data)
{
   data.print(out);

   return out;
}


ossimRpfFrameEntry::ossimRpfFrameEntry(const ossimString& rootDirectory,
                                       const ossimString& pathToFrameFileFromRoot)
   :theRootDirectory(rootDirectory),
    thePathToFrameFileFromRoot(pathToFrameFileFromRoot)
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

void ossimRpfFrameEntry::setEntry(const ossimString& rootDirectory,
                                  const ossimString& pathToFrameFileFromRoot)
{
   ossimFilename temp(rootDirectory + pathToFrameFileFromRoot);
   
   theRootDirectory           = rootDirectory;
   thePathToFrameFileFromRoot = pathToFrameFileFromRoot;
   
   if(temp.exists())
   {
      theFullValidPath = temp;
      theExists        = true;
   }
   else // it might be upper cases( upper/lower) may be different. check it
   {
      thePathToFrameFileFromRoot = thePathToFrameFileFromRoot.downcase();
      temp = ossimFilename(theRootDirectory + thePathToFrameFileFromRoot);
      if(temp.exists())
      {
         theFullValidPath = temp;
         theExists        = true;
      }
      else
      {
         thePathToFrameFileFromRoot = thePathToFrameFileFromRoot.upcase();
         temp = ossimFilename(theRootDirectory + thePathToFrameFileFromRoot);
         if(temp.exists())
         {
            theFullValidPath = temp;
            theExists = true;
         }
         else
         {
            thePathToFrameFileFromRoot = pathToFrameFileFromRoot;
            theFullValidPath = ossimFilename(rootDirectory + pathToFrameFileFromRoot);
            theExists = false;
         }
      }
   }
}

void ossimRpfFrameEntry::print(ostream& out)const
{
   out << "exists:          " << theExists    << endl
       << "theFilename:     " << theFullValidPath;
}
