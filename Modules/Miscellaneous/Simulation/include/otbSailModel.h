/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSailModel_h
#define otbSailModel_h

#include "OTBSimulationExport.h"
#include "otbSpectralResponse.h"
#include "otbDataSpecP5B.h"
#include "otbSimulationStep2Base.h"
#include "otbSoilDataBase.h"
#include <string>
#include <memory>

namespace otb
{
/** \class SailModel
   * \brief This class implements the SAIL model (see http://teledetection.ipgp.jussieu.fr/prosail/).
   *
   * Hemispherical and viewing reflectances and absorptances are computed from 400 to 2500 nm (step 1nm).
   * Input Reflectance and transmittance must be spectrum (otbSpectralResponse) from 400 nm to 2500 nm with step 1nm.
   * \sa itk::ProcessObject
   * \sa SimulationStep2Base
 *
 * \ingroup OTBSimulation
 */


class OTBSimulation_EXPORT SailModel : public SimulationStep2Base
{
public:
  /** Standard class typedefs */
  typedef SailModel                     Self;
  typedef SimulationStep2Base           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef Superclass::SpectralResponseType SpectralResponseType;
  typedef std::vector<double>              VectorType;
  typedef Superclass::ParametersType       ParametersType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(SailModel, ProcessObject);


  /** Set Inputs */
  void SetReflectance(const SpectralResponseType* object) override;
  SpectralResponseType* GetReflectance();

  void SetTransmittance(const SpectralResponseType* object) override;
  SpectralResponseType* GetTransmittance();

  /** Set parameters */
  /** Set/Get leaf area index */
  itkSetMacro(LAI, double);
  itkGetMacro(LAI, double);

  /** Set/Get average leaf angle (deg)*/
  itkSetMacro(Angl, double);
  itkGetMacro(Angl, double);

  /** Set/Get soil coefficient*/
  itkSetMacro(PSoil, double);
  itkGetMacro(PSoil, double);

  /** Set/Get diffuse/direct radiation*/
  itkSetMacro(Skyl, double);
  itkGetMacro(Skyl, double);

  /** Set/Get hot spot */
  itkSetMacro(HSpot, double);
  itkGetMacro(HSpot, double);

  /** Set/Get solar zenith angle (deg) */
  itkSetMacro(TTS, double);
  itkGetMacro(TTS, double);

  /** Set/Get observer zenith angle (deg) */
  itkSetMacro(TTO, double);
  itkGetMacro(TTO, double);

  /** Set/Get azimuth(deg) */
  itkSetMacro(PSI, double);
  itkGetMacro(PSI, double);

  /** Get the computed fcover */
  itkGetMacro(FCoverView, double);

  /** Use an external soil DB */
  void UseExternalSoilDB(std::shared_ptr<SoilDataBase> SoilDB, size_t SoilIndex);

  /** GenerateData */
  void GenerateData() override;

  /** Get Output */
  virtual SpectralResponseType* GetViewingReflectance();
  virtual SpectralResponseType* GetHemisphericalReflectance();
  virtual SpectralResponseType* GetViewingAbsorptance();
  virtual SpectralResponseType* GetHemisphericalAbsorptance();

  const ParametersType GetInput();
  void                 SetInput(const ParametersType&);
  using Superclass::SetInput;


protected:
  /** Constructor */
  SailModel();
  /** Destructor */
  ~SailModel() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType) override;
  using Superclass::MakeOutput;

  /** Compute Leaf Angle Distribution */
  void Calc_LIDF(const double a, VectorType& lidf) const;
  void Campbell(const double ala, VectorType& freq) const;

  /** J functions */
  double Jfunc1(const double k, const double l, const double t) const;
  double Jfunc2(const double k, const double l, const double t) const;
  double Jfunc3(const double k, const double l, const double t) const;
  /** Volscatt */
  void Volscatt(const double tts, const double tto, const double psi, const double ttl, VectorType& result) const;

private:
  SailModel(const Self&) = delete;
  void operator=(const Self&) = delete;

  double                        m_LAI;         // leaf area index
  double                        m_Angl;        // average leaf angle
  double                        m_PSoil;       // soil coefficient
  double                        m_Skyl;        // diffuse/direct radiation
  double                        m_HSpot;       // hot spot
  double                        m_TTS;         // solar zenith angle
  double                        m_TTO;         // observer zenith angle
  double                        m_PSI;         // azimuth
  double                        m_FCoverView;  // fCover in the viewing direction
  bool                          m_UseSoilFile; // use a soil file instead of DataSpecP5B
  size_t                        m_SoilIndex;   // which soil in the soil file
  std::shared_ptr<SoilDataBase> m_SoilDataBase;
};

} // end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
// #include "otbSailModel.cxx"
#endif

#endif
