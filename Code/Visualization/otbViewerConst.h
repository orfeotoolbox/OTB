/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbViewerConst_h
#define __otbViewerConst_h

namespace otb
{
  namespace Function
  {
    const unsigned int SCREEN_COLOR_MIN_VALUE(0);
    const unsigned int SCREEN_COLOR_MIDDLE_VALUE(127);
    const unsigned int SCREEN_COLOR_MAX_VALUE(255);

    /**
     * Default standard deviation value of the gaussian distribution
     */
    const double GAUSSIAN_STANDARD_DEVIATION(1.0);
    const double GAUSSIAN_MEAN(SCREEN_COLOR_MIDDLE_VALUE);

  } // namespace Function

} // namespace otb

#endif /* __otbViewerConst_h */
