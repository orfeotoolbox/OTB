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

#include "otbFilterWatcherBase.h"

namespace otb
{

/** \class StandardFilterWatcher
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
 *  StandardFilterWatcher watcher(thresholdFilter, "Threshold");
 *  \endcode
 *
 *  \see itk::SimpleFilterWatcher
 *  \see otb::fltkFilterWatcher
 *
 * \ingroup OTBCommon
 */
class /*ITK_EXPORT*/ StandardFilterWatcher : public FilterWatcherBase
{
public:

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  StandardFilterWatcher(itk::ProcessObject* process,
                        const char *comment = "");

  StandardFilterWatcher(itk::ProcessObject* process,
                        const std::string& comment = "");

  /** Default constructor */
  StandardFilterWatcher() : m_StarsCount(0) {};

  /** Copy constructor */
  StandardFilterWatcher(const StandardFilterWatcher&);

  /** operator=  */
  void operator =(const StandardFilterWatcher&);

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
  virtual void ShowProgress();

  /** Callback method to show the StartEvent */
  virtual void StartFilter();

  /** Callback method to show the EndEvent */
  virtual void EndFilter();

private:

  /** Stars coutning */
  int m_StarsCount;

  int m_CurrentNbStars;
};

} // end namespace otb

#endif
