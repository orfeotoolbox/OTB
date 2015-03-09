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
#ifndef __otbWriterWatcherBase_h
#define __otbWriterWatcherBase_h

#include "itkCommand.h"
#include "itkProcessObject.h"
#include "itkTimeProbe.h"

namespace otb
{

/** \class WriterWatcherBase
 *  \brief This class is an abstract class
 *         Provides an interface to progress task mechanic
 *
 *  Implement your callbacks
 *  \li ShowWriterProgress : callback called for each completed tile
 *  \li StartWriter        : callback called at the begin of tile writing
 *  \li EndWriter          : callback called at the end of tile writing
 *  \li ShowFilterProgress : callback called for each completed pixel
 *  \li StartFilter        : callback called at the begin of filter execution for a given tile
 *  \li EndFilter          : callback called at the end of filter execution for a given tile
 *
 *
 * \ingroup OTBCommon
 */
class /*ITK_EXPORT*/ WriterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  WriterWatcherBase(itk::ProcessObject* process,
                    const char *comment = "");

  /** This other constructor is provided so that the user can set a different processing filter than the one
  just before process in the pipeline */
  WriterWatcherBase(itk::ProcessObject* process, itk::ProcessObject * source, const char *comment = "");

  /** Default constructor */
  WriterWatcherBase();

  /** Copy constructor */
  WriterWatcherBase(const WriterWatcherBase&);

  /** operator=  */
  void operator =(const WriterWatcherBase&);

  /** Destructor. */
  virtual ~WriterWatcherBase();

  const char *GetNameOfClass()
    {
    return (m_Process.GetPointer() ? m_Process->GetNameOfClass() : "None");
    }

  /** Methods to access member data */
  /** Get a pointer to the process object being watched. */
  itk::ProcessObject *GetProcess()
  {
    return m_Process.GetPointer();
  }

  /** Get the comment for the watcher. */
  std::string GetComment() const
  {
    return m_Comment;
  }

  /** Get a reference to the TimeProbe */
  itk::TimeProbe& GetTimeProbe()
  {
    return m_TimeProbe;
  }

protected:

  /** Callback method to show the ProgressEvent from the writer */
  virtual void ShowWriterProgress() = 0;

  /** Callback method to show the StartEvent from the writer*/
  virtual void StartWriter() = 0;

  /** Callback method to show the EndEvent from the writer*/
  virtual void EndWriter() = 0;

  /** Callback method to show the ProgressEvent from the filter */
  virtual void ShowFilterProgress() = 0;

  /** Callback method to show the StartEvent from the filter*/
  virtual void StartFilter() = 0;

  /** Callback method to show the EndEvent from the filter*/
  virtual void EndFilter() = 0;

  /** Computing time */
  itk::TimeProbe m_TimeProbe;

  /** Associated comment */
  std::string m_Comment;

  /** Abstract process object */
  itk::ProcessObject::Pointer m_Process;

  /** Second abstract process object representing the source */
  itk::ProcessObject::Pointer m_SourceProcess;

  /** Internal type */
  typedef itk::SimpleMemberCommand<WriterWatcherBase> CommandType;

  /** Start writer callback */
  CommandType::Pointer m_StartWriterCommand;

  /** End writer callback */
  CommandType::Pointer m_EndWriterCommand;

  /** Progress writer callback */
  CommandType::Pointer m_ProgressWriterCommand;

  /** Start filter callback */
  CommandType::Pointer m_StartFilterCommand;

  /** End writer callback */
  CommandType::Pointer m_EndFilterCommand;

  /** Progress writer callback */
  CommandType::Pointer m_ProgressFilterCommand;

  /** Start oberserver */
  unsigned long m_StartWriterTag;

  /** End observer */
  unsigned long m_EndWriterTag;

  /** Progress observer */
  unsigned long m_ProgressWriterTag;

  /** Start oberserver */
  unsigned long m_StartFilterTag;

  /** End observer */
  unsigned long m_EndFilterTag;

  /** Progress observer */
  unsigned long m_ProgressFilterTag;

private:

};

} // end namespace otb

#endif
