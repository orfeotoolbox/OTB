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
#ifndef __otbFltkFilterWatcher_h
#define __otbFltkFilterWatcher_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Progress.H>

#include "otbFilterWatcherBase.h"

namespace otb
{

/** \class FltkFilterWatcher
 *  \brief This class implements the progress mechanism
 *         on pipeline filtering execution
 *
 */
class ITK_EXPORT FltkFilterWatcher : public FilterWatcherBase
{
public:
  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  FltkFilterWatcher(itk::ProcessObject * process,
                    int x, int y, int w, int h,
                    const char *comment = "");

  /** Destructor. */
  virtual ~FltkFilterWatcher();

protected:
  /** Callback method to show the ProgressEvent */
  virtual void ShowProgress();

  /** Callback method to show the StartEvent */
  virtual void StartFilter();

  /** Callback method to show the EndEvent */
  virtual void EndFilter();

private:
  Fl_Window *   m_Window;
  Fl_Progress * m_Progress;

  double m_CurrentProgress;
};

} // end namespace otb

#endif
