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
::AeronetData() :
  m_AngstromCoefficient(0),
  m_Water(0),
  m_AerosolOpticalThickness(0),
  m_CloudEstimation(false),
  m_SolarZenithAngle(0),
  m_StdDev(0.0),
  m_DataDate(""),
  m_EpsilonDate(0.0),
  m_NumberOfDateUsed(0)
{}

/**
 * PrintSelf method
 */

void
AeronetData
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Angstrom coefficient  :              " << m_AngstromCoefficient << std::endl;
  os << "Water data  :                        " << m_Water << std::endl;
  os << "Aerosol optical thickness data:      " << m_AerosolOpticalThickness << std::endl;
  os << "Cloud estimation dat  :              " << m_CloudEstimation << std::endl;
  os << "Solar Zenith Angle  :                " << m_SolarZenithAngle << std::endl;
  os << "Statistic : Standard deviation data: " << m_StdDev << std::endl;
  os << "Data Date  :                         " << m_DataDate << std::endl;
  os << "Epsilon to select time  :            " << m_EpsilonDate << std::endl;
  os << "Number of date used:                 " << m_NumberOfDateUsed << std::endl;

}
} // end namespace otb
