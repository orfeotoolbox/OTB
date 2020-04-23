/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbAtmosphericRadiativeTerms_h
#define otbAtmosphericRadiativeTerms_h

#include "OTBOpticalCalibrationExport.h"
#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include <vector>

namespace otb
{

/** \class AtmosphericRadiativeTermsSingleChannel
 *  \brief This class contains all atmospheric radiative terms for one channel.
 *
 * Each value can be deducted from the atmospheric correction parameters (using 6S)
 * or directly set by the user.
 *
 * \ingroup Radiometry
 *
 * \ingroup OTBOpticalCalibration
 */

class OTBOpticalCalibration_EXPORT AtmosphericRadiativeTermsSingleChannel : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericRadiativeTermsSingleChannel Self;
  typedef itk::DataObject                        Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericRadiativeTermsSingleChannel, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /**
   * Set/Get the intrinsic atmospheric reflectance.
   */
  itkSetMacro(IntrinsicAtmosphericReflectance, double);
  itkGetMacro(IntrinsicAtmosphericReflectance, double);

  /**
   * Set/Get the spherical albedo of the atmosphere.
   */
  itkSetMacro(SphericalAlbedo, double);
  itkGetMacro(SphericalAlbedo, double);
  /**
   * Set/Get the total gaseous transmission.
   */
  itkSetMacro(TotalGaseousTransmission, double);
  itkGetMacro(TotalGaseousTransmission, double);

  /**
  * Set/Get the downward transmittance of the atmosphere.
  */
  itkSetMacro(DownwardTransmittance, double);
  itkGetMacro(DownwardTransmittance, double);

  /**
   * Set/Get the upward transmittance of the atmosphere.
   */
  itkSetMacro(UpwardTransmittance, double);
  itkGetMacro(UpwardTransmittance, double);

  /**
   * Set/Get the upward diffuse transmittance
   */
  itkSetMacro(UpwardDiffuseTransmittance, double);
  itkGetMacro(UpwardDiffuseTransmittance, double);

  /**
   * Set/Get the upward direct transmittance
   */
  itkSetMacro(UpwardDirectTransmittance, double);
  itkGetMacro(UpwardDirectTransmittance, double);

  /**
   * Set/Get the upward diffuse transmittance for rayleigh
   */
  itkSetMacro(UpwardDiffuseTransmittanceForRayleigh, double);
  itkGetMacro(UpwardDiffuseTransmittanceForRayleigh, double);

  /**
   * Set/Get the upward diffuse transmittance for aerosols
   */
  itkSetMacro(UpwardDiffuseTransmittanceForAerosol, double);
  itkGetMacro(UpwardDiffuseTransmittanceForAerosol, double);

  /**
   * Set/Get the wavelength
   */
  itkSetMacro(WavelengthSpectralBand, double);
  itkGetMacro(WavelengthSpectralBand, double);

protected:
  /** Constructor */
  AtmosphericRadiativeTermsSingleChannel();
  /** Destructor */
  ~AtmosphericRadiativeTermsSingleChannel() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  AtmosphericRadiativeTermsSingleChannel(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The intrinsic atmospheric reflectance. */
  double m_IntrinsicAtmosphericReflectance;

  /** The spherical albedo of the atmosphere.*/
  double m_SphericalAlbedo;

  /**The total gaseous transmission (for all species). */
  double m_TotalGaseousTransmission;

  /** The downward transmittance. */
  double m_DownwardTransmittance;

  /** The upward transmittance. */
  double m_UpwardTransmittance;

  /** The upward diffuse transmittance. */
  double m_UpwardDiffuseTransmittance;

  /** The upward direct transmittance. */
  double m_UpwardDirectTransmittance;

  /** The upward diffuse transmittance for rayleigh. */
  double m_UpwardDiffuseTransmittanceForRayleigh;

  /** The upward diffuse transmittance for aerosols. */
  double m_UpwardDiffuseTransmittanceForAerosol;

  /** Wavelength associated with the data: not used in the computation, for reference only */
  double m_WavelengthSpectralBand;
};

/** \class AtmosphericRadiativeTerms
 *  \brief This class is a vector of AtmosphericRadiativeTermsSingleChannel,
 *         it contains all atmospheric radiative terms for each studied channel.
 *
 * \ingroup AtmosphericRadiativeTermSingleChannel
 * \ingroup Radiometry
 *
 * \ingroup OTBOpticalCalibration
 */

class OTBOpticalCalibration_EXPORT AtmosphericRadiativeTerms : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericRadiativeTerms     Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericRadiativeTerms, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef AtmosphericRadiativeTermsSingleChannel::Pointer ValueType;
  typedef std::vector<ValueType>                          VectorValueType;
  typedef std::vector<double>                             DataVectorType;
  typedef DataVectorType::size_type                       DataVectorSizeType;
  /**
   * Set/Get the values.
   */
  void SetValues(const VectorValueType& val)
  {
    m_Values = val;
    this->Modified();
  }
  VectorValueType& GetValues()
  {
    return m_Values;
  }
  const VectorValueType& GetValues() const
  {
    return m_Values;
  }

