/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbOverlapSaveConvolutionImageFilter_h
#define otbOverlapSaveConvolutionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace otb
{
/** \class OverlapSaveConvolutionImageFilter
 *
 * This filter implements the convolution operation between a kernel and an
 * image using the overlap save algorithm (http://wikipedia.org/wiki/Overlap-save_method).
 *
 * This method takes advantages of the FFTW implementation of Fast Fourrier Transform to
 * exchange an intensive convolution product in the space domain for a simple term by term
 * product in the Fourrier domain. This result in tremendous speed gain when using large kernel
 * with exactly the same result as the classical convolution filter.
 *
 * \note This filter could be threaded but requires additional design due to limited thread-safety
 * of the FFTW library.
 *
 * \note For the moment only constant zero boundary conditions are used in this filter. This could produce
 *  very different results from the classical convolution filter with zero flux neumann boundary condition,
 * especially with large kernels.
 *
 * \note ITK must be set to use FFTW (double implementation) for this filter to work properly. If not, exception
 *  will be raised at filter creation.
 *  Install fftw and set the cmake variable ITK_USE_FFTWD to ON.
 *
 * \sa ConvolutionImageFilter
 *
 * \ingroup ShouldBeThreaded
 * \ingroup Streamed
 * \ingroup IntensityImageFilters
 *
 * \ingroup OTBConvolution
 */
template <class TInputImage, class TOutputImage,
    class TBoundaryCondition = itk::ZeroFluxNeumannBoundaryCondition<TInputImage> >
class ITK_EXPORT OverlapSaveConvolutionImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef OverlapSaveConvolutionImageFilter                        Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OverlapSaveConvolutionImageFilter, ImageToImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename InputImageType::SizeType                     InputSizeType;
  typedef typename itk::Array<InputRealType>                    ArrayType;
  typedef TBoundaryCondition                                    BoundaryConditionType;

  /** Set the radius of the neighborhood used to compute the mean. */
  virtual void SetRadius(const InputSizeType rad)
  {
    itkDebugMacro("setting radius to " << rad);
    if (this->m_Radius != rad)
      {
      this->m_Radius = rad;
      unsigned int arraySize = 1;
      for (unsigned int i = 0; i < m_Radius.GetSizeDimension(); ++i)
        {
        arraySize *= 2 * this->m_Radius[i] + 1;
        }
      this->m_Filter.SetSize(arraySize);
      this->m_Filter.Fill(1);
      this->Modified();
      }
  }

  /** Get the radius of the neighborhood used to compute the mean */
  itkGetConstReferenceMacro(Radius, InputSizeType);

  /** Set the input filter */
  void SetFilter(ArrayType filter)
  {
    if ((filter.Size() != m_Filter.Size()))
      {
      itkExceptionMacro(
        "Error in SetFilter, invalid filter size:" << filter.Size() <<
        " instead of 2*(m_Radius[0]+1)*(2*m_Radius[1]+1): " << m_Filter.Size());
      }
    else
      {
      m_Filter = filter;
      }
    this->Modified();
  }
  /** Get the filter */
  itkGetConstReferenceMacro(Filter, ArrayType);

  /** Set/Get methods for the normalization of the filter */
  itkSetMacro(NormalizeFilter, bool);
  itkGetMacro(NormalizeFilter, bool);
  itkBooleanMacro(NormalizeFilter);

  /** Since this filter implements a neighborhood operation, it requests a largest input
   * region than the output region.
   */
  void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError) override;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck, (itk::Concept::HasNumericTraits<InputPixelType>));
  /** End concept checking */
#endif

protected:
  /** Constructor */
  OverlapSaveConvolutionImageFilter();
  /** destructor */
  ~OverlapSaveConvolutionImageFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /* TODO For the moment this class provide only a GenerateData(),
   * due to limited thread-safety of FFTW plan creation.
   */
  void GenerateData() override;
  // void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

private:
  OverlapSaveConvolutionImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Radius of the filter */
  InputSizeType m_Radius;

  /** Filter array */
  ArrayType m_Filter;

  /** Flag for filter normalization */
  bool m_NormalizeFilter;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbOverlapSaveConvolutionImageFilter.hxx"
#endif

#endif
