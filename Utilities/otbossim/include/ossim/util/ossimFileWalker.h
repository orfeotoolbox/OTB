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
// Description:  See description for class below.
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimFileWalker_HEADER
#define ossimFileWalker_HEADER 1

#include <ossim/base/ossimCallback1.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimFilename.h>
#include <ossim/parallel/ossimJob.h>
#include <ossim/parallel/ossimJobMultiThreadQueue.h>
#include <OpenThreads/Mutex>
#include <string>
#include <vector>

class ossimFilename;

/**
 * @class ossimFileWalker
 *
 * Utility class to walk through directories and get a list of files to
 * process. For each file found the callback is excecuted.  Mechanisms are in
 * place to filter files and to call back  to a processFile method.  This
 * requires a callback method to process files from the user.  The callback is
 * set via registerProcessFileCallback method.  Internally the processFile
 * calls are placed in a job queue.
 *
 * Typical usage (snip from ossimTiledElevationDatabase):
 *
 * ossimFileWalker* fw = new ossimFileWalker();
 * fw->initializeDefaultFilterList();
 * ossimCallback1<const ossimFilename&, bool&, bool>* cb =
 * new ProcessFileCB(this, &ossimTiledElevationDatabase::processFile);
 * fw->registerProcessFileCallback(cb);
 * fw->walk(f);
 */
class OSSIM_DLL ossimFileWalker
{
public:
   
   /** default constructor */
   ossimFileWalker();

   /** destructor */
   ~ossimFileWalker();

   /**
    * @brief Takes an array of files.
    *
    * For each file in array:  If files is a directory, it will walk it. Files
    * found in walk or files(not directories) in the array will be processed
    * via a job queue.
    *
    * Files are filter prior to the callback execution.  The filtering is to
    * eliminate sending unwanted files to the callback.  There is a default
    * filter table.  This can be edited by calling the non-const
    * getFilteredExtensions method.
    *
    * Each callback is placed in a threaded job queue.  So users should ensure
    * their callback is thread safe.
    */
   void walk(const std::vector<ossimFilename>& files);

   /**
    * @brief This will walk "root" and execute a callback for each file found.
    *
    * Files are filter prior to the callback execution.  The filtering is to
    * eliminate sending unwanted files to the callback.  There is a default
    * filter table.  This can be edited by calling the non-const
    * getFilteredExtensions method.
    *
    * Each callback is placed in a threaded job queue.  So users should ensure
    * their callback is thread safe.
    */
   void walk(const ossimFilename& root);

   // void walk(const ossimFilename& root);

   /**
    * @brief Registers callback method to process a file.
    *
    * @param cb Callback to register.
    *
    * @see m_processFileCallBackPtr for documentation on template interface.
    */   
   void registerProcessFileCallback(ossimCallback1<const ossimFilename&>* cb);

   /** @return The list of filtered out files. */
   const std::vector<std::string>& getFilteredExtensions() const;

   /**
    * @brief Non const method to allow access for
    * adding or deleting extensions from the list.
    *
    * The list is used by the private isFiltered method to avoid trying to
    * process unwanted files.
    */
   std::vector<std::string>& getFilteredExtensions();

   /**
    * @brief Initializes the filter list with a default set of filtered out
    * file names.
    */
   void initializeDefaultFilterList();

   /**
    * @brief Sets recurse flag.
    *
    * If set to true this stops recursing of the
    * current directory.  Defaulted to true in the constructor.
    * Typically used to indicate the directory being processed holds a
    * directory based image, e.g. RPF data.
    *
    * @param flag True to recurse, false to stop recursion of current
    * directory.
    */
   void setRecurseFlag(bool flag);

   /**
    * @brief Sets waitOnDir flag.
    *
    * If set to true each directory is processed in entirety before sub
    * directories are recursed.  This allows callers of setRecurseFlag
    * to disable directory walking.  If your code is calling setRecurseFlag
    * this flag should be set to true.
    *
    * @param flag true to wait, false to not wait. Defaulted to false in the constructor. 
    */   
   void setWaitOnDirFlag(bool flag);

   /**
    * @brief If set to true this stops files walking (aborts).
    * @param flag True to abort current "walk".
    */
   void setAbortFlag(bool flag);

   /** @brief Sets the max number of threads(jobs) to run at one time. */
   void setNumberOfThreads(ossim_uint32 nThreads);
   
private:

   /** @brief Private ossimJob class. */
   class ossimFileWalkerJob : public ossimJob
   {
   public:
      /**
       * @brief Constructor that takes callback and file.
       * @param cb Callback to method to process a file.
       * @param file The file to process.
       */
      ossimFileWalkerJob(ossimCallback1<const ossimFilename&>* cb,
                         const ossimFilename& file);
      /**
       * @brief Defines pure virtual ossimJob::start.
       *
       * This executes the call to m_processFileCallBackPtr.
       */
      virtual void start();
      
   private:
      ossimCallback1<const ossimFilename&>* m_processFileCallBackPtr;
      ossimFilename                         m_file;
      
   }; // End: class ossimFileWalkerJob

   /** @brief Private ossimJobCallback class. */
   class ossimFileWalkerJobCallback : public ossimJobCallback
   {
   public:
      ossimFileWalkerJobCallback();
      virtual void started(ossimJob* job);
      virtual void finished(ossimJob* job);
      virtual void canceled(ossimJob* job);
   };

   /**
    * @brief Processes files in directory.
    *
    * If a file in the directory is itself a directory this will do a recursive
    * call to itself.  Individual files are processed in a job queue...
    */
   void walkDir(const ossimFilename& dir);
   
   /**
    * @brief Convenience method for file walker code to check file to see is
    * it should be processed.
    *
    * @param f File to check.
    * 
    * @return true f is stagagble, false if not.
    */
   bool isFiltered(const ossimFilename& f) const;
   
   /**
    * @brief isDotFile method.
    * @param f File/directory to check.
    * @return true if file is a dot file.
    */   
   bool isDotFile(const ossimFilename& f) const;
   
   /**
    * @brief Callback to method to process a file.
    *
    * @param const ossimFilename& First parameter(argument) file to process.
    */
   ossimCallback1<const ossimFilename&>* m_processFileCallBackPtr;
   ossimRefPtr<ossimJobMultiThreadQueue> m_jobQueue;
   std::vector<std::string>              m_filteredExtensions;
   bool                                  m_recurseFlag;
   bool                                  m_waitOnDirFlag;
   bool                                  m_abortFlag;
   OpenThreads::Mutex                    m_mutex;
};

#endif /* #ifndef ossimFileWalker_HEADER */
