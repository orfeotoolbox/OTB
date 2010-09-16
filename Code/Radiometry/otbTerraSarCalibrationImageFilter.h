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
#ifndef __otbTerraSarCalibrationImageFilter_h
#define __otbTerraSarCalibrationImageFilter_h


#include "vcl_deprecated_header.h"

#include "itkImageToImageFilter.h"
#include "otbTerraSarCalibrationFunctor.h"
#include "itkMetaDataDictionary.h"

namespace otb
{
/** \class TerraSarCalibrationImageFilter
 *  \brief Calculates the calibration of TerraSar sensor images.
 *
 * Implementation of the calibration algorithm has been made following
 * Infoterra documentation  "Radiometric Calibration of TerraSAR-X
 * Data".
 *
 * This filter computes the sigma naught (\f$ \sigma^{0} \f$) using the following formula:
 * \f[\sigma^{0} = ( k_{s} \cdot |DN|^{2} - NEBN) \cdot \sin(\theta_{loc})\f]
 *
 * Where \f$ k_{s} \f$ is the calibration factor, \f$NEBN\f$ is the
 * Noise Equivalent Beta Naugth and \f$ \theta_{loc} \f$ is the local
 * incident angle.
 *
 * NEBN is interpolated for each range position according to the
 * polynomial coefficients from the most recent noise record.
 *
 * \f$ \theta_{loc} \f$ is estimated from the incidence angles from
 * the metadata following a least-square regression.
 * One can also add its own incident angle records using the
 * appropriate add method.
 *
 * For  now, this filter does not support more accurate behaviour like
 * incident angle interpolation from metadata or the Geocoded
 * Incidence Angle mask optional 1B product (GIM).
 *
 * All parameters are imported from the input image metadata, even if
 * one can set different values using the proper accessors.
 *
 * The UseFastCalibration flag alllows to derive a coarser yet faster
 * calibration by neglecting the noise term.
 *
 * Results can be obtained either in linear or logarithmic scale
 * (decibels), using the ResultsInDecibels flag.
 *
 * When signal value is too small with respect to the noise term, it
 * may happen that \f$\sigma_{0} \f$ becomes negative. In that case
 * the value is replaced by a default value, which can be tuned by the
 * user.
 *
 * This filter works with either real or complex image. In the case of
 * the complex images (SLC products for instance), modulus and phase
 * are extracted, sigma naught is computed from the modulus and the
 * phase is set back to the result.
 *
 * For implementation details, consider reading the code
 * TerraSarCalibrationFunctor.
 *
 * \sa TerraSarCalibrationFunctor
 * \sa TerraSarBrightnessImageFilter
 *
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup Radiometry
 */

template<class TInputImage, class TOutputImage>
class ITK_EXPORT TerraSarCalibrationImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** "typedef" for standard classes. */
  typedef TerraSarCalibrationImageFilter                     Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Extract input and output images dimensions.*/
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition
   * and the declaration. */
  typedef TInputImage                                                      InputImageType;
  typedef TOutputImage                                                     OutputImageType;
  typedef typename InputImageType::RegionType                              InputImageRegionType;
  typedef typename OutputImageType::RegionType                             OutputImageRegionType;
  typedef typename InputImageType::InternalPixelType                       InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType                      OutputInternalPixelType;
  typedef typename  itk::NumericTraits<InputInternalPixelType>::ValueType  InputValueType;
  typedef typename  itk::NumericTraits<OutputInternalPixelType>::ValueType OutputValueType;
  typedef typename InputImageType::IndexType                               IndexType;

  /** Incidence angle samples */
  typedef std::pair<IndexType, double>          IncidenceAngleRecordType;
  typedef std::vector<IncidenceAngleRecordType> IncidenceAngleRecordVectorType;

  /** Calibration functor typedef */
  typedef typename Functor::TerraSarCalibrationFunctor<InputValueType,
      OutputValueType>    CalibrationFunctorType;

  /** typedef to access metadata */
  typedef itk::MetaDataDictionary           MetaDataDictionaryType;
  typedef typename InputImageType::SizeType SizeType;