  /** Set/Get the data classified by channel. */
  /** Set methods with vectors. */
  void SetIntrinsicAtmosphericReflectances(const DataVectorType& vect);
  void SetSphericalAlbedos(const DataVectorType& vect);
  void SetTotalGaseousTransmissions(const DataVectorType& vect);
  void SetDownwardTransmittances(const DataVectorType& vect);
  void SetUpwardTransmittances(const DataVectorType& vect);

  void SetUpwardDiffuseTransmittances(const DataVectorType& vect);
  void SetUpwardDirectTransmittances(const DataVectorType& vect);
  void SetUpwardDiffuseTransmittancesForRayleigh(const DataVectorType& vect);
  void SetUpwardDiffuseTransmittancesForAerosol(const DataVectorType& vect);
  void SetWavelengthSpectralBand(const DataVectorType& vect);

  /** Set methods with index. */
  void SetValueByIndex(unsigned int id, const ValueType& val);
  void SetIntrinsicAtmosphericReflectance(unsigned int id, const double& val);
  void SetSphericalAlbedo(unsigned int id, const double& val);
  void SetTotalGaseousTransmission(unsigned int id, const double& val);
  void SetDownwardTransmittance(unsigned int id, const double& val);
  void SetUpwardTransmittance(unsigned int id, const double& val);

  void SetUpwardDiffuseTransmittance(unsigned int id, const double& val);
  void SetUpwardDirectTransmittance(unsigned int id, const double& val);
  void SetUpwardDiffuseTransmittanceForRayleigh(unsigned int id, const double& val);
  void SetUpwardDiffuseTransmittanceForAerosol(unsigned int id, const double& val);
  void SetWavelengthSpectralBand(unsigned int id, const double& val);

  /** Get methods with vectors. */
  DataVectorType GetIntrinsicAtmosphericReflectances() const;
  DataVectorType GetSphericalAlbedos() const;
  DataVectorType GetTotalGaseousTransmissions() const;
  DataVectorType GetDownwardTransmittances() const;
  DataVectorType GetUpwardTransmittances() const;

  DataVectorType GetUpwardDiffuseTransmittances() const;
  DataVectorType GetUpwardDirectTransmittances() const;
  DataVectorType GetUpwardDiffuseTransmittancesForRayleigh() const;
  DataVectorType GetUpwardDiffuseTransmittancesForAerosol() const;
  DataVectorType GetWavelengthSpectralBand() const;

  /** Get methods with index. */
  double GetIntrinsicAtmosphericReflectance(unsigned int id) const;
  double GetSphericalAlbedo(unsigned int id) const;
  double GetTotalGaseousTransmission(unsigned int id) const;
  double GetDownwardTransmittance(unsigned int id) const;
  double GetUpwardTransmittance(unsigned int id) const;
  double GetUpwardDiffuseTransmittance(unsigned int id) const;
  double GetUpwardDirectTransmittance(unsigned int id) const;
  double GetUpwardDiffuseTransmittanceForRayleigh(unsigned int id) const;
  double GetUpwardDiffuseTransmittanceForAerosol(unsigned int id) const;
  double GetWavelengthSpectralBand(unsigned int id) const;

  const ValueType GetValueByIndex(unsigned int id) const;

  /** Initialization method.*/
  void ValuesInitialization(DataVectorSizeType nbChannel);

protected:
  /** Constructor */
  AtmosphericRadiativeTerms();
  /** Destructor */
  ~AtmosphericRadiativeTerms() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  AtmosphericRadiativeTerms(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** The vector containing each channel information. */
  VectorValueType m_Values;
  /** Boolean to know if m_Values has been initialized. */
  bool m_IsInitialized;
};

} // end namespace otb

#endif
