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

#ifndef otbVectorImageToMatrixFilter_h
#define otbVectorImageToMatrixFilter_h

#include "otbPersistentImageFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"
#include "itkSimpleDataObjectDecorator.h"
#include "itkImageRegionSplitter.h"
#include "itkVariableSizeMatrix.h"
#include "itkVariableLengthVector.h"

namespace otb
{

/** \class PersistentVectorImageToMatrixFilter
 * \brief Compute covariance & correlation of a large image using streaming
 *
 *  This filter persists its temporary data. It means that if you Update it n times on n different
 * requested regions, the output statistics will be the statitics of the whole set of n regions.
 *
 * To reset the temporary data, one should call the Reset() function.
 *
 * To get the statistics once the regions have been processed via the pipeline, use the Synthetize() method.
 *
 * \sa PersistentImageFilter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 *
 * \ingroup OTBStatistics
 */
template<class TInputImage>
class ITK_EXPORT PersistentVectorImageToMatrixFilter :
  public PersistentImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard Self typedef */
  typedef PersistentVectorImageToMatrixFilter           Self;
  typedef PersistentImageFilter<TInputImage, TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                         Pointer;
  typedef itk::SmartPointer<const Self>                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PersistentVectorImageToMatrixFilter, PersistentImageFilter);

  /** Image related typedefs. */
  typedef TInputImage                             ImageType;
  typedef typename TInputImage::Pointer           InputImagePointer;
  typedef typename TInputImage::RegionType        RegionType;
  typedef typename TInputImage::SizeType          SizeType;
  typedef typename TInputImage::IndexType         IndexType;
  typedef typename TInputImage::PixelType         PixelType;
  typedef typename TInputImage::InternalPixelType InternalPixelType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Type to use for computations. */
  //typedef typename itk::NumericTraits<InternalPixelType>::RealType RealType;
  typedef InternalPixelType RealType;
  typedef itk::VariableLengthVector<RealType>                      RealPixelType;

  /** Smart Pointer type to a DataObject. */
  typedef typename itk::DataObject::Pointer DataObjectPointer;
  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Type of DataObjects used for scalar outputs */
  typedef vnl_matrix<RealType>                          MatrixType;
  typedef itk::SimpleDataObjectDecorator<MatrixType>    MatrixObjectType;

  /** Return the computed matrix as a DataObject */
  MatrixObjectType* GetMatrixOutput();
  const MatrixObjectType* GetMatrixOutput() const;

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
  void Synthetize(void) override;
  void Reset(void) override;

protected:
  PersistentVectorImageToMatrixFilter();
  ~PersistentVectorImageToMatrixFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  void  ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

private:
  PersistentVectorImageToMatrixFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
}; // end of class PersistentVectorImageToMatrixFilter

/**===========================================================================*/

/** \class VectorImageToMatrixImageFilter
 * \brief This class streams the whole input image through the PersistentStatisticsImageFilter.
 *
 * This way, it allows computing the first order global statistics of this image. It calls the
 * Reset() method of the PersistentStatisticsImageFilter before streaming the image and the
 * Synthetize() method of the PersistentStatisticsImageFilter after having streamed the image
 * to compute the statistics. The accessor on the results are wrapping the accessors of the
 * internal PersistentStatisticsImageFilter.
 *
 * \sa PersistentVectorImageToMatrixFilter
 * \sa PersistentImageFilter
 * \sa PersistentFilterStreamingDecorator
 * \sa StreamingImageVirtualWriter
 * \ingroup Streamed
 * \ingroup Multithreaded
 * \ingroup MathematicalStatisticsImageFilters
 *
 * \ingroup OTBStatistics
 */

template<class TInputImage>
class ITK_EXPORT VectorImageToMatrixImageFilter :
  public PersistentFilterStreamingDecorator<PersistentVectorImageToMatrixFilter<TInputImage> >
{
public:
  /** Standard Self typedef */
  typedef VectorImageToMatrixImageFilter Self;
  typedef PersistentFilterStreamingDecorator
  <PersistentVectorImageToMatrixFilter<TInputImage> > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToMatrixImageFilter, PersistentFilterStreamingDecorator);

  typedef TInputImage                                 InputImageType;
  typedef typename Superclass::FilterType             PersistentFilterType;

  typedef typename PersistentFilterType::MatrixType             MatrixType;
  typedef typename PersistentFilterType::MatrixObjectType       MatrixObjectType;

  using Superclass::SetInput;
  void SetInput(InputImageType * input)
  {
    this->GetFilter()->SetInput(input);
  }
  const InputImageType * GetInput()
  {
    return this->GetFilter()->GetInput();
  }

  /** Return the computed Mean. */
  const MatrixType& GetMatrix() const
  {
    return this->GetMatrixOutput()->Get();
  }
  MatrixObjectType* GetMatrixOutput()
  {
    return this->GetFilter()->GetMatrixOutput();
  }
  const MatrixObjectType* GetMatrixOutput() const
  {
    return this->GetFilter()->GetMatrixOutput();
  }

protected:
  /** Constructor */
  VectorImageToMatrixImageFilter() {};
  /** Destructor */
  ~VectorImageToMatrixImageFilter() override {}

private:
  VectorImageToMatrixImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageToMatrixImageFilter.hxx"
#endif

#endif
