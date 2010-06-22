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
#ifndef __otb_ScalarImageToTexturesFilter_h
#define __otb_ScalarImageToTexturesFilter_h

#include "itkImageToImageFilter.h"

#include "otbMaskedScalarImageToGreyLevelCoocurenceMatrixGenerator.h"
#include "itkGreyLevelCooccurrenceMatrixTextureCoefficientsCalculator.h"

namespace otb
{
template<class TInpuImage, class TOutputImage>
class ScalarImageToTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToTexturesFilter                            Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage>      Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToTexturesFilter,ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                                             InputImageType;
  typedef typename InputImageType::Pointer                       InputImagePointerType;
  typedef typename InputImageType::PixelType                     InputPixelType;
  typedef typename InputImageType::RegionType                    InputRegionType;
  typedef typename InputRegionType::SizeType                     SizeType;
  typedef TOutputImage                                           OutputImageType;
  typedef typename OutputImageType::Pointer                      OutputImagePointerType;
  typedef typename OutputImageType::RegionType                   OutputRegionType;

  /** Co-occurence matrix and textures calculator */
  typedef otb::MaskedScalarImageToGreyLevelCooccurrenceMatrixGenerator
    <InputImageType>                                             CoocurrenceMatrixGeneratorType;
  typedef typename CoocurrenceMatrixGeneratorType::Pointer       CoocurrenceMatrixGeneratorPointerType;
  typedef typename CoocurrenceMatrixGeneratorType::OffsetType    OffsetType;
  typedef typename CoocurrenceMatrixGeneratorType::HistogramType HistogramType;
  typedef itk::Statistics::GreyLevelCooccurrenceMatrixTextureCoefficientsCalculator
  <HistogramType>                                                TextureCoefficientsCalculatorType;
  typedef typename TextureCoefficientsCalculatorType::Pointer    TextureCoefficientsCalculatorPointerType;

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius,SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius,SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset,OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset,OffsetType);

  /** Set the number of bin per axis for histogram generation */
  itkSetMacro(NumberOfBinsPerAxis,unsigned int);

  /** Get the number of bin per axis for histogram generation */
  itkGetMacro(NumberOfBinsPerAxis,unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum,InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum,InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum,InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum,InputPixelType);

  /** Get the energy output image */
  OutputImageType * GetEnergyOutput();

  /** Get the entropy output image */
  OutputImageType * GetEntropyOutput();

  /** Get the correlation output image */
  OutputImageType * GetCorrelationOutput();

  /** Get the inverse difference moment output image */
  OutputImageType * GetInverseDifferenceMomentOutput();

  /** Get the inertia output image */
  OutputImageType * GetInertiaOutput();

  /** Get the cluster shade output image */
  OutputImageType * GetClusterShadeOutput();

  /** Get the cluster prominence image */
  OutputImageType * GetClusterProminenceOutput();

  /** Get the Haralick correlation output image */
  OutputImageType * GetHaralickCorrelationOutput();

protected:
  /** Constructor */
  ScalarImageToTexturesFilter();
  /** Destructor */
  ~ScalarImageToTexturesFilter();
  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();
  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType & outputRegion, int threadId);

private:
  ScalarImageToTexturesFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType & region1, const OutputRegionType & region2);

  /** Radius of the window on which to compute textures */
  SizeType                                                       m_Radius;

  /** Offset for co-occurence */
  OffsetType                                                     m_Offset;

  /** Number of bins per axis for histogram generation */
  unsigned int                                                   m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType                                                 m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType                                                 m_InputImageMaximum;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToTexturesFilter.txx"
#endif

#endif
