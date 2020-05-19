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

#ifndef otbPolarimetricData_h
#define otbPolarimetricData_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \enum ArchitectureType
 * \brief This enumeration describes the different architectures we can find in polarimetry.
 *  HH_HV_VH_VV (0), HH_HV_VV (1), HH_VH_VV (2), HH_HV(3), VH_VV (4), HH_VV (5).
 */
typedef enum { HH_HV_VH_VV = 0, HH_HV_VV = 1, HH_VH_VV = 2, HH_HV = 3, VH_VV = 4, HH_VV = 5, UNKNOWN_ARCHITECTURE = 6 } ArchitectureType;

/** \class PolarimetricData
*  \brief This class allows determining the type of architecture we get.
*
*  HH_HV_VH_VV (0), HH_HV_VV (1), HH_VH_VV (2), HH_HV(3), VH_VV (4), HH_VV (5).
*
* \ingroup SARPolarimetry
 *
 * \ingroup OTBPolarimetry
*/

class ITK_EXPORT PolarimetricData : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef PolarimetricData              Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(PolarimetricData, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Determine the kind of architecture of the input */
  void DetermineArchitecture(bool* PresentInputImages);
  void DetermineArchitecture(int NumberOfImages, bool EmissionH, bool EmissionV);

  /** Set/Get ArchitectureType */
  itkSetMacro(ArchitectureType, ArchitectureType);
  itkGetMacro(ArchitectureType, ArchitectureType);

protected:
  /** Constructor */
  PolarimetricData();
  /** Destructor */
  ~PolarimetricData() override
  {
  }
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  PolarimetricData(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Architecture Type */
  ArchitectureType m_ArchitectureType;
};

} // end namespace otb

#endif
