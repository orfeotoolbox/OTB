//----------------------------------------------------------------------------
//
// File: ossimFileWalker.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Utility class to walk through directories and get a list of files to
// process.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/util/ossimFileWalker.h>
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/parallel/ossimJobQueue.h>
#include <OpenThreads/Thread>

static ossimTrace traceDebug(ossimString("ossimFileWalker:debug"));

ossimFileWalker::ossimFileWalker()
   : m_processFileCallBackPtr(0),
     m_jobQueue(new ossimJobMultiThreadQueue(new ossimJobQueue(), 1)),     
     m_filteredExtensions(0),
     m_recurseFlag(true),
     m_waitOnDirFlag(false),
     m_abortFlag(false),
     m_mutex()
{
}

ossimFileWalker::~ossimFileWalker()
{
   m_jobQueue = 0; // Not a leak, ref pointer.
}

void ossimFileWalker::walk(const std::vector<ossimFilename>& files)
{
   static const char M[] = "ossimFileWalker::walk";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered\n";
   }

   if ( files.size() )
   {
      std::vector<ossimFilename>::const_iterator i = files.begin();
      while ( i != files.end() )
      {
         // Must have call back set at this point.
         if ( !m_abortFlag && m_processFileCallBackPtr )
         {
            ossimFilename file = (*i).expand();
            if ( file.size() && file.exists() )
            {
               if ( file.isDir() ) // Directory:
               {
                  walkDir(file);
               }  
               else // File:
               {
                  if ( isFiltered(file) == false )
                  {
                     if(traceDebug())
                     {
                        ossimNotify(ossimNotifyLevel_DEBUG)
                           << "Making the job for: " << (*i) << std::endl;
                     }
                     
                     // Make the job:
                     ossimRefPtr<ossimFileWalkerJob> job =
                        new ossimFileWalkerJob( m_processFileCallBackPtr, file );
                     
                     job->setName( ossimString( file.string() ) );
                     
                     job->setCallback( new ossimFileWalkerJobCallback() );
                     
                     // Set the state to ready:
                     job->ready();
                     
                     // Add job to the queue:
                     m_jobQueue->getJobQueue()->add( job.get() );
                     
                     m_mutex.lock();
                     if ( m_abortFlag )
                     {
                        // Clear out the queue.
                        m_jobQueue->getJobQueue()->clear();
                        
                        break; // Callee set our abort flag so break out of loop.
                     }
                     m_mutex.unlock();
                  }
               }
            }
         }

         ++i;
      
      } // while ( i != files.end() )

      // FOREVER loop until all jobs are completed.
      while (1)
      {
         if ( OpenThreads::Thread::microSleep(250) == 0 )
         {
            if ( m_jobQueue->hasJobsToProcess() == false )
            {
               break;
            }
         }
      }

   } // if ( files.size() )

   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exiting...\n";
   }  
}

void ossimFileWalker::walk(const ossimFilename& root)
{
   static const char M[] = "ossimFileWalker::walk";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " entered root=" << root << "\n";
   }

   // Must have call back set at this point.
   if ( !m_abortFlag && m_processFileCallBackPtr )
   {
      ossimFilename rootFile = root.expand();
      if ( rootFile.size() && rootFile.exists() )
      {
         if ( rootFile.isDir() )
         {
            walkDir(rootFile);

            // FOREVER loop until all jobs are completed.
            while (1)
            {
               if ( OpenThreads::Thread::microSleep(250) == 0 )
               {
                  if ( m_jobQueue->hasJobsToProcess() == false )
                  {
                     break;
                  }
               }
            }
         }
         else
         {
            // Single file no job queue needed.
            if ( isFiltered(rootFile) == false )
            {
               m_processFileCallBackPtr->operator()(rootFile);
            }
         }
      }
   }
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exiting...\n";
   }  
}

