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

#ifndef otbWaveletInverseImageFilter_h
#define otbWaveletInverseImageFilter_h

#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbWaveletsSynopsisImageToWaveletsBandsListFilter.h"

namespace otb {

/** \class WaveletInverseImageFilter
 * \brief
 *
 * \ingroup OTBWavelet
 * \sa WaveletImageFilter
 */
template < class TInputImage,
           class TOutputImage,
           Wavelet::Wavelet TMotherWaveletOperator >
class WaveletInverseImageFilter :
  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard class typedefs. */
  typedef TInputImage                                                                InputImageType;
  typedef TOutputImage                                                               OutputImageType;

  itkStaticConstMacro( ImageDimension,            unsigned int, InputImageType::ImageDimension );
  itkStaticConstMacro( MotherWaveletOperator,     short,        TMotherWaveletOperator );
  itkStaticConstMacro( DirectionOfTransformation, short ,       otb::Wavelet::FORWARD );

  typedef WaveletInverseImageFilter<InputImageType,OutputImageType, TMotherWaveletOperator>  Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType>                    Superclass;
  typedef itk::SmartPointer<Self>                                                     Pointer;
  typedef itk::SmartPointer<const Self>                                               ConstPointer;

  typedef typename InputImageType::PixelType                                          InputPixelType;
  typedef typename OutputImageType::PixelType                                         OutputPixelType;

  typedef otb::WaveletOperator< TMotherWaveletOperator, otb::Wavelet::INVERSE, OutputPixelType, ImageDimension >
    WaveletOperatorType;
  typedef otb::WaveletFilterBank< OutputImageType, OutputImageType, WaveletOperatorType, otb::Wavelet::INVERSE >
    FilterBankType;
  typedef otb::WaveletTransform< OutputImageType, OutputImageType, FilterBankType, otb::Wavelet::INVERSE >
    WaveletInverseTransformFilterType;
  typedef typename WaveletInverseTransformFilterType::Pointer
    WaveletInverseTransformFilterPointerType;

  typedef otb::WaveletsSynopsisImageToWaveletsBandsListFilter<InputImageType, typename WaveletInverseTransformFilterType::InputImageListType>
    WaveletsSynopsisImageToWaveletsBandsListFilterType;
  typedef typename WaveletsSynopsisImageToWaveletsBandsListFilterType::Pointer
    WaveletsSynopsisImageToWaveletsBandsListFilterPointerType;


  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(WaveletInverseImageFilter, ImageToImageFilter);

  itkGetMacro(NumberOfDecompositions,unsigned int);
  itkSetMacro(NumberOfDecompositions,unsigned int);

  /** If the filter is modified, the internal filters need to be modified too */
  virtual void Modified() const;

protected:
  WaveletInverseImageFilter();
  virtual ~WaveletInverseImageFilter();

  virtual void GenerateInputRequestedRegion();

  virtual void GenerateData();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  WaveletInverseImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  WaveletsSynopsisImageToWaveletsBandsListFilterPointerType m_SynopsisImageToWaveletsBandsList;
  WaveletInverseTransformFilterPointerType m_WaveletTransform;
  unsigned int m_NumberOfDecompositions;
};

}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletInverseImageFilter.txx"
#endif

#endif
