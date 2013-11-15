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
#ifndef __otbScalarImageToAdvancedTexturesFilter_h
#define __otbScalarImageToAdvancedTexturesFilter_h

#include "itkImageToImageFilter.h"
#include "itkScalarImageToCooccurrenceMatrixFilter.h"
#include "otbGreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator.h"

namespace otb
{
/**
 * \class ScalarImageToAdvancedTexturesFilter
 * \brief TODO
 */
template<class TInpuImage, class TOutputImage>
class ScalarImageToAdvancedTexturesFilter : public itk::ImageToImageFilter
  <TInpuImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ScalarImageToAdvancedTexturesFilter               Self;
  typedef itk::ImageToImageFilter<TInpuImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through the object factory */
  itkNewMacro(Self);

  /** RTTI */
  itkTypeMacro(ScalarImageToAdvancedTexturesFilter, ImageToImageFilter);

  /** Template class typedefs */
  typedef TInpuImage                           InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename InputImageType::RegionType  InputRegionType;
  typedef typename InputRegionType::SizeType   SizeType;
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputRegionType;

  /** Co-occurence matrix and textures calculator */
  typedef itk::Statistics::ScalarImageToCooccurrenceMatrixFilter<InputImageType> CoocurrenceMatrixGeneratorType;
  typedef typename CoocurrenceMatrixGeneratorType::Pointer       CoocurrenceMatrixGeneratorPointerType;
  typedef typename CoocurrenceMatrixGeneratorType::OffsetType    OffsetType;
  typedef typename CoocurrenceMatrixGeneratorType::HistogramType HistogramType;
  typedef GreyLevelCooccurrenceMatrixAdvancedTextureCoefficientsCalculator
  <HistogramType>                                                TextureCoefficientsCalculatorType;
  typedef typename TextureCoefficientsCalculatorType::Pointer TextureCoefficientsCalculatorPointerType;

  /** Set the radius of the window on which textures will be computed */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of the window on which textures will be computed */
  itkGetMacro(Radius, SizeType);

  /** Set the offset for co-occurence computation */
  itkSetMacro(Offset, OffsetType);

  /** Get the offset for co-occurence computation */
  itkGetMacro(Offset, OffsetType);

  /** Set the number of bin per axis for histogram generation */
  itkSetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Get the number of bin per axis for histogram generation */
  itkGetMacro(NumberOfBinsPerAxis, unsigned int);

  /** Set the input image minimum */
  itkSetMacro(InputImageMinimum, InputPixelType);

  /** Get the input image minimum */
  itkGetMacro(InputImageMinimum, InputPixelType);

  /** Set the input image maximum */
  itkSetMacro(InputImageMaximum, InputPixelType);

  /** Get the input image maximum */
  itkGetMacro(InputImageMaximum, InputPixelType);

  /** Get the mean output image */
  OutputImageType * GetMeanOutput();

  /** Get the variance output image */
  OutputImageType * GetVarianceOutput();

  /** Get the sum average output image */
  OutputImageType * GetSumAverageOutput();

  /** Get the sum of variances output image */
  OutputImageType * GetSumVarianceOutput();

  /** Get the sum of entropies output image */
  OutputImageType * GetSumEntropyOutput();

  /** Get the difference of entropies output image */
  OutputImageType * GetDifferenceEntropyOutput();

  /** Get the difference of variance output image */
  OutputImageType * GetDifferenceVarianceOutput();

  /** Get the IC1 image */
  OutputImageType * GetIC1Output();

  /** Get the IC2 output image */
  OutputImageType * GetIC2Output();

protected:
  /** Constructor */
  ScalarImageToAdvancedTexturesFilter();
  /** Destructor */
  ~ScalarImageToAdvancedTexturesFilter();
  /** Generate the input requested region */
  virtual void GenerateInputRequestedRegion();
  /** Parallel textures extraction */
  virtual void ThreadedGenerateData(const OutputRegionType& outputRegion, itk::ThreadIdType threadId);

private:
  ScalarImageToAdvancedTexturesFilter(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Convenient method to compute union of 2 regions */
  static OutputRegionType RegionUnion(const OutputRegionType& region1, const OutputRegionType& region2);

  /** Radius of the window on which to compute textures */
  SizeType m_Radius;

  /** Offset for co-occurence */
  OffsetType m_Offset;

  /** Number of bins per axis for histogram generation */
  unsigned int m_NumberOfBinsPerAxis;

  /** Input image minimum */
  InputPixelType m_InputImageMinimum;

  /** Input image maximum */
  InputPixelType m_InputImageMaximum;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbScalarImageToAdvancedTexturesFilter.txx"
#endif

#endif
