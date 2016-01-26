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
#ifndef __otbAtmosphericCorrectionParameters_h
#define __otbAtmosphericCorrectionParameters_h

#include "itkObject.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include "otbObjectList.h"
#include "otbFilterFunctionValues.h"
#include <vector>
#include <iostream>

namespace otb
{
/** \class AtmosphericCorrectionParameters
 *  \brief This class contains all atmospheric correction parameters.
 *
 * Each value can be read in the metadata of an image (ex: SPOT5, ...) or directly set by the user.
 *
 * \ingroup Radiometry
 *
 *
 * \ingroup OTBOpticalCalibration
 */

class ITK_EXPORT AtmosphericCorrectionParameters : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericCorrectionParameters Self;
  typedef itk::Object                 Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericCorrectionParameters, Object);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef enum {NO_AEROSOL = 0, CONTINENTAL = 1, MARITIME = 2, URBAN = 3, DESERTIC = 4} AerosolModelType;

  typedef ObjectList<FilterFunctionValues>                  InternalWavelengthSpectralBandVectorType;
  typedef InternalWavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorType;

  /**
   * Set/Get the atmospheric pressure.
   */
  itkSetMacro(AtmosphericPressure, double);
  itkGetMacro(AtmosphericPressure, double);

  /**
   * Set/Get the water vapor amount.
   */
  itkSetMacro(WaterVaporAmount, double);
  itkGetMacro(WaterVaporAmount, double);

  /**
   * Set/Get the ozone amount.
   */
  itkSetMacro(OzoneAmount, double);
  itkGetMacro(OzoneAmount, double);

  /**
   * Set/Get the aerosol model.
   */
  itkSetEnumMacro(AerosolModel, AerosolModelType);
  itkGetEnumMacro(AerosolModel, AerosolModelType);

  /**
   * Set/Get the aerosol optical.
   */
  itkSetMacro(AerosolOptical, double);
  itkGetMacro(AerosolOptical, double);

  
  /** Get/Set Aeronet file name. */
  itkSetMacro(AeronetFileName, std::string);
  itkGetMacro(AeronetFileName, std::string);


  /** Read the aeronet data and extract aerosol optical and water vapor amount. */
  void ReadAeronetData(const std::string& file, int year, int month, int day, int hour, int minute, double epsi);


  void UpdateAeronetData(const std::string& file, int year, int month, int day, int hour, int minute, double epsi);
  void UpdateAeronetData(const std::string& file, int year, int month, int day, int hour, int minute)
  {
    this->UpdateAeronetData(file, year, month, day, hour, minute, 0.4);
  }
  void UpdateAeronetData(int year, int month, int day, int hour, int minute, double epsi)
  {
    this->UpdateAeronetData(m_AeronetFileName, year, month, day, hour, minute, epsi);
  }
  void UpdateAeronetData(int year, int month, int day, int hour, int minute)
  {
    this->UpdateAeronetData(m_AeronetFileName, year, month, day, hour, minute, 0.4);
  }
  void UpdateAeronetData(const std::string& file, int year, int hour, int minute)
  {
    this->UpdateAeronetData(file, year, m_Month, m_Day, hour, minute, 0.4);
  }
  void UpdateAeronetData(int year, int hour, int minute)
  {
    this->UpdateAeronetData(m_AeronetFileName, year, m_Month, m_Day, hour, minute, 0.4);
  }

  /*void UpdateAeronetData(const std::string& file, int year, int hour, int minute, double epsi) CHRIS
  {
    this->UpdateAeronetData(file, year, m_Month, m_Day, hour, minute, epsi);
  }
  void UpdateAeronetData(const std::string& file, int year, int hour, int minute)
  {
    this->UpdateAeronetData(file, year, m_Month, m_Day, hour, minute, 0.4);
  }*/

  /** Constructor */
  AtmosphericCorrectionParameters();
  /** Destructor */
  ~AtmosphericCorrectionParameters() {}

protected:

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericCorrectionParameters(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** Path to an Aeronet data file, allows computing aerosol optical and water vapor amounts. */
  std::string m_AeronetFileName;
  /** Day */
  int m_Day;
  /** Month */
  int m_Month;
  /** The Atmospheric pressure */
  double m_AtmosphericPressure;
  /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
  double m_WaterVaporAmount;
  /** The Ozone amount (Stratospheric ozone layer content) */
  double m_OzoneAmount;
  /** The Aerosol model */
  AerosolModelType m_AerosolModel;
  /** The Aerosol optical (radiative impact of aerosol for the reference wavelength 550-nm) */
  double m_AerosolOptical;
};

} // end namespace otb

#endif
