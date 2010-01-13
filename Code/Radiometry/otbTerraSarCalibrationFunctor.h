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
#ifndef __otbTerraSarCalibrationFunctor_h
#define __otbTerraSarCalibrationFunctor_h

#include "otbMath.h"
#include "otbTerraSarBrightnessFunctor.h"

#include "itkSize.h"
#include "itkIndex.h"

#include <complex>

#include <otb/ImageNoise.h>

namespace otb
{
namespace Functor
{
/**
   * \class TerraSarCalibrationImageFunctor
   *  \brief Compute sigma naught coefficient from a modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template<class TInput, class TOutput>
class TerraSarCalibrationFunctor
{
public:
  /** Constructor */
  TerraSarCalibrationFunctor();
  /** Destructor */
  virtual ~TerraSarCalibrationFunctor() {};

  /** Typedef to define the noise records map */
  typedef ossimplugins::ImageNoise                  ImageNoiseType;
  
  /** Typedef for image size and index */
  typedef itk::Size<2>                              SizeType;
  typedef itk::Index<2>                             IndexType;

  /** Typedef for the brightness functor */
  typedef TerraSarBrightnessFunctor<double, double> BrightnessFunctorType;

  /**
   * Set the noise record
   */
  void SetNoiseRecord(const ImageNoiseType& record)
  {
    m_NoiseRecord = record;
  }

  /**
   * Get the noise record
   */
  const ImageNoiseType &  GetNoiseRecord() const
  {
    return m_NoiseRecord;
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
  void SetOriginalProductSize( SizeType size ) 
  { 
    m_OriginalProductSize = size; 
  };
  
  /** Get the image size */
  const SizeType & GetOriginalProductSize() const 
  { 
    return m_OriginalProductSize; 
  };
  
  /** Set the UseFastCalibration flag */
  void SetUseFastCalibration( bool b ) 
  { 
    m_UseFastCalibration = b; 
  };
  
  /** Get the UseFastCalibration flag */
  bool GetUseFastCalibration() const 
  { 
    return m_UseFastCalibration; 
  };

  /** Set the ResultsInDecibels flag */
  void SetResultsInDecibels( bool b ) 
  { 
    m_ResultsInDecibels = b; 
  };
  
  /** Get the ResultsInDecibelsMethod flag */
  bool GetResultsInDecibels() const 
  { 
    return m_ResultsInDecibels; 
  };
  
  /** Perform the calibration for one pixel (scalar -> modulus image) */
  inline TOutput operator() (const TInput & inPix, IndexType index);

  /** Perform the calibration for one pixel (complex -> complex image) */
  inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix, IndexType index);

private:
  /** Return the current range position */
  inline double ComputeRangePosition(const IndexType& index) const;

  /** Return the current NEBN value */
  inline double ComputeNoiseEquivalentBetaNaught(double range) const;

  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Radar Brightness functor */
  BrightnessFunctorType m_Brightness;

  /** Noise record vector */
  ImageNoiseType m_NoiseRecord;

  /** Sensor local incident angle in degree */
  double m_LocalIncidentAngle;
  
  /** sin of the LocalIncidentAngle */
  double m_SinLocalIncidentAngle;

  /** Image Size */
  SizeType m_OriginalProductSize;

  /** Fast Calibration Method. If set to true, will consider only the first noise coefficient else,
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates
   *  of the pixel in the image. */
  bool m_UseFastCalibration;

  /** Compute results in dB */
  bool m_ResultsInDecibels;
};

}// end namespace functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationFunctor.txx"
#endif

#endif
