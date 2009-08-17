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
#include "otbAeronetData.h"

namespace otb
{
/**
 * Constructor
 */

AeronetData
::AeronetData()
{
  /** Angstrom coefficient */
  m_AngstromCoefficient = 0.;
  /** Water data */
  m_Water = 0.;
  /** Aerosol optical thickness data */
  m_AerosolOpticalThickness = 0.;
  /** Cloud estimation data */
  bool m_CloudEstimation = false;

  /** Solar Zenith Angle */
  m_SolarZenithAngle = 0.;

  /** Statistic : Standard deviation data */
  m_StdDev = 0.;

  /** Data Date */
  m_DataDate = "";
  /** Epsilon to select time */
  m_EpsilonDate = 0.;
  /** Number of date used */
 m_NumberOfDateUsed = 0;

};

/**
 * PrintSelf method
 */

void
AeronetData
::PrintSelf( std::ostream& os,itk::Indent indent ) const
{
  Superclass::PrintSelf(os,indent);
}
}// end namespace otb
