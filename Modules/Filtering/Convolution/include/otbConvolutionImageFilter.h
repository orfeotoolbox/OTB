/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbConvolutionImageFilter_h
#define otbConvolutionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkArray.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"

namespace otb
{
/** \class ConvolutionImageFilter
 * \brief Applies a convolution filter to a mono channel image
 *
 * Computes an image which is the convolution of the input image
 * with a filter.
 *
 * The radius of the input filter is provided by the \code SetInput() \endcode
 * method and the filters coefficients are given by an itk::Array passed to the
 * \code SetFilter() \endcode method.
 *
 * By default, the input filter is not normalized but it can be using the
 * NormalizeFilterOn() method.
 *
 * This filter allows the user to choose the boundary condtions in the template parameters.
 Default boundary conditions are zero flux Neumann boundary conditions.
 *
 * An optimized version of this filter using FFTW is available in the Orfeo ToolBox and
 * will significantly improves performances especially for large kernels
 * (see OverlapSaveConvolutionImageFilter).
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 * \sa ImageBoundaryCondition
 * \sa ZeroFluxNeumannBoundaryCondition
 * \sa OverlapSaveConvolutionImageFilter
 *
 * \ingroup IntensityImageFilters
 * \ingroup Streamed
 * \ingroup MultiThreaded
 *
 * \ingroup OTBConvolution
 */
template <class TInputImage, class TOutputImage, class TBoundaryCondition = itk::ZeroFluxNeumannBoundaryCondition<TInputImage>,
          class TFilterPrecision = typename itk::NumericTraits<typename TInputImage::InternalPixelType>::RealType>
class ITK_EXPORT ConvolutionImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Standard class typedefs. */
  typedef ConvolutionImageFilter Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConvolutionImageFilter, ImageToImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename InputImageType::SizeType                     InputSizeType;
  typedef TFilterPrecision                                      FilterPrecisionType;
  typedef typename itk::Array<FilterPrecisionType>              ArrayType;
  typedef TBoundaryCondition                                    BoundaryConditionType;

  /** Set the radius of the neighborhood of the filter */
  virtual void SetRadius(const InputSizeType rad)
  {
    itkDebugMacro("setting radius to " << rad);
    if (this->m_Radius != rad)
    {
      this->m_Radius         = rad;
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

  /** Get the radius of the neighborhood of the filter*/
  itkGetConstReferenceMacro(Radius, InputSizeType);

  /** Set the input filter */
  virtual void SetFilter(ArrayType filter)
  {
    if (filter.Size() != m_Filter.Size())
    {
      itkExceptionMacro("Error in SetFilter, invalid filter size:" << filter.Size() << " instead of (2*m_Radius[0]+1)*(2*m_Radius[1]+1): " << m_Filter.Size());
    }
    else
    {
      m_Filter = filter;
    }
    this->Modified();
  }
  itkGetConstReferenceMacro(Filter, ArrayType);

  /**
   * Set/Get methods for the normalization of the filter
  */
  itkSetMacro(NormalizeFilter, bool);
  itkGetMacro(NormalizeFilter, bool);
  itkBooleanMacro(NormalizeFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck, (itk::Concept::HasNumericTraits<InputPixelType>));
/** End concept checking */
#endif

protected:
  ConvolutionImageFilter();
  ~ConvolutionImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** ConvolutionImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  /** ConvolutionImageFilter needs a larger input requested region than
   * the output requested region.  As such, ConvolutionImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  void GenerateInputRequestedRegion() override;

private:
  ConvolutionImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Radius of the filter */
  InputSizeType m_Radius;
  /** Array containing the filter values */
  ArrayType m_Filter;
  /** Flag for filter coefficients normalization */
  bool m_NormalizeFilter;
};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbConvolutionImageFilter.hxx"
#endif

#endif
