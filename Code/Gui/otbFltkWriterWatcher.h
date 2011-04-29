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
#ifndef __otbFltkWriterWatcher_h
#define __otbFltkWriterWatcher_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Progress.H>

#include "otbWriterWatcherBase.h"

namespace otb
{

/** \class FltkWriterWatcher
 *  \brief This class implements the progress mechanism
 *         on pipeline filtering execution
 *
 */
class ITK_EXPORT FltkWriterWatcher : public WriterWatcherBase
{
public:
  /** Classes that need access to filter's private data */
  // friend class XMLWriterWatcher;

  /** Constructor. Takes a ProcessObject to monitor and an optional
   * comment string that is prepended to each event message. */
  FltkWriterWatcher(itk::ProcessObject * process,
                    int x, int y, int w, int h,
                    const char *comment = "");

  FltkWriterWatcher(itk::ProcessObject * process,
                    itk::ProcessObject * source,
                    int x, int y, int w, int h,
                    const char *comment = "");

  /** Destructor. */
  virtual ~FltkWriterWatcher();

protected:

  /** Callback method to show the ProgressEvent from the writer */
  virtual void ShowWriterProgress();

  /** Callback method to show the StartEvent from the writer*/
  virtual void StartWriter();

  /** Callback method to show the EndEvent from the writer*/
  virtual void EndWriter();

  /** Callback method to show the ProgressEvent from the filter */
  virtual void ShowFilterProgress();

  /** Callback method to show the StartEvent from the filter*/
  virtual void StartFilter();

  /** Callback method to show the EndEvent from the filter*/
  virtual void EndFilter();

  void BuildGUI(int x, int y, int w, int h, const char * comment);

private:

  Fl_Window *   m_Window;
  Fl_Progress * m_WriterProgress;
  Fl_Progress * m_FilterProgress;

  double m_CurrentFilterProgress;
  double m_CurrentWriterProgress;
};

} // end namespace otb

#endif
