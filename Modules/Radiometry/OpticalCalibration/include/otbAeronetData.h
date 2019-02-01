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

#ifndef otbAeronetData_h
#define otbAeronetData_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include "OTBOpticalCalibrationExport.h"
#include <string>

namespace otb
{
/**
 * \class AeronetData
 * \brief This class is a data structure designed to store Aeronet data
 * extracted from a aeronet file.
 *
 * \sa AeronetFileReader
 *
 * \ingroup OTBOpticalCalibration
 */
class OTBOpticalCalibration_EXPORT AeronetData :  public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef AeronetData                   Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AeronetData, itk::DataObject);

  /** Set/Get Solar Zenith Angle data */
  itkSetMacro(SolarZenithAngle, double);
  itkGetMacro(SolarZenithAngle, double);
  /**  Set/Get Angst data */
  itkSetMacro(AngstromCoefficient, double);
  itkGetMacro(AngstromCoefficient, double);
  /**  Set/Get Water data */
  itkSetMacro(Water, double);
  itkGetMacro(Water, double);
  /**  Set/Get AerosolOpticalThickness data */
  itkSetMacro(AerosolOpticalThickness, double);
  itkGetMacro(AerosolOpticalThickness, double);
  /** Statistic : Standard deviation data */
  itkSetMacro(StdDev, double);
  itkGetMacro(StdDev, double);

  /**  Set/Get Cloud estimation data */
  itkSetMacro(CloudEstimation, bool);
  itkGetMacro(CloudEstimation, bool);

  /** Set/Get Data Date */
  itkSetMacro(DataDate, std::string);
  itkGetMacro(DataDate, std::string);

  /** Set/Get Number of date used */
  itkSetMacro(NumberOfDateUsed, unsigned int);
  itkGetMacro(NumberOfDateUsed, unsigned int);
  /** Statistic : Epsilon date */
  itkSetMacro(EpsilonDate, double);
  itkGetMacro(EpsilonDate, double);

protected:
  /** Constructor */
  AeronetData();
  /** Destructor */
  ~AeronetData() override {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  /** Angstrom coefficient */
  double m_AngstromCoefficient;
  /** Water data */
  double m_Water;
  /** Aerosol optical thickness data */
  double m_AerosolOpticalThickness;
  /** Cloud estimation data */
  bool m_CloudEstimation;

  /** Solar Zenith Angle */
  double m_SolarZenithAngle;

  /** Statistic : Standard deviation data */
  double m_StdDev;

  /** Data Date */
  std::string m_DataDate;
  /** Epsilon to select time */
  double m_EpsilonDate;
  /** Number of date used */
  unsigned int m_NumberOfDateUsed;

};

} // end namespace otb

#endif
