/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbAeronetData.h"

namespace otb
{
/**
 * Constructor
 */

AeronetData::AeronetData()
  : m_AngstromCoefficient(0),
    m_Water(0),
    m_AerosolOpticalThickness(0),
    m_CloudEstimation(false),
    m_SolarZenithAngle(0),
    m_StdDev(0.0),
    m_DataDate(""),
    m_EpsilonDate(0.0),
    m_NumberOfDateUsed(0)
{
}

/**
 * PrintSelf method
 */

void AeronetData::PrintSelf(std::ostream& os, itk::Indent indent) const
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
