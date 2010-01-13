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

#include "itkImageToImageFilter.h"
#include "otbTerraSarCalibrationFunctor.h"
#include "itkMetaDataDictionary.h"

namespace otb
{


/** \class TerraSarCalibrationImageFilter
 *  \brief Calculates the calibration of TerraSar sensor images.
 *
 *  For details, please refer to Infoterra documentation : "Radiometric Calibration of TerraSAR-X Data".
 *  Using FastCalibration boolean allows to compute the result considering the first noise polygone
 *  coeffcients for each line or to use every polygones coefficient given.
 *  The code was made considering that the first acquisition point is the lower left one.
 *
 * \ingroup Radiometry
 */

template<class TInputImage, class TOutputImage >
class ITK_EXPORT TerraSarCalibrationImageFilter :
    public itk::ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** "typedef" for standard classes. */
  typedef TerraSarCalibrationImageFilter                                                      Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>                                   Superclass;
  typedef itk::SmartPointer<Self>                                                             Pointer;
  typedef itk::SmartPointer<const Self>                                                       ConstPointer;

  /** Extract input and output images dimensions.*/
  itkStaticConstMacro( InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro( OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** "typedef" to simplify the variables definition and the declaration. */
  typedef TInputImage                                                                         InputImageType;
  typedef TOutputImage                                                                        OutputImageType;
  typedef typename InputImageType::RegionType                                                 InputImageRegionType;
  typedef typename OutputImageType::RegionType                                                OutputImageRegionType;
  typedef typename InputImageType::InternalPixelType                                          InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType                                         OutputInternalPixelType;
  typedef typename  itk::NumericTraits<InputInternalPixelType>::ValueType                     InputValueType;
  typedef typename  itk::NumericTraits<OutputInternalPixelType>::ValueType                    OutputValueType;

  /** Calibration functor typedef */
  typedef typename Functor::TerraSarCalibrationFunctor< InputValueType, OutputValueType>      CalibrationFunctorType;  

  /** typedef to access metadata */
  typedef itk::MetaDataDictionary                                                             MetaDataDictionaryType;
  typedef typename InputImageType::SizeType                                                   SizeType;

  /** Noise records typedefs */
  typedef ossimplugins::ImageNoise                                                            ImageNoiseType;
  /** 
   * This is used to store the noise record along with its
   * acquisition time 
   */
  typedef std::pair<double,ImageNoiseType>                                                    NoiseRecordType;
  typedef std::vector<NoiseRecordType>                                                        NoiseRecordVectorType;

  /** object factory method. */
  itkNewMacro(Self);

  /** return class name. */
  itkTypeMacro(TerraSarCalibrationImageFilter,ImageToImageFilter);

  /** Accessors */
  /** Calibration Factor */
  itkSetMacro(CalibrationFactor,double);
  itkGetMacro(CalibrationFactor,double);

  /** Sensor local incident angle in degree */
  itkSetMacro(LocalIncidentAngle,double);
  itkGetMacro(LocalIncidentAngle,double);
  
  /** If fast calibration is On, noise is ignored */
  itkSetMacro(UseFastCalibration,bool);
  itkGetMacro(UseFastCalibration,bool);
  itkBooleanMacro(UseFastCalibration);

  /** Activate if you wish results in decibels */
  itkSetMacro(ResultsInDecibels,bool);
  itkGetMacro(ResultsInDecibels,bool);
  itkBooleanMacro(ResultsInDecibels);

  /** Pulse Repetition Frequency */
  itkSetMacro(PRF,double);
  itkGetMacro(PRF,double);

  /** The Calibration algorithm uses the original product size to
   * compute the range and azimuth location. Hence, if the input image is
   * an extract of the original product, this information must be set.
   * If not set, the LargestPossibleRegion of the input image is
   * used. Please note that this parameter has no influence on the output
   * image regions. */
  itkSetMacro(OriginalProductSize,SizeType);
  itkGetConstReferenceMacro(OriginalProductSize,SizeType);
  
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

protected:
  /** Constructor */
  TerraSarCalibrationImageFilter();
  /** Destructor */
  virtual ~TerraSarCalibrationImageFilter();
  
  /** Initialize the functor vector */
  void BeforeThreadedGenerateData();

  /** Threaded generate Data */
  virtual void ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,int threadId);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  TerraSarCalibrationImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Method used to order noise records by increasing acquisition
   * date */
  static bool CompareNoiseRecords(const NoiseRecordType& record1, const NoiseRecordType& record2);

  /** Calibration Factor */
  double m_CalibrationFactor;

  /** Sensor local incident angle in degree */
  double m_LocalIncidentAngle;

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
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarCalibrationImageFilter.txx"
#endif

#endif