  /** Noise records typedefs */
  typedef ossimplugins::ImageNoise ImageNoiseType;
  /**
   * This is used to store the noise record along with its
   * acquisition time
   */
  typedef std::pair<double, ImageNoiseType> NoiseRecordType;
  typedef std::vector<NoiseRecordType>      NoiseRecordVectorType;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(TerraSarCalibrationImageFilter, ImageToImageFilter);

  /** Accessors */
  /** Calibration Factor */
  itkSetMacro(CalibrationFactor, double);
  itkGetMacro(CalibrationFactor, double);

  /** If fast calibration is On, noise is ignored */
  itkSetMacro(UseFastCalibration, bool);
  itkGetMacro(UseFastCalibration, bool);
  itkBooleanMacro(UseFastCalibration);

  /** Activate if you wish results in decibels */
  itkSetMacro(ResultsInDecibels, bool);
  itkGetMacro(ResultsInDecibels, bool);
  itkBooleanMacro(ResultsInDecibels);

  /** Pulse Repetition Frequency */
  itkSetMacro(PRF, double);
  itkGetMacro(PRF, double);

  /** Set the default value (replacing negative sigma */
  itkSetMacro(DefaultValue, double);
  itkGetMacro(DefaultValue, double);

  /** The Calibration algorithm uses the original product size to
   * compute the range and azimuth location. Hence, if the input image is
   * an extract of the original product, this information must be set.
   * If not set, the LargestPossibleRegion of the input image is
   * used. Please note that this parameter has no influence on the output
   * image regions. */
  itkSetMacro(OriginalProductSize, SizeType);
  itkGetConstReferenceMacro(OriginalProductSize, SizeType);

  /**
   * Add a new noise record for calibration.
   * \param utcAcquisitionTime Noise record time
   * \param record The noise record
   */
  void AddNoiseRecord(double utcAcquisitionTime, const ImageNoiseType& record);

  /**
   * Clear all noise records
   */
  void ClearNoiseRecords();

  /**
   * Add an incidence angle record to estimate the local incidence.
   * Angle has to be given in degrees.
   */
  void AddIncidenceAngleRecord(const IndexType& index, double angle);

  /**
   * Clear the incidence angle records.
   */
  void ClearIncidenceAngleRecords();

protected:
  /** Constructor */
  TerraSarCalibrationImageFilter();
  /** Destructor */
  virtual ~TerraSarCalibrationImageFilter();

  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();

  /** Threaded generate Data */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Reimplement the Modified() method to handle the
   *  ParametersUpToDate flag */
  virtual void Modified();

private:
  TerraSarCalibrationImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Method used to order noise records by increasing acquisition
   * date */
  static bool CompareNoiseRecords(const NoiseRecordType& record1, const NoiseRecordType& record2);

  /** Estimate angular plane. The angular is interpolated by a plane
   * fitted by least squares to the angular corners. More Appropriate
   * methods (using DEM for instance) could be implemented later */
  void EstimateAngularPlaneParameters();

  /**
   * Compute the incidence angle from the index
   */
  inline double ComputeIncidenceAngle(const IndexType& index) const;

  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Pulse Repetition Frequency */
  double m_PRF;

  /** Original product size */
  SizeType m_OriginalProductSize;

  /** Use fast calibration ? */
  bool m_UseFastCalibration;

  /** Results in decibels ? */
  bool m_ResultsInDecibels;

  /** Noise record vector */
  NoiseRecordVectorType m_NoiseRecords;

  /** Default value (for negative sigma) */
  double m_DefaultValue;

  /** Incidence angle records vector */
  IncidenceAngleRecordVectorType m_IncidenceAngleRecords;

  /** Incidence angle linear regression parameters */
  double m_IncidenceAngleAx;
  double m_IncidenceAngleAy;
  double m_IncidenceAngleOffset;

  /** True if parameters are up-to-date (avoid recomputing) */
  bool m_ParametersUpToDate;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationImageFilter.txx"
#endif

#endif
