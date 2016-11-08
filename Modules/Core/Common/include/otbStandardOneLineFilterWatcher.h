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
#ifndef otbStandardOneLineFilterWatcher_h
#define otbStandardOneLineFilterWatcher_h

#include "otbFilterWatcherBase.h"

namespace otb
{

/** \class StandardOneLineFilterWatcher
 *  \brief This class shows the percentage progress execution
 *         of the pipeline filtering process
 *
 *  This class is based on Observer design patter
 *  Abstract class ProcessObject is the subject
 *  Event are observers
 *
 *  Usage example:
 *
 *  \code
 *  typedef itk::BinaryThresholdImageFilter<ImageType> FilterType;
 *  FilterType::Pointer thresholdFilter = FilterType::New();
 *
 *  StandardOneLineFilterWatcher watcher(thresholdFilter, "Threshold");
 *  \endcode
 *
 *  \see itk::SimpleFilterWatcher
 *  \see otb::fltkFilterWatcher
 *
 * \ingroup OTBCommon
 */
class OTBCommon_EXPORT StandardOneLineFilterWatcher : public FilterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardOneLineFilterWatcher(itk::ProcessObject* process,
                        const char *comment = "");

  StandardOneLineFilterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");

  /** Default constructor */
  StandardOneLineFilterWatcher();

  /** Get/Set number of stars */
  void SetStars(int count)
  {
    m_StarsCount = count;
  }
  const int& GetStars() const
  {
    return m_StarsCount;
  }

protected:

  /** Callback method to show the ProgressEvent */
  void ShowProgress() ITK_OVERRIDE;

  /** Callback method to show the StartEvent */
  void StartFilter() ITK_OVERRIDE;

  /** Callback method to show the EndEvent */
  void EndFilter() ITK_OVERRIDE;

private:

  /** Stars coutning */
  int m_StarsCount;

  int m_CurrentNbStars;
};

} // end namespace otb

#endif