void ossimFileWalker::walkDir(const ossimFilename& dir)
{
   static const char M[] = "ossimFileWalker::walkDir";
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << M << " entered...\n" << "processing dir: " << dir << "\n";
   }

   // List of directories in this directory...
   std::vector<ossimFilename> dirs;
   
   // List of files in this directory...
   std::vector<ossimFilename> files;

   m_mutex.lock();
   ossimDirectory d;
   bool ossimDirectoryStatus = d.open(dir);
   m_mutex.unlock();

   if ( ossimDirectoryStatus )
   {
      // Loop to get the list of files and directories in this directory.
      m_mutex.lock();
      ossimFilename f;
      bool valid_file = d.getFirst(f);
      while ( valid_file )
      {
         if ( isFiltered(f) == false )
         {
            if (f.isDir())
            {
               dirs.push_back(f);
            }
            else
            {
               files.push_back(f);
            }
         }
         valid_file = d.getNext(f);
      }
      m_mutex.unlock();

      //---
      // Process files first before recursing directories.  If a file is a directory base image,
      // e.g. RPF, then the callee should call ossimFileWalker::setRecurseFlag to false to
      // stop us from going into sub directories.
      //---
      std::vector<ossimFilename>::const_iterator i = files.begin();
      while (i != files.end())
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "Making the job for: " << (*i) << std::endl;
         }
         
         // Make the job:
         ossimRefPtr<ossimFileWalkerJob> job =
            new ossimFileWalkerJob( m_processFileCallBackPtr, (*i) );

         job->setName( ossimString( (*i).string() ) );

         job->setCallback( new ossimFileWalkerJobCallback() );

         // Set the state to ready:
         job->ready();

         // Add job to the queue:
         m_jobQueue->getJobQueue()->add( job.get() );

         m_mutex.lock();
         if ( m_abortFlag )
         {
            // Clear out the queue.
            m_jobQueue->getJobQueue()->clear();
            
            break; // Callee set our abort flag so break out of loop.
         }
         m_mutex.unlock();

         ++i;
      }

      if ( m_waitOnDirFlag )
      {
         // FOREVER loop until all jobs are completed.
         while (1)
         {
            if ( OpenThreads::Thread::microSleep(250) == 0 )
            {
               if ( m_jobQueue->hasJobsToProcess() == false )
               {
                  break;
               }
            }
         }
      }

      m_mutex.lock();
      if ( !m_abortFlag && m_recurseFlag )
      {
         // Process sub directories...
         i = dirs.begin();
         while (i != dirs.end())
         {
            m_mutex.unlock();
            walkDir( (*i) );
            m_mutex.lock();

            if ( m_abortFlag )
            {
               break; // Callee set our abort flag so break out of loop.
            }
            ++i;
         }
      }
      m_mutex.unlock();
      
   } // if ( ossimDirectoryOpenStatus )

   // Reset the m_recurseFlag.
   m_mutex.lock();
   m_recurseFlag = true;
   m_mutex.unlock();
   
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG) << M << " exited...\n";
   }
}

bool ossimFileWalker::isFiltered(const ossimFilename& file) const
{
   bool result = false;
   if ( file.size() )
   {
      if ( isDotFile(file) )
      {
         result = true;
      }
      else if ( file[file.size()-1] == '~' )
      {
         result = true;
      }
      else 
      {
         std::string ext = file.ext().downcase().c_str();
         if ( ext.size() )
         {
            std::vector<std::string>::const_iterator i = m_filteredExtensions.begin();
            while ( i != m_filteredExtensions.end() )
            {
               if ( ext == (*i) )
               {
                  result = true;
                  break;
               }
               ++i;
            }
         }
      }
   }
#if 0 /* Please leave for debug. (drb) */
   if(traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimFileWalker::isFiltered file " << (result?"filtered: ":"not filtered: ")
         << file << "\n";
   }
#endif
   
   return result;
}

bool ossimFileWalker::isDotFile(const ossimFilename& f) const
{
   bool result = false;
   
   // Get the size in bytes.
   if ( f.size() )
   {
      std::string::size_type firstDotPos = f.find('.');
      if ( firstDotPos == 0 )
      {
         result = true;
      }
      else if ( firstDotPos != std::string::npos ) // Dot in file.
      {
         // Get the position of first dot from the end.
         std::string::size_type lastDotPos = f.find_last_of('.');
         if ( lastDotPos != std::string::npos )
         {
            // Make copy.
            ossimFilename f2 = f;
            
            // Convert an '\'s to '/'s. 
            f2.convertBackToForwardSlashes();
            
            // Find the first slash from end.
            std::string::size_type lastSlashPos = f2.find_last_of('/');
            
            if (lastSlashPos != std::string::npos) // Found a slash.
            {
               if ( (lastSlashPos+1) == lastDotPos )
               {
                  // dot in front of slash like /home/foo/.xemacs
                  result = true;
               }
            }
         }
      }
   }

   return result;
}

