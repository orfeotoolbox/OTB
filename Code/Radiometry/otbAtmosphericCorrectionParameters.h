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

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkMacro.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include <vector>
#include <iostream>

namespace otb
{
  /** \class FilterFunctionValues
   *  \brief This class contains the values of the filter function for the processed spectral band.
   *
   *  The step between 2 values is 0.0025µm. The class contains the min and the max value.Those value can be directly read from the image metadatas.
   */
  class ITK_EXPORT FilterFunctionValues : public itk::DataObject
    {
    public:
      /** Standard typedefs */
      typedef FilterFunctionValues          Self;
      typedef itk::DataObject               Superclass;
      typedef itk::SmartPointer<Self>       Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;

      /** Type macro */
      itkTypeMacro(FilterFunctionValues,DataObject);

      /** Creation through object factory macro */
      itkNewMacro(Self);

      typedef double WavelenghtSpectralBandType;
      typedef std::vector<WavelenghtSpectralBandType>    ValuesVectorType;

      /** Set vector that contains the filter function value. */
      void SetFilterFunctionValues(const ValuesVectorType & vect)
  {
    m_FilterFunctionValues = vect;
    this->Modified();
  };
      /** Get vector that contains the filter function value. */
      const ValuesVectorType & GetFilterFunctionValues() const { return m_FilterFunctionValues; };
      /** Get vector that contains the filter function value 6S. */
      void SetFilterFunctionValues6S(const ValuesVectorType & vect)
      {
          m_FilterFunctionValues6S = vect;
    this->Modified();
      };
      /** Get vector that contains the filter function value 6S. */
      const ValuesVectorType & GetFilterFunctionValues6S() const { return m_FilterFunctionValues6S; };

      /** Set minimum spectral value. */
      itkSetMacro(MinSpectralValue,WavelenghtSpectralBandType);
      /** Get minimum spectral value. */
      itkGetMacro(MinSpectralValue,WavelenghtSpectralBandType);
      /** Set maximum spectral value. This value is automatically computed.*/
      itkSetMacro(MaxSpectralValue,WavelenghtSpectralBandType);
      /** Get maximum spectral value. This value is automatically computed.*/
      itkGetMacro(MaxSpectralValue,WavelenghtSpectralBandType);
      /** Set user step between each wavelenght spectral band values. */
      itkSetMacro(UserStep,WavelenghtSpectralBandType);
      /** Get user step between each wavelenght spectral band values. */
      itkGetMacro(UserStep,WavelenghtSpectralBandType);

    protected:
      /** Constructor */
      FilterFunctionValues();
      /** Destructor */
      ~FilterFunctionValues(){};

      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;


    private:
      FilterFunctionValues(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented


      /** Vector that contains the filter function value. */
      ValuesVectorType m_FilterFunctionValues;
      /** Vector that contains the filter function value in 6S format (step of 0.0025µm).
        * There values a computed by 6S. If the UserStep is 0.0025µm, then m_FilterFunctionValues is identical as m_FilterFunctionValues6S
        */
      ValuesVectorType m_FilterFunctionValues6S;
      /** Minimum spectral value (in µm). */
      WavelenghtSpectralBandType m_MinSpectralValue;
      /** Maximum spectral value (in µm). */
      WavelenghtSpectralBandType m_MaxSpectralValue;
      /** User step between each wavelenght spectral band values. (in µm) */
      WavelenghtSpectralBandType m_UserStep;
    };




/** \class AtmosphericCorrectionParameters
 *  \brief This class contains all atmospheric correction parameters.
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
  typedef std::vector<FilterFunctionValues::Pointer>                               WavelenghtSpectralBandVectorType;
  //typedef itk::VariableSizeMatrix<WavelenghtSpectralBandType>    WavelenghtSpectralBandMatrixType;

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

  /**
   * Set/Get the wavelenght spectral band.
   */
  void SetWavelenghtSpectralBand( const WavelenghtSpectralBandVectorType & waveband){ m_WavelenghtSpectralBand = waveband; };
  void SetWavelenghtSpectralBandWithIndex( unsigned int id, const FilterFunctionValues::Pointer & function)
    {
      if (m_WavelenghtSpectralBand.size() <  id+1)
  {
    for(unsigned int j=0; j<(id+1-m_WavelenghtSpectralBand.size());j++)
      {
        FilterFunctionValues::Pointer temp;
        m_WavelenghtSpectralBand.push_back(temp);
      }
  }
      m_WavelenghtSpectralBand[id] = function;
    };
  WavelenghtSpectralBandVectorType GetWavelenghtSpectralBand(){ return m_WavelenghtSpectralBand; };
  WavelenghtSpectralBandVectorType * GetWavelenghtSpectralBandRef(){ return &m_WavelenghtSpectralBand; };

  /** Constructor */
  AtmosphericCorrectionParameters();
  /** Destructor */
  ~AtmosphericCorrectionParameters(){};

protected:

  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  AtmosphericCorrectionParameters(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

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
  WavelenghtSpectralBandVectorType m_WavelenghtSpectralBand;

};

} // end namespace otb

#endif


