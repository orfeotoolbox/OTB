/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbStandardFilterWatcher_h
#define __otbStandardFilterWatcher_h

#include "itkCommand.h"
#include "itkProcessObject.h"
#include "itkTimeProbe.h"

namespace otb
{

  /** \class StandardFilterWatcher
   *  \brief This class shows the percentage progress execution
   *         of the pipeline filtering process
   *
   *  This class is based on oberservers desgin patter 
   *  Abstract class ProcessObject is the subject
   *  Event are oberservers
   *
   *  \example Example usage
   *
   *  \code
   *  typedef itk::BinaryThresholdImageFilter<ImageType> FilterType;
   *  FilterType::Pointer thresholdFilter = FilterType::New();
   *
   *  StandardFilterWatcher watcher(thresholdFilter, "Threshold");
   *  \endcode
   *
   *  \see itk::SimpleFilterWatcher
   *  \see otb::fltkFilterWatcher
   */
  class ITK_EXPORT StandardFilterWatcher
    {
    public:
      
      /** Constructor. Takes a ProcessObject to monitor and an optional
       * comment string that is prepended to each event message. */
      StandardFilterWatcher(itk::ProcessObject* process,
			    const char *comment="");
      
      /** Copy constructor */
      StandardFilterWatcher(const StandardFilterWatcher&);
      
      /** operator=  */
      void operator=(const StandardFilterWatcher& );
      
      /** Destructor. */
      virtual ~StandardFilterWatcher();
      
      /** Methods to access member data */
      /** Get a pointer to the process object being watched. */
      itk::ProcessObject *GetProcess () {return m_Process.GetPointer();}
      
      /** Get the comment for the watcher. */
      std::string GetComment() {return m_Comment;}
      
      /** Get a reference to the TimeProbe */
      itk::TimeProbe &GetTimeProbe() {return m_TimeProbe;}
      
      /** Get/Set number of stars */
      void SetStars( int count ) { m_StarsCount = count;}
      const int& GetStars() const { return m_StarsCount;}
      
    protected:
      
      /** Callback method to show the ProgressEvent */
      virtual void ShowProgress();
      
      /** Callback method to show the StartEvent */
      virtual void StartFilter();
      
      /** Callback method to show the EndEvent */
      virtual void EndFilter();
      
    private:
      
      /** Computing time */
      itk::TimeProbe m_TimeProbe;
      
      /** Associated comment */
      std::string m_Comment;
      
      /** Stars coutning */
      int m_StarsCount;
      
      /** Abstract process object */
      itk::ProcessObject::Pointer m_Process;
      
      /** Internal type */
      typedef itk::SimpleMemberCommand<StandardFilterWatcher> CommandType;
      
      /** Start filter callback */
      CommandType::Pointer m_StartFilterCommand;
      
      /** End filter callback */
      CommandType::Pointer m_EndFilterCommand;
      
      /** Progress filter callback */
      CommandType::Pointer m_ProgressFilterCommand;
      
      /** Start oberserver */
      unsigned long m_StartTag;
      
      /** End observer */
      unsigned long m_EndTag;
      
      /** Progress observer */
      unsigned long m_ProgressTag;
    };
  
} // end namespace otb

#endif