const std::vector<std::string>& ossimFileWalker::getFilteredExtensions() const
{
   return m_filteredExtensions;
}

std::vector<std::string>& ossimFileWalker::getFilteredExtensions()
{
   return m_filteredExtensions;
}

void ossimFileWalker::initializeDefaultFilterList()
{
   m_mutex.lock();
   
   // Common extensions to filter out, most common first.
   m_filteredExtensions.push_back(std::string("ovr"));
   m_filteredExtensions.push_back(std::string("omd"));
   m_filteredExtensions.push_back(std::string("his"));
   m_filteredExtensions.push_back(std::string("geom"));
   
   // The rest alphabetical.
   m_filteredExtensions.push_back(std::string("aux"));
   m_filteredExtensions.push_back(std::string("bin"));
   m_filteredExtensions.push_back(std::string("dbf"));
   m_filteredExtensions.push_back(std::string("hdr"));
   m_filteredExtensions.push_back(std::string("jpw"));
   m_filteredExtensions.push_back(std::string("kwl"));
   m_filteredExtensions.push_back(std::string("log"));
   m_filteredExtensions.push_back(std::string("out"));
   m_filteredExtensions.push_back(std::string("prj"));
   m_filteredExtensions.push_back(std::string("save"));
   m_filteredExtensions.push_back(std::string("sdw"));
   m_filteredExtensions.push_back(std::string("shx"));
   m_filteredExtensions.push_back(std::string("spec"));
   m_filteredExtensions.push_back(std::string("statistics"));
   m_filteredExtensions.push_back(std::string("tfw"));
   m_filteredExtensions.push_back(std::string("tmp"));
   m_filteredExtensions.push_back(std::string("txt"));

   m_mutex.unlock();
}

void ossimFileWalker::setRecurseFlag(bool flag)
{
   m_mutex.lock();
   m_recurseFlag = flag;
   m_mutex.unlock();
}

void ossimFileWalker::setWaitOnDirFlag(bool flag)
{
   m_mutex.lock();
   m_waitOnDirFlag = flag;
   m_mutex.unlock();
}

void ossimFileWalker::setAbortFlag(bool flag)
{
   m_mutex.lock();
   m_abortFlag = flag;
   m_mutex.unlock();
}

void ossimFileWalker::setNumberOfThreads(ossim_uint32 nThreads)
{
   m_mutex.lock();
   m_jobQueue->setNumberOfThreads(nThreads);
   m_mutex.unlock();
}

void ossimFileWalker::registerProcessFileCallback(
   ossimCallback1<const ossimFilename&>* cb)
{
   m_mutex.lock();
   m_processFileCallBackPtr = cb;
   m_mutex.unlock();
}

ossimFileWalker::ossimFileWalkerJob::ossimFileWalkerJob(
   ossimCallback1<const ossimFilename&>* cb,
   const ossimFilename& file)
   : m_processFileCallBackPtr(cb),
     m_file(file)
{
}

void ossimFileWalker::ossimFileWalkerJob::start()
{
   if ( m_processFileCallBackPtr && m_file.size() )
   {
      m_processFileCallBackPtr->operator()(m_file);
   }
}

ossimFileWalker::ossimFileWalkerJobCallback::ossimFileWalkerJobCallback()
   : ossimJobCallback()
{
}

void ossimFileWalker::ossimFileWalkerJobCallback::started(ossimJob* job)
{
   ossimJobCallback::started(job);
}

void ossimFileWalker::ossimFileWalkerJobCallback::finished(ossimJob* job)
{
   ossimJobCallback::finished(job);
}

void ossimFileWalker::ossimFileWalkerJobCallback::canceled(ossimJob* job)
{
   ossimJobCallback::canceled(job);
}
