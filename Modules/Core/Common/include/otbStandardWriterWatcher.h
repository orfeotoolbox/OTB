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
#ifndef otbStandardWriterWatcher_h
#define otbStandardWriterWatcher_h

#include "otbWriterWatcherBase.h"

namespace otb
{

/** \class StandardWriterWatcher
 *  \brief This class shows the percentage progress execution
 *         of the pipeline filtering process
 *
 *  This class is based on Observer design pattern
 *  Abstract class ProcessObject is the subject
 *  Event are observers
 *
 *  Usage example:
 *
 *  \code
 *  typedef itk::BinaryThresholdImageWriter<ImageType> WriterType;
 *  WriterType::Pointer thresholdWriter = WriterType::New();
 *
 *  StandardWriterWatcher watcher(thresholdWriter, "Threshold");
 *  \endcode
 *
 *  \see itk::SimpleWriterWatcher
 *  \see otb::fltkWriterWatcher
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT StandardWriterWatcher : public WriterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardWriterWatcher(itk::ProcessObject* process,
                        const char *comment = "");
  StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const char *comment = "");

  StandardWriterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");
  StandardWriterWatcher(itk::ProcessObject* process, itk::ProcessObject * source,
                        const std::string& comment = "");

  /** Default constructor */
  StandardWriterWatcher() : m_StarsCount(0) {};

  /** Copy constructor */
  StandardWriterWatcher(const StandardWriterWatcher&);

  /** operator=  */
  void operator =(const StandardWriterWatcher&);

  /** Get/Set number of stars */
  void SetStars(int count)
  {
    m_StarsCount = count;
  }
  const unsigned int& GetStars() const
  {
    return m_StarsCount;
  }

protected:

  /** Callback method to show the ProgressEvent */
  void ShowWriterProgress() ITK_OVERRIDE;

  /** Callback method to show the StartEvent */
  void StartWriter() ITK_OVERRIDE;

  /** Callback method to show the EndEvent */
  void EndWriter() ITK_OVERRIDE;

  /** Callback method to show the ProgressEvent */
  void ShowFilterProgress() ITK_OVERRIDE;

  /** Callback method to show the StartEvent */
  void StartFilter() ITK_OVERRIDE {}

  /** Callback method to show the EndEvent */
  void EndFilter() ITK_OVERRIDE {}

  /** This is the method invoked by ShowFilterProgress() and ShowWriterProgress() */
  virtual void ShowProgress();

private:

  /** Stars coutning */
  unsigned int m_StarsCount;
};

} // end namespace otb

#endif
