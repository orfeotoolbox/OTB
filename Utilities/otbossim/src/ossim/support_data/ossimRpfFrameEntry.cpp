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
// $Id: ossimRpfFrameEntry.cpp 18052 2010-09-06 14:33:08Z dburken $

#include <ostream>
#include <ossim/support_data/ossimRpfFrameEntry.h>

std::ostream& operator<<(std::ostream& out, const ossimRpfFrameEntry& data)
{
   return data.print(out);
}

ossimRpfFrameEntry::ossimRpfFrameEntry(const ossimFilename& rootDirectory,
                                       const ossimFilename& pathToFrameFileFromRoot)
   :m_exists(false),
    m_rootDirectory(rootDirectory),
    m_pathToFrameFileFromRoot(pathToFrameFileFromRoot),
    m_fullValidPath()
{
   setEntry(rootDirectory,
            pathToFrameFileFromRoot);
}

ossimRpfFrameEntry::ossimRpfFrameEntry(const ossimRpfFrameEntry& obj)
   :m_exists(obj.m_exists),
    m_rootDirectory(obj.m_rootDirectory),
    m_pathToFrameFileFromRoot(obj.m_pathToFrameFileFromRoot),
    m_fullValidPath(obj.m_fullValidPath)
{}

const ossimRpfFrameEntry& ossimRpfFrameEntry::operator=(const ossimRpfFrameEntry& rhs)
{
   if (this != &rhs)
   {
      m_exists                  = rhs.m_exists;
      m_rootDirectory           = rhs.m_rootDirectory;
      m_pathToFrameFileFromRoot = rhs.m_pathToFrameFileFromRoot;
      m_fullValidPath           = rhs.m_fullValidPath;
   }
   return *this;
}

void ossimRpfFrameEntry::setEntry(const ossimFilename& rootDirectory,
                                  const ossimFilename& pathToFrameFileFromRoot)
{
   //---
   // We must check for case combinations:
   //---
   m_rootDirectory           = rootDirectory;
   m_pathToFrameFileFromRoot = pathToFrameFileFromRoot;
   m_fullValidPath = m_rootDirectory.dirCat(m_pathToFrameFileFromRoot);

   // Check as supplied:
   if(m_fullValidPath.exists())
   {
      m_exists = true;
   }
   else // Check root/downcased_path
   {
      m_pathToFrameFileFromRoot = m_pathToFrameFileFromRoot.downcase();
      m_fullValidPath = m_rootDirectory.dirCat(m_pathToFrameFileFromRoot);

      if(m_fullValidPath.exists())
      {
         m_exists = true;
      }
      else // Check root/upcased_path
      {
         m_pathToFrameFileFromRoot = m_pathToFrameFileFromRoot.upcase();
         m_fullValidPath =
            m_rootDirectory.dirCat(m_pathToFrameFileFromRoot);
         if(m_fullValidPath.exists())
         {
            m_exists = true;
         }
         else
         {
            m_pathToFrameFileFromRoot = pathToFrameFileFromRoot;
            m_fullValidPath =
               m_rootDirectory.dirCat(m_pathToFrameFileFromRoot);
            m_exists = false;
         }
      }
   }
}
std::ostream& ossimRpfFrameEntry::print(
   std::ostream& out, const std::string& prefix) const
{
   out << prefix << "exists:       " << m_exists << "\n"
       << prefix << "root_directory: " << m_rootDirectory << "\n"
       << prefix << "relative_path: "
       << m_pathToFrameFileFromRoot << "\n"
       << prefix << "full_path:     " << m_fullValidPath << "\n";

   return out;
}

bool ossimRpfFrameEntry::exists() const
{
   return m_exists;
}

const ossimFilename& ossimRpfFrameEntry::getFullPath() const
{
   return m_fullValidPath;
}

const ossimString& ossimRpfFrameEntry::getRootDirectory() const
{
   return m_rootDirectory;
}

const ossimString ossimRpfFrameEntry::getPathToFrameFileFromRoot() const
{
   return m_pathToFrameFileFromRoot;
}
