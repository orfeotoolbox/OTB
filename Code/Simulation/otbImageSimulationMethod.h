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

#ifndef __otbImageSimulationMethod_h
#define __otbImageSimulationMethod_h


#include "itkImageSource.h"
#include "otbProspectModel.h"
#include "otbSailModel.h"
#include "otbLabelMapToSimulatedImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"

//#include "otbStreamingResampleImageFilter.h"
#include "itkResampleImageFilter.h"
#include "otbProlateInterpolateImageFunction.h"
#include "itkLabelMapToLabelImageFilter.h"

namespace otb
{

/**
 * \class ImageSimulationMethod
 * \brief Image simulation method
 *
 * ImageSimulationMethod converts a vectorData describing spatial distribution of materials into an image of reflectance.
 * Reflectance image is computed by applying the RSR (relatives spectral response) of the satellite to the spectra obtained
 * by the spectrum simulator. Then the satellite FTM is applied to compute spatial distribution of spectra.
 * FTM is computed  using otbResampleImageFilter and a ProlateInterpolateImageFunction.
 * This method is not a true composite filter and the method UpdateData() must be called in a main program using this class.
 *
 *In order to avoid homogenous regions of a material, for each pixel in the region a noise is added to the reference sprectrum obtained from the spectrum simulator (see LabelMapToSimulatedImageFilter).
 *
 * \sa ImageSource
 */

template <class TInputVectorData, class TSpatialisation, class TSimulationStep1, class TSimulationStep2, class TFTM, class TOutputImage>
class ITK_EXPORT ImageSimulationMethod :
  public itk::ImageSource <TOutputImage>

{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef ImageSimulationMethod Self;
  typedef itk::ImageSource<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputVectorData InputVectorDataType;
  typedef typename InputVectorDataType::Pointer InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer InputVectorDataConstPointer;

  typedef TSpatialisation SpatialisationType;
  typedef typename SpatialisationType::Pointer SpatialisationPointer;
  typedef typename SpatialisationType::OutputLabelMapType LabelMapType;
  typedef typename LabelMapType::LabelObjectType::LabelType LabelType;
  typedef TSimulationStep1 SimulationStep1Type;
  typedef typename SimulationStep1Type::Pointer SimulationStep1Pointer;
  typedef TSimulationStep2 SimulationStep2Type;
  typedef typename SimulationStep2Type::Pointer SimulationStep2Pointer;
  typedef TFTM FTMType;
  typedef typename FTMType::Pointer FTMPointer;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::ConstPointer OutputImageConstPointer;
  typedef typename OutputImageType::Pointer OutputImagePointer;
  typedef typename OutputImageType::IndexType IndexType;
  typedef typename OutputImageType::InternalPixelType InternalPixelType;
  typedef otb::Image<LabelType, 2> OutputLabelImageType;

  typedef otb::LabelMapToSimulatedImageFilter<LabelMapType, SimulationStep1Type, SimulationStep2Type, OutputImageType>
      LabelMapToSimulatedImageFilterType;
  typedef typename LabelMapToSimulatedImageFilterType::Pointer LabelMapToSimulatedImageFilterPointer;

  typedef otb::MultiToMonoChannelExtractROI<double, double> MultiToMonoChannelFilterType;
  typedef typename MultiToMonoChannelFilterType::Pointer MultiToMonoChannelFilterPointer;
  typedef MultiToMonoChannelFilterType::OutputImageType SingleImageType;
  typedef otb::ImageList<SingleImageType> ImageListType;
  typedef typename ImageListType::Pointer ImageListPointer;
  typedef otb::ImageListToVectorImageFilter<ImageListType, OutputImageType> ImageListToVectorImageFilterType;
  typedef typename ImageListToVectorImageFilterType::Pointer ImageListToVectorImageFilterPointer;

  typedef otb::ProlateInterpolateImageFunction<SingleImageType> InterpolatorType;
  typedef typename InterpolatorType::Pointer InterpolatorPointer;
  typedef itk::ResampleImageFilter<SingleImageType, SingleImageType, double> FTMFilterType;
  //    typedef otb::StreamingResampleImageFilter<SingleImageType, SingleImageType, double>   FTMFilterType;
  typedef typename FTMFilterType::Pointer FTMFilterPointer;

  typedef itk::LabelMapToLabelImageFilter<LabelMapType, OutputLabelImageType> LabelMapToLabelImageFilterType;
  typedef typename LabelMapToLabelImageFilterType::Pointer LabelMapToLabelImageFilterPointer;

  /** Standard Macro*/
  itkTypeMacro(ImageSimulationMethod, ImageSource)
; itkNewMacro(Self)
;

  itkSetMacro(NumberOfComponentsPerPixel, unsigned int)
; itkGetMacro(NumberOfComponentsPerPixel, unsigned int)
;

  //prolate radius
  itkSetMacro(Radius, unsigned int)
; itkGetMacro(Radius, unsigned int)
;

  itkSetMacro(SatRSRFilename, std::string)
; itkGetMacro(SatRSRFilename, std::string)
;

  itkSetMacro(PathRoot, std::string)
; itkGetMacro(PathRoot, std::string)
;

  itkSetMacro(Mean, double)
; itkGetMacro(Mean, double)
;

  itkSetMacro(Variance, double)
; itkGetMacro(Variance, double)
;

  itkSetObjectMacro(Spatialisation, SpatialisationType)
;
  //    itkSetObjectMacro(LabelMapToSimulatedImage, LabelMapToSimulatedImageType);
  //    itkSetObjectMacro(FTM, FTMType);


  /** Set/Get Input */
  itkSetObjectMacro(InputVectorData, InputVectorDataType)
; itkGetConstObjectMacro(InputVectorData, InputVectorDataType)
;
  //    void SetInput(const InputVectorDataType *object);
  //    InputVectorDataType * GetInput();

  /** Get outputs */
  virtual OutputLabelImageType * GetOutputLabelImage();
  virtual OutputImageType * GetOutputReflectanceImage();

  void UpdateData();

protected:
  ImageSimulationMethod();
  virtual ~ImageSimulationMethod()
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  //    virtual void GenerateData();
  virtual void GenerateOutputInformation();

private:
  ImageSimulationMethod(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int m_NumberOfComponentsPerPixel;
  unsigned int m_Radius; //radius of prolate interpolate
  std::string m_SatRSRFilename;
  std::string m_PathRoot;
  double m_Mean;
  double m_Variance;

  InputVectorDataPointer m_InputVectorData;
  SpatialisationPointer m_Spatialisation;
  LabelMapToSimulatedImageFilterPointer m_LabelMapToSimulatedImageFilter;
  //    FTMPointer                       m_FTM;
  std::vector<MultiToMonoChannelFilterPointer> m_MultiToMonoChannelFilterList;
  ImageListPointer m_ImageList;
  ImageListToVectorImageFilterPointer m_ImageListToVectorImageFilter;

  std::vector<FTMFilterPointer> m_FTMFilterList;
  std::vector<InterpolatorPointer> m_InterpolatorList;

  LabelMapToLabelImageFilterPointer m_LabelMapToLabelImageFilter;

};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSimulationMethod.txx"
#endif

#endif
