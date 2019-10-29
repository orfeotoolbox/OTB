/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStreamingMatrixTransposeMatrixImageFilter_h
#define otbStreamingMatrixTransposeMatrixImageFilter_h

#include "otbPersistentImageFilter.h"
#include "itkSimpleDataObjectDecorator.h"
#include "otbStreamingTraits.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"
#include "otbPersistentFilterStreamingDecorator.h"

namespace otb
{

/** \class PersistentMatrixTransposeMatrixImageFilter
 * \brief Compute \f[X^T.Y \f]. Allow a padding of ones.
 *
 * \f[X\f] and \f[Y\f] are the input images.
 * The padding has the effect of adding a component filled with ones to the image
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the result for the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa StreamingTraits
 * \sa StatisticsImageFilter
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBImageManipulation
 */
template <class TInputImage, class TInputImage2>
class ITK_EXPORT PersistentMatrixTransposeMatrixImageFilter : public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentMatrixTransposeMatrixImageFilter Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentMatrixTransposeMatrixImageFilter, PersistentImageFilter);

  /** Image related typedefs. */
  // First Input
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  typedef typename TInputImage2::IndexType         IndexType2;
  typedef typename TInputImage2::PixelType         PixelType2;
  typedef typename TInputImage2::InternalPixelType InternalPixelType2;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  itkSetMacro(UsePadFirstInput, bool);
  itkGetMacro(UsePadFirstInput, bool);
  itkSetMacro(UsePadSecondInput, bool);
  itkGetMacro(UsePadSecondInput, bool);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Type to use for computations. */
  // First Input
  typedef double                              RealType;
  typedef itk::VariableLengthVector<RealType> RealPixelType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer                  DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Type of DataObjects used for scalar outputs */
  typedef typename itk::Array<long>                     ArrayLongPixelType;
  typedef typename itk::VariableSizeMatrix<RealType>    MatrixType;
  typedef typename std::vector<MatrixType>              ArrayMatrixType;
  typedef typename std::vector<RealPixelType>           ArrayRealPixelType;
  typedef typename std::vector<PixelType>               ArrayPixelType;
  typedef itk::SimpleDataObjectDecorator<RealPixelType> RealPixelObjectType;
  typedef itk::SimpleDataObjectDecorator<PixelType>     PixelObjectType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;

  /** Return the computed transpose(Image1)*Image2. */
  MatrixType GetResult() const
  {
    return this->GetResultOutput()->Get();
  }
  MatrixObjectType*       GetResultOutput();
  const MatrixObjectType* GetResultOutput() const;

  /** Make a DataObject of the correct type to be used as the specified
   * output.
   */
  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType idx) override;
  using Superclass::MakeOutput;

  /** Pass the input through unmodified. Do this by Grafting in the
   *  AllocateOutputs method.
   */
  void AllocateOutputs() override;
  void GenerateOutputInformation() override;
  // Override since the filter needs all the data for the algorithm
  void GenerateInputRequestedRegion() override;
  void Reset(void) override;
  void Synthetize(void) override;

  /** Input wrapper */
  void SetFirstInput(const TInputImage* input1)
  {
    this->SetInput(0, input1);
  }
  void SetSecondInput(const TInputImage2* input2)
  {
    this->SetInput(1, input2);
  }

  const TInputImage* GetFirstInput()
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return nullptr;
    }
    else
      return (static_cast<const TInputImage*>(this->itk::ProcessObject::GetInput(0)));
  }

  const TInputImage2* GetSecondInput()
  {
    if (this->GetNumberOfInputs() < 2)
    {
      return nullptr;
    }
    else
      return (static_cast<const TInputImage2*>(this->itk::ProcessObject::GetInput(1)));
  }

protected:
  PersistentMatrixTransposeMatrixImageFilter();
  ~PersistentMatrixTransposeMatrixImageFilter() override
  {
  }
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /** Multi-thread version GenerateData. */
  void ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentMatrixTransposeMatrixImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;

  ArrayMatrixType m_ThreadSum;
  bool            m_UsePadFirstInput;
  bool            m_UsePadSecondInput;

  /** Nulber Of Component per Pixel. Change for padding */
  unsigned int m_NumberOfComponents1;
  unsigned int m_NumberOfComponents2;
}; // end of class

/**===========================================================================*/

/** \class StreamingMatrixTransposeMatrixImageFilter
 * \brief This class streams the whole input image through the PersistentMatrixTransposeMatrixImageFilter.
 *
 * This way, it allows computing \f[X^T.Y \f] where \f[X\f] and \f[Y\f] are the input images.
 * first order global statistics of this image. It calls the Reset() method of the
 * PersistentMatrixTransposeMatrixImageFilter before streaming the image and the
 * Synthetize() method of the PersistentMatrixTransposeMatrixImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter. The accessor on the pad options are also provided.
 *
 * \sa PersistentMatrixTransposeMatrixImageFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBImageManipulation
 */

template <class TInputImage1, class TInputImage2>
class ITK_EXPORT StreamingMatrixTransposeMatrixImageFilter
    : public PersistentFilterStreamingDecorator<PersistentMatrixTransposeMatrixImageFilter<TInputImage1, TInputImage2>>
{
public:
  /** Standard Self typedef */
  typedef StreamingMatrixTransposeMatrixImageFilter Self;
  typedef PersistentFilterStreamingDecorator<PersistentMatrixTransposeMatrixImageFilter<TInputImage1, TInputImage2>> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(StreamingMatrixTransposeMatrixImageFilter, PersistentFilterStreamingDecorator);

  typedef typename Superclass::FilterType                        MatrixToTransposeMatrixFilterType;
  typedef typename MatrixToTransposeMatrixFilterType::MatrixType MatrixType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>             MatrixObjectType;

  typedef TInputImage1 InputImage1Type;
  typedef TInputImage2 InputImage2Type;

  /** Interfaces to the embedded filter */
  void SetFirstInput(InputImage1Type* input1)
  {
    this->GetFilter()->SetFirstInput(input1);
  }
  void SetSecondInput(InputImage2Type* input2)
  {
    this->GetFilter()->SetSecondInput(input2);
  }
  void SetUsePadFirstInput(bool pad)
  {
    this->GetFilter()->SetUsePadFirstInput(pad);
  }
  void SetUsePadSecondInput(bool pad)
  {
    this->GetFilter()->SetUsePadSecondInput(pad);
  }
  bool GetUsePadFirstInput(void)
  {
    return this->GetFilter()->GetUsePadFirstInput();
  }
  bool GetUsePadSecondInput(void)
  {
    return this->GetFilter()->GetUsePadSecondInput();
  }
  /** Return the computed transpose(Image1)*Image2. */
  MatrixType GetResult(void) const
  {
    return this->GetResultOutput()->Get();
  }
  MatrixObjectType* GetResultOutput(void)
  {
    return this->GetFilter()->GetResultOutput();
  }
  const MatrixObjectType* GetResultOutput() const
  {
    return this->GetFilter()->GetResultOutput();
  }

protected:
  /** Constructor */
  StreamingMatrixTransposeMatrixImageFilter(){};
  /** Destructor */
  ~StreamingMatrixTransposeMatrixImageFilter() override
  {
  }

private:
  StreamingMatrixTransposeMatrixImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingMatrixTransposeMatrixImageFilter.hxx"
#endif

#endif
