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

#ifndef otbLeafParameters_h
#define otbLeafParameters_h


#include "itkDataObject.h"
#include "itkMacro.h"
#include "itkObjectFactory.h"
#include "OTBSimulationExport.h"

namespace otb
{
/** \class LeafParameters
   * \brief Data class containing Leaf Parameters.
   *
   * \sa itk::DataObject
 *
 * \ingroup OTBSimulation
 */

class OTBSimulation_EXPORT LeafParameters : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef LeafParameters                Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkTypeMacro(LeafParameters, DataObject);
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

  /** Set/Get equivalent leaf water thickness, EWT (cm)*/
  itkSetMacro(Cw, double);
  itkGetMacro(Cw, double);

  /** Set/Get leaf dry matter, LMA (g/cm2)*/
  itkSetMacro(Cm, double);
  itkGetMacro(Cm, double);

  /** Set/Get leaf structure coef*/
  itkSetMacro(N, double);
  itkGetMacro(N, double);

protected:
  /** Constructor */
  LeafParameters();
  /** Destructor */
  ~LeafParameters() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  LeafParameters(const Self&) = delete;
  void operator=(const Self&) = delete;

  double m_Cab;    // Chlorophyll content
  double m_Car;    // Carotenoid content
  double m_CBrown; // Brown pigment content
  double m_Cw;     // EWT
  double m_Cm;     // LMA
  double m_N;      // structure coef
};
} // end namespace otb

#endif
