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

#include "otbAtmosphericRadiativeTerms.h"

namespace otb
{
/************** AtmosphericRadiativeTermsSingleChannel******************/
/** Constructor */
AtmosphericRadiativeTermsSingleChannel::AtmosphericRadiativeTermsSingleChannel() :
  m_IntrinsicAtmosphericReflectance(0.04),
  m_SphericalAlbedo(0.09),
  m_TotalGaseousTransmission(0.95),
  m_DownwardTransmittance(0.91),
  m_UpwardTransmittance(0.94),
  m_UpwardDiffuseTransmittance(0.09),
  m_UpwardDirectTransmittance(0.85),
  m_UpwardDiffuseTransmittanceForRayleigh(0.05),
  m_UpwardDiffuseTransmittanceForAerosol(0.04),
  m_WavelengthSpectralBand(0.0)
{
}

/**PrintSelf method */
void
AtmosphericRadiativeTermsSingleChannel
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Atmospheric Intrinsic Reflectance        : " << m_IntrinsicAtmosphericReflectance << std::endl;
  os << indent << "Atmosphere Shperical Albedo              : " << m_SphericalAlbedo << std::endl;
  os << indent << "Total Gaseous Transmission               : " << m_TotalGaseousTransmission << std::endl;
  os << indent << "Atmospher Downward Transmittance         : " << m_DownwardTransmittance << std::endl;
  os << indent << "Atmospher Upward Transmittance           : " << m_UpwardTransmittance << std::endl;
  os << indent << "Upward Diffuse Transmittance             : " << m_UpwardDiffuseTransmittance << std::endl;
  os << indent << "Upward Direct Transmittance              : " << m_UpwardDirectTransmittance << std::endl;
  os << indent << "Upward Diffuse Transmittance for Rayleigh: " << m_UpwardDiffuseTransmittanceForRayleigh << std::endl;
  os << indent << "Upward Diffuse Transmittance for aerosols: " << m_UpwardDiffuseTransmittanceForAerosol << std::endl;
}

/***************** AtmosphericRadiativeTerms **************************/
/**CONSTRUCTOR. */
AtmosphericRadiativeTerms
::AtmosphericRadiativeTerms()
{
  m_Values.clear();
  m_IsInitialized = false;
}

void
AtmosphericRadiativeTerms
::ValuesInitialization(unsigned int nbChannel)
{
  for (unsigned int j = 0; j < nbChannel; ++j)
    {
    ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
    m_Values.push_back(temp);
    }
  m_IsInitialized = true;
}

