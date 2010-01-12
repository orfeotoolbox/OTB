/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTerraSarFunctors_h
#define __otbTerraSarFunctors_h

#include "otbMath.h"
#include "itkSize.h"
#include "itkIndex.h"
#include <complex>
#include <otb/ImageNoise.h>

namespace otb
{
namespace Functor
{

/**
   * \class TerraSarBrightnessImageFunctor
   *  \brief Compute the radar brightness from an modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarBrightnessImageFunctor
{
public:
  TerraSarBrightnessImageFunctor();
 virtual ~TerraSarBrightnessImageFunctor() {};
 
 typedef std::vector<double>           DoubleVectorType;
 typedef std::vector<DoubleVectorType> DoubleVectorVectorType;
 typedef itk::Size<2>                  SizeType;
 
 /** Accessors */
 void SetCalibrationFactor( double val ) { m_CalibrationFactor = val; };
 double GetCalibrationFactor() { return m_CalibrationFactor; };
 
 /** We assume that the input pixel is a scalar -> modulus image */
 inline TOutput operator() (const TInput & inPix);
 /** We assume that the input pixel is a complex -> complex image */
 inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix);

private:
  /** Calibration Factor */
  double m_CalibrationFactor;
};




/**
   * \class TerraSarCalibrationImageFunctor
   *  \brief Compute sigma naught coefficient from a modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template<class TInput, class TOutput>
class TerraSarCalibrationImageFunctor
{
public:
  /** Constructor */
  TerraSarCalibrationImageFunctor();
  /** Destructor */
  virtual ~TerraSarCalibrationImageFunctor() {};

  /** Typedef to define the noise records map */
  typedef ossimplugins::ImageNoise                       ImageNoiseType;
  typedef std::pair<double,ImageNoiseType>               NoiseRecordType;
  typedef std::vector<NoiseRecordType >                  NoiseRecordVectorType;

  /** Typedef for image size and index */
  typedef itk::Size<2>                                   SizeType;
  typedef itk::Index<2>                                  IndexType;

  /** Typedef for the brightness functor */
  typedef TerraSarBrightnessImageFunctor<double, double> BrightnessFunctorType;

  /**
   * Add a new noise record to the map 
   */
  void AddNoiseRecord(double utcAcquisitionTime, const ImageNoiseType& record)
  {
    // Create the pair
    NoiseRecordType newRecord(utcAcquisitionTime,record);

    // Add it to the list
    m_NoiseRecords.push_back(newRecord);
  }

  /** returns the number of noise records */
  unsigned int GetNumberOfNoiseRecords() const
  {
    return m_NoiseRecords.size();
  }
  
  /**
   * Clear all noise records
   */
  void ClearNoiseRecords()
  {
    m_NoiseRecords.clear();
  }

  /** Set the calibration factor */
  void SetCalibrationFactor( double val ) 
  { 
    m_CalibrationFactor =  val; 
    m_Brightness.SetCalibrationFactor(val); 
  };
  
  /** Get the calibration factor */
  double GetCalibrationFactor() const 
  { 
    return m_CalibrationFactor; 
  };

  /** Set the local incidence angle */
  void SetLocalIncidentAngle( double val )
  {
    m_LocalIncidentAngle = val;
    m_SinLocalIncidentAngle = vcl_sin(m_LocalIncidentAngle*CONST_PI_180);
  };
  
  /** Get the local incidence angle */
  double GetLocalIncidentAngle() const 
  { 
    return m_LocalIncidentAngle; 
  };
  
  /** Set the image size */
  void SetImageSize( SizeType size ) 
  { 
    m_ImageSize = size; 
  };
  
  /** Get the image size */
  const SizeType & GetImageSize() const 
  { 
    return m_ImageSize; 
  };
  
  /** Set the UseFastCalibrationMethod flag */
  void SetUseFastCalibrationMethod( bool b ) 
  { 
    m_UseFastCalibrationMethod = b; 
  };
  
  /** Get the UseFastCalibrationMethod flag */
  bool GetUseFastCalibrationMethod() const 
  { 
    return m_UseFastCalibrationMethod; 
  };
  
  /** Set The Pulse Repetition Frequency (PRF) */
  void SetPRF( double val ) 
  { 
    m_PRF = val; 
    m_InvPRF = 1./m_PRF; 
  };

  /** Get the PRF */
  double GetPRF() const 
  { 
    return m_PRF; 
  };

  /** Set the acquisition start time */
  void SetAcquisitionStartTime(double startTime)
  {
    m_AcquisitionStartTime = startTime;
  }

  /** Get the acqusition start time */
  double GetAcquisitionStartTime(void) const
  {
    return m_AcquisitionStartTime;
  }

  /** Perform the calibration for one pixel (scalar -> modulus image) */
  inline TOutput operator() (const TInput & inPix, IndexType index);

  /** Perform the calibration for one pixel (complex -> complex image) */
  inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix, IndexType index);

private:
  /** Return the closest noise record in the noise record vector */
  inline const ImageNoiseType & FindClosestNoiseRecord(double utcTime) const;

  /** Return the current azimuth position (current acquisition time) */
  inline double ComputeAzimuthPosition(const IndexType& index) const;

  /** Return the current range position */
  inline double ComputeRangePosition(const ImageNoiseType& noise, const IndexType& index) const;

  /** Return the current NEBN value */
  inline double ComputeNoiseEquivalentBetaNaught(const ImageNoiseType& noise, double range) const;

  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Radar Brightness functor */
  BrightnessFunctorType m_Brightness;

  /** Noise record vector */
  NoiseRecordVectorType m_NoiseRecords;

  /** Sensor local incident angle in degree */
  double m_LocalIncidentAngle;
  
  /** sin of the LocalIncidentAngle */
  double m_SinLocalIncidentAngle;

  /** Image Size */
  SizeType m_ImageSize;

  /** Fast Calibration Method. If set to true, will consider only the first noise coefficient else,
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates
   *  of the pixel in the image. */
  bool m_UseFastCalibrationMethod;
  
  /** Acquisition start time */
  double m_AcquisitionStartTime;

  /** Pulse Repetition Frequency */
  double m_PRF;

  /** Inverse Pulse Repetition Frequency */
  double m_InvPRF;
};

}// end namespace functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarFunctors.txx"
#endif

#endif
