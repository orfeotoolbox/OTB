/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbFunctionWithNeighborhoodToImageFilter_h
#define otbFunctionWithNeighborhoodToImageFilter_h

#include "otbFunctionToImageFilter.h"

namespace otb
{

/** \class FunctionWithNeighborhoodToImageFilter
  * \brief Evaluates a ImageFunction onto a source image
 *
 * The class walks an input image and evaluates
 * the function at every pixel location. The output of the spatial function
 * and the pixel type of the output image must be compatible.
 *
 * Like its parent ImageToImageFilter, this class functions in the filtering
 * pipeline and produces a unique output image.
 *
 * The function has to inherit from itkImageFunction
 *
 * \sa SpatialFunctionImageEvaluatorFilter
 * \sa SpatialFunctionImageFilter
 * \sa ImageFunction
 * \ingroup ImageFilters
 *
 * \ingroup OTBImageManipulation
 */

template <class TInputImage, class TOutputImage, class TFunction>
class ITK_EXPORT FunctionWithNeighborhoodToImageFilter :
  public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef FunctionWithNeighborhoodToImageFilter              Self;
  typedef itk::InPlaceImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FunctionWithNeighborhoodToImageFilter, itk::InPlaceImageFilter);

  /** Some typedefs. */
  /** Image size typedef. */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     InputImageSizeType;
  typedef typename InputImageType::OffsetType   InputImageOffsetType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointer;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputImagePixelType;
  /** Type of function. */
  typedef TFunction                         FunctionType;
  typedef typename FunctionType::Pointer    FunctionPointerType;
  typedef typename FunctionType::OutputType FunctionValueType;
  typedef typename FunctionType::InputType  FunctionPositionType;

//  itkGetObjectMacro(Function, FunctionType);

  /** Set the internal spatial function. */
  void SetFunction(FunctionPointerType function)
  {
    m_Function = function;
    this->Modified();
  }
  FunctionPointerType GetFunction()
  {
    return m_Function;
  }


  /** Image dimensions */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);


  /** Accessors */
  itkGetMacro(Radius, InputImageSizeType);
  void SetRadius(InputImageSizeType& rad)
  {
    m_Radius = rad;
    m_Function->SetRadius(rad);
    this->Modified();
  }
  itkGetMacro(Offset, InputImageOffsetType);
  void SetOffset(InputImageOffsetType& offset)
  {
    m_Offset = offset;
    m_Function->SetOffset(offset);
    this->Modified();
  }

protected:
  FunctionWithNeighborhoodToImageFilter();
  ~FunctionWithNeighborhoodToImageFilter() override{}

  void BeforeThreadedGenerateData() override;

  void GenerateInputRequestedRegion() override;

  /** SpatialFunctionImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId) override;

private:
  FunctionWithNeighborhoodToImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  //FunctionPointerType  m_Function;
  FunctionPointerType              m_Function;
  std::vector<FunctionPointerType> m_FunctionList;
  InputImageSizeType               m_Radius;
  InputImageOffsetType             m_Offset;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFunctionWithNeighborhoodToImageFilter.hxx"
#endif

#endif
