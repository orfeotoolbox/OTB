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

#ifndef otbProSailParameters_h
#define otbProSailParameters_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"


namespace otb
{
/** \class ProSailParameters
   * \brief this class implements the ProSailParameters lib.
   *
   * see http://teledetection.ipgp.jussieu.fr/prosail/
   * \sa itk::DataObject
 *
 * \ingroup OTBSimulation
 */

class ITK_EXPORT ProSailParameters : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef ProSailParameters             Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(ProSailParameters, DataObject);
  itkNewMacro(Self);


  /** Set/Get Chlorophyll content (ug/cm2)*/
  itkSetMacro(Cab, double);
  itkGetMacro(Cab, double);

  /** Set/Get Carotenoid content (ug/cm2)*/
  itkSetMacro(Car, double);
  itkGetMacro(Car, double);

  /** Set/Get Brown pigment content (arbitrary unit)*/
  itkSetMacro(CBrown, double);
  itkGetMacro(CBrown, double);

  /** Set/Get EWT (cm)*/
  itkSetMacro(Cw, double);
  itkGetMacro(Cw, double);

  /** Set/Get LMA (g/cm2)*/
  itkSetMacro(Cm, double);
  itkGetMacro(Cm, double);

  /** Set/Get structure coef*/
  itkSetMacro(N, double);
  itkGetMacro(N, double);

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


protected:
  /** Constructor */
  ProSailParameters();
  /** Destructor */
  virtual ~ProSailParameters(){};
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;


private:
  ProSailParameters(const Self&) = delete;
  void operator=(const Self&) = delete;

  double m_Cab;    // Chlorophyll content
  double m_Car;    // Carotenoid content
  double m_CBrown; // Brown pigment content
  double m_Cw;     // EWT
  double m_Cm;     // LMA
  double m_N;      // structure coef
  double m_LAI;    // leaf area index
  double m_Angl;   // average leaf angle
  double m_PSoil;  // soil coefficient
  double m_Skyl;   // diffuse/direct radiation
  double m_HSpot;  // hot spot
  double m_TTS;    // solar zenith angle
  double m_TTO;    // observer zenith angle
  double m_PSI;    // azimuth
};
} // end namespace otb

#endif
