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
#ifndef __otbImageMetadataCorrectionParameters_h
#define __otbImageMetadataCorrectionParameters_h

#include "itkObject.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include "otbObjectList.h"
#include "otbFilterFunctionValues.h"
#include <vector>
#include <iostream>

namespace otb
{
/** \class ImageMetadataCorrectionParameters
 *  \brief This class contains all atmospheric correction parameters.
 *
 * Each value can be read in the metadata of an image (ex: SPOT5, ...) or directly set by the user.
 *
 * \ingroup Radiometry
 *
 */

class ITK_EXPORT ImageMetadataCorrectionParameters : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef ImageMetadataCorrectionParameters Self;
  typedef itk::Object                 Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Type macro */
  itkTypeMacro(ImageMetadataCorrectionParameters, Object);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  typedef ObjectList<FilterFunctionValues>                  InternalWavelengthSpectralBandVectorType;
  typedef InternalWavelengthSpectralBandVectorType::Pointer WavelengthSpectralBandVectorType;

  /**
   * Set/Get the solar zenithal angle.
   */
  itkSetMacro(SolarZenithalAngle, double);
  itkGetMacro(SolarZenithalAngle, double);
  /**
   * Set/Get the solar azimutal angle.
   */
  itkSetMacro(SolarAzimutalAngle, double);
  itkGetMacro(SolarAzimutalAngle, double);

  /**
   * Set/Get the viewing zenithal angle.
   */
  itkSetMacro(ViewingZenithalAngle, double);
  itkGetMacro(ViewingZenithalAngle, double);
  /**
   * Set/Get the viewing azimutal angle.
   */
  itkSetMacro(ViewingAzimutalAngle, double);
  itkGetMacro(ViewingAzimutalAngle, double);

  /**
   * Set/Get the month.
   */
  itkSetClampMacro(Month, unsigned int, 1, 12);
  itkGetMacro(Month, unsigned int);

  /**
   * Set/Get the day.
   */
  itkSetClampMacro(Day, unsigned int, 1, 31);
  itkGetMacro(Day, unsigned int);

  /**
  * Set/Get the year.
  */
  itkSetMacro(Year, unsigned int);
  itkGetMacro(Year, unsigned int);

  /** Get/Set FilterFunction file name. */
  itkSetMacro(FilterFunctionValuesFileName, std::string);
  itkGetMacro(FilterFunctionValuesFileName, std::string);
 
  /**
   * Set/Get the wavelength spectral band.
   */
  void SetWavelengthSpectralBand(const WavelengthSpectralBandVectorType& waveband)
  {
    m_WavelengthSpectralBand = waveband;
  }
  void SetWavelengthSpectralBandWithIndex(unsigned int id, const FilterFunctionValues::Pointer& function)
  {
    if (m_WavelengthSpectralBand->Size() <  id + 1)
      {
      for (unsigned int j = 0; j < (id + 1 - m_WavelengthSpectralBand->Size()); ++j)
        {
        FilterFunctionValues::Pointer temp;
        m_WavelengthSpectralBand->PushBack(temp);
        }
      }
    m_WavelengthSpectralBand->SetNthElement(id, function);
  }
  WavelengthSpectralBandVectorType GetWavelengthSpectralBand() const
  {
    return m_WavelengthSpectralBand;
  }
  const WavelengthSpectralBandVectorType * GetWavelengthSpectralBandRef() const
  {
    return &m_WavelengthSpectralBand;
  }

   /**
   * Read a file that contains filter function values on the 6S format.
   */
  void LoadFilterFunctionValue(const std::string& filename);
  void LoadFilterFunctionValue()
  {
    this->LoadFilterFunctionValue(m_FilterFunctionValuesFileName);
  }

  /** Constructor */
  ImageMetadataCorrectionParameters();
  /** Destructor */
  ~ImageMetadataCorrectionParameters() {}

protected:

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageMetadataCorrectionParameters(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

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
  /** The Year */
  unsigned int m_Year;
  std::string m_FilterFunctionValuesFileName;
 
  /** Wavelength for the each spectral band definition */
  WavelengthSpectralBandVectorType m_WavelengthSpectralBand;
};

} // end namespace otb

#endif
