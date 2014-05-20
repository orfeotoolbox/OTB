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

#include "otbAtmosphericRadiativeTerms2.h"

namespace otb
{
/************** AtmosphericRadiativeTermsSingleChannel2******************/
/** Constructor */
AtmosphericRadiativeTermsSingleChannel2::AtmosphericRadiativeTermsSingleChannel2() :
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
AtmosphericRadiativeTermsSingleChannel2
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
AtmosphericRadiativeTerms2
::AtmosphericRadiativeTerms2()
{
  m_Values.clear();
  m_IsInitialized = false;
}

void
AtmosphericRadiativeTerms2
::ValuesInitialization(unsigned int nbChannel)
{
  for (unsigned int j = 0; j < nbChannel; ++j)
    {
    ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
    m_Values.push_back(temp);
    }
  m_IsInitialized = true;
}

/** SET ACCESSORS WITH VECTORS. */
void
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
::SetValueByIndex(unsigned int id, const ValueType& val)
{
  if (m_IsInitialized)
    {
    if (m_Values.size() < id + 1)
      {
      while (id >=  m_Values.size())
        {
        ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
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
AtmosphericRadiativeTerms2
::SetIntrinsicAtmosphericReflectance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetIntrinsicAtmosphericReflectance(val);
}
void
AtmosphericRadiativeTerms2
::SetSphericalAlbedo(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetSphericalAlbedo(val);
}
void
AtmosphericRadiativeTerms2
::SetTotalGaseousTransmission(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetTotalGaseousTransmission(val);
}
void
AtmosphericRadiativeTerms2
::SetDownwardTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetDownwardTransmittance(val);
}
void
AtmosphericRadiativeTerms2
::SetUpwardTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardTransmittance(val);
}
void
AtmosphericRadiativeTerms2
::SetUpwardDiffuseTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittance(val);
}
void
AtmosphericRadiativeTerms2
::SetUpwardDirectTransmittance(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDirectTransmittance(val);
}
void
AtmosphericRadiativeTerms2
::SetUpwardDiffuseTransmittanceForRayleigh(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittanceForRayleigh(val);
}
void
AtmosphericRadiativeTerms2
::SetUpwardDiffuseTransmittanceForAerosol(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetUpwardDiffuseTransmittanceForAerosol(val);
}
void
AtmosphericRadiativeTerms2
::SetWavelengthSpectralBand(unsigned int id, const double& val)
{
  if (m_Values.size() < id + 1)
    {
    while (id >=  m_Values.size())
      {
      ValueType temp = AtmosphericRadiativeTermsSingleChannel2::New();
      m_Values.push_back(temp);
      }
    }
  m_Values[id]->SetWavelengthSpectralBand(val);
}

/** GET ACCESSORS WITH VECTORS. */
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetIntrinsicAtmosphericReflectances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetIntrinsicAtmosphericReflectance();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetSphericalAlbedos() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetSphericalAlbedo();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetTotalGaseousTransmissions() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetTotalGaseousTransmission();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetDownwardTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetDownwardTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetUpwardTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetUpwardDirectTransmittances() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDirectTransmittance();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittancesForRayleigh() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittanceForRayleigh();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittancesForAerosol() const
{
  DataVectorType vect(m_Values.size(), 0);
  for (unsigned int nbChannel = 0; nbChannel < m_Values.size(); ++nbChannel)
    {
    vect[nbChannel] = m_Values[nbChannel]->GetUpwardDiffuseTransmittanceForAerosol();
    }
  return vect;
}
AtmosphericRadiativeTerms2::DataVectorType
AtmosphericRadiativeTerms2
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
AtmosphericRadiativeTerms2
::GetIntrinsicAtmosphericReflectance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetIntrinsicAtmosphericReflectance();
}
double
AtmosphericRadiativeTerms2
::GetSphericalAlbedo(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetSphericalAlbedo();
}
double
AtmosphericRadiativeTerms2
::GetTotalGaseousTransmission(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetTotalGaseousTransmission();
}
double
AtmosphericRadiativeTerms2
::GetDownwardTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetDownwardTransmittance();
}
double
AtmosphericRadiativeTerms2
::GetUpwardTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardTransmittance();
}
double
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittance();
}
double
AtmosphericRadiativeTerms2
::GetUpwardDirectTransmittance(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDirectTransmittance();
}
double
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittanceForRayleigh(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittanceForRayleigh();
}
double
AtmosphericRadiativeTerms2
::GetUpwardDiffuseTransmittanceForAerosol(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetUpwardDiffuseTransmittanceForAerosol();
}
double
AtmosphericRadiativeTerms2
::GetWavelengthSpectralBand(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id]->GetWavelengthSpectralBand();
}

const AtmosphericRadiativeTerms2::ValueType
AtmosphericRadiativeTerms2
::GetValueByIndex(unsigned int id) const
{
  if (id >= m_Values.size()) itkExceptionMacro(<< " band index out of bounds: " << id
                                               << " for " << m_Values.size() << " bands.");
  return m_Values[id];
}

/**PrintSelf method */
void
AtmosphericRadiativeTerms2
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