/** SET ACCESSORS WITH VECTORS. */
void
AtmosphericRadiativeTerms
::SetIntrinsicAtmosphericReflectances(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());

    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetIntrinsicAtmosphericReflectance(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetSphericalAlbedos(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetSphericalAlbedo(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetTotalGaseousTransmissions(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetTotalGaseousTransmission(vect[nbChannel]);
    }

}
void
AtmosphericRadiativeTerms
::SetDownwardTransmittances(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetDownwardTransmittance(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetUpwardTransmittances(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetUpwardTransmittance(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittances(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetUpwardDiffuseTransmittance(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetUpwardDirectTransmittances(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetUpwardDirectTransmittance(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittancesForRayleigh(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetUpwardDiffuseTransmittanceForRayleigh(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittancesForAerosol(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetUpwardDiffuseTransmittanceForAerosol(vect[nbChannel]);
    }
}
void
AtmosphericRadiativeTerms
::SetWavelengthSpectralBand(const DataVectorType& vect)
{
  if (!m_IsInitialized)
    {
    this->ValuesInitialization(vect.size());
    }
  for (unsigned int nbChannel = 0; nbChannel < vect.size(); ++nbChannel)
    {
    m_Values[nbChannel]->SetWavelengthSpectralBand(vect[nbChannel]);
    }
}

/** SET ACCESSORS WITH INDEX. */
void
AtmosphericRadiativeTerms
::SetValueByIndex(unsigned int id, const ValueType& val)
{
  if (m_IsInitialized)
    {
    if (m_Values.size() < id + 1)
      {
      while (id >=  m_Values.size())
        {
        ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
        m_Values.push_back(temp);
        }
      }
    m_Values[id] = val;
    }
  else
    {
    itkExceptionMacro(<< "Can't insert value before iniatilizing vector value..." << std::endl);
    }
}
void
AtmosphericRadiativeTerms
::SetIntrinsicAtmosphericReflectance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetIntrinsicAtmosphericReflectance(val);
}
void
AtmosphericRadiativeTerms
::SetSphericalAlbedo(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetSphericalAlbedo(val);
}
void
AtmosphericRadiativeTerms
::SetTotalGaseousTransmission(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetTotalGaseousTransmission(val);
}
void
AtmosphericRadiativeTerms
::SetDownwardTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetDownwardTransmittance(val);
}
void
AtmosphericRadiativeTerms
::SetUpwardTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardTransmittance(val);
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittance(val);
}
void
AtmosphericRadiativeTerms
::SetUpwardDirectTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDirectTransmittance(val);
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittanceForRayleigh(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittanceForRayleigh(val);
}
void
AtmosphericRadiativeTerms
::SetUpwardDiffuseTransmittanceForAerosol(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittanceForAerosol(val);
}
void
AtmosphericRadiativeTerms
::SetWavelengthSpectralBand(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetWavelengthSpectralBand(val);
}

/** GET ACCESSORS WITH VECTORS. */
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetIntrinsicAtmosphericReflectances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetSphericalAlbedos() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetSphericalAlbedo();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetTotalGaseousTransmissions() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetTotalGaseousTransmission();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetDownwardTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetDownwardTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardDirectTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDirectTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittancesForRayleigh() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittanceForRayleigh();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittancesForAerosol() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittanceForAerosol();
    }
  return vect;
}
AtmosphericRadiativeTerms::DataVectorType
AtmosphericRadiativeTerms
::GetWavelengthSpectralBand() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetWavelengthSpectralBand();
    }
  return vect;
}

/** GET ACCESSORS WITH INDEX. */
double
AtmosphericRadiativeTerms
::GetIntrinsicAtmosphericReflectance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetIntrinsicAtmosphericReflectance();
}
double
AtmosphericRadiativeTerms
::GetSphericalAlbedo(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetSphericalAlbedo();
}
double
AtmosphericRadiativeTerms
::GetTotalGaseousTransmission(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetTotalGaseousTransmission();
}
double
AtmosphericRadiativeTerms
::GetDownwardTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetDownwardTransmittance();
}
double
AtmosphericRadiativeTerms
::GetUpwardTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardTransmittance();
}
double
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittance();
}
double
AtmosphericRadiativeTerms
::GetUpwardDirectTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDirectTransmittance();
}
double
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittanceForRayleigh(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittanceForRayleigh();
}
double
AtmosphericRadiativeTerms
::GetUpwardDiffuseTransmittanceForAerosol(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittanceForAerosol();
}
double
AtmosphericRadiativeTerms
::GetWavelengthSpectralBand(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetWavelengthSpectralBand();
}

const AtmosphericRadiativeTerms::ValueType
AtmosphericRadiativeTerms
::GetValueByIndex(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id];
}

/**PrintSelf method */
void
AtmosphericRadiativeTerms
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  os << m_Values.size() << " channels:" << std::endl;
  for (unsigned int i = 0; i < m_Values.size(); ++i)
    {
    os << "Channel " << i << " : " << std::endl;
    //ValueType::(os, indent);
    os << indent << "Intrinsic Atmospheric Reflectance     : "
       << m_Values[i]->GetIntrinsicAtmosphericReflectance() << std::endl;
    os << indent << "Shperical Albedo of the Atmosphere    : "
       << m_Values[i]->GetSphericalAlbedo() << std::endl;
    os << indent << "Total Gaseous Transmission            : "
       << m_Values[i]->GetTotalGaseousTransmission() << std::endl;
    os << indent << "Atmosphere Downward Transmittance     : "
       << m_Values[i]->GetDownwardTransmittance() << std::endl;
    os << indent << "Atmosphere Upward Transmittance       : "
       << m_Values[i]->GetUpwardTransmittance() << std::endl;
    os << indent << "Upward Diffuse Transmittance          : "
       << m_Values[i]->GetUpwardDiffuseTransmittance() << std::endl;
    os << indent << "Upward Direct Transmittance           : "
       << m_Values[i]->GetUpwardDirectTransmittance() << std::endl;
    os << indent << "Rayleigh Upward Diffuse Transmittance : "
       << m_Values[i]->GetUpwardDiffuseTransmittanceForRayleigh() << std::endl;
    os << indent << "Aerosols Upward Diffuse Transmittance : "
       << m_Values[i]->GetUpwardDiffuseTransmittanceForAerosol() << std::endl;
    os << indent << "Wavelength           : "
       << m_Values[i]->GetWavelengthSpectralBand() << std::endl;
    }
}

} // end namespace otb
