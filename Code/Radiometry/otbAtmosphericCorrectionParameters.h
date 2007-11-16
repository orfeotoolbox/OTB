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
#ifndef _otbAtmosphericCorrectionParameters_h
#define _otbAtmosphericCorrectionParameters_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include "itkVariableSizeMatrix.h"

namespace otb 
{
/** \class AtmosphericCorrectionParameters
 *  \brief This class contain all atmospheric correction parameters.
 *
 * Each value can be read in the metadata of an image (ex: SPOT5, ...) or directly set by the user.
 */

class ITK_EXPORT AtmosphericCorrectionParameters : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef AtmosphericCorrectionParameters Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(AtmosphericCorrectionParameters,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef enum {NO_AEROSOL=0,CONTINENTAL=1,MARITIME=2,URBAN=3,DESERTIC=5} AerosolModelType;
  typedef float WavelenghtSpectralBandType;
  typedef itk::VariableSizeMatrix<WavelenghtSpectralBandType>    WavelenghtSpectralBandMatrixType;
  /**
   * Set/Get the solar zenithal angle.
   */
  itkSetMacro(SolarZenithalAngle,double); 
  itkGetMacro(SolarZenithalAngle,double); 
  /**
   * Set/Get the solar azimutal angle.
   */
  itkSetMacro(SolarAzimutalAngle,double); 
  itkGetMacro(SolarAzimutalAngle,double); 

  /**
   * Set/Get the viewing zenithal angle.
   */
  itkSetMacro(ViewingZenithalAngle,double); 
  itkGetMacro(ViewingZenithalAngle,double); 
  /**
   * Set/Get the viewing azimutal angle.
   */
  itkSetMacro(ViewingAzimutalAngle,double); 
  itkGetMacro(ViewingAzimutalAngle,double); 

  /**
   * Set/Get the month.
   */
  itkSetClampMacro(Month,unsigned int,1,12); 
  itkGetMacro(Month,unsigned int); 

  /**
   * Set/Get the day.
   */
  itkSetClampMacro(Day,unsigned int,1,31); 
  itkGetMacro(Day,unsigned int); 

  /**
   * Set/Get the atmospheric pressure.
   */
  itkSetMacro(AtmosphericPressure,double); 
  itkGetMacro(AtmosphericPressure,double); 

  /**
   * Set/Get the water vapor amount.
   */
  itkSetMacro(WaterVaporAmount,double); 
  itkGetMacro(WaterVaporAmount,double); 

  /**
   * Set/Get the ozone amount.
   */
  itkSetMacro(OzoneAmount,double); 
  itkGetMacro(OzoneAmount,double); 

  /**
   * Set/Get the aerosol model.
   */
  itkSetEnumMacro(AerosolModel,AerosolModelType); 
  itkGetEnumMacro(AerosolModel,AerosolModelType); 

  /**
   * Set/Get the aerosol optical.
   */
  itkSetMacro(AerosolOptical,double); 
  itkGetMacro(AerosolOptical,double); 


protected:
  /** Constructor */
  AtmosphericCorrectionParameters();
  /** Destructor */
  ~AtmosphericCorrectionParameters(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericCorrectionParameters(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented

  /** The Solar zenithal angle */
  double m_SolarZenithalAngle;
  /** The Solar azimutal angle */
  double m_SolarAzimutalAngle;
  /** The Viewing zenithal angle */
  double m_ViewingZenithalAngle;
  /** The Viewing azimutal angle */
  double m_ViewingAzimutalAngle;
  /** The Month */
  unsigned int m_Month;
  /** The Day (in the month) */
  unsigned int m_Day;
  /** The Atmospheric pressure */
  double m_AtmosphericPressure; 
  /** The Water vapor amount (Total water vapor content over vertical atmospheric column) */
  double m_WaterVaporAmount;
  /** The Ozone amount (Stratospheric ozone layer content) */
  double m_OzoneAmount;
  /** The Aerosol model */
  AerosolModelType m_AerosolModel;
  /** The Aerosol optical (radiative impact of aerosol for the reference wavelenght 550-nm) */
  double m_AerosolOptical;
  
  /** Wavelenght for the each spectral band definition */
  WavelenghtSpectralBandMatrixType m_WavelenghtSpectralBand;
  
  /** Step between each wavelenght spectral band values */
  static const double m_StepOfWavelenghtSpectralBandValues;
};

} // end namespace otb

#endif


