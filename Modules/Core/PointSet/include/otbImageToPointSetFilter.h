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

#ifndef otbImageToPointSetFilter_h
#define otbImageToPointSetFilter_h

#include "otbPointSetSource.h"
#include "otbRAMDrivenAdaptativeStreamingManager.h"

// TODO: This class needs a refactoring to become a persistent filter

namespace otb
{

/** \class ImageToPointSetFilter
 * \brief Base class to output PointSet data with image data as input
 *
 * ImageToPointSetFilter is the base class for all process objects that output
 * Point Set data and require image data as input. Specifically, this class
 * defines the SetInput() method for defining the input to a filter.
 *
 * \ingroup ImageFilters
 *
 * \ingroup OTBPointSet
 */
template <class TInputImage, class TOutputPointSet>
class ITK_EXPORT ImageToPointSetFilter : public PointSetSource<TOutputPointSet>
{
public:
  /** Standard class typedefs. */
  typedef ImageToPointSetFilter           Self;
  typedef PointSetSource<TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToPointSetFilter, PointSetSource);

  /** Some Image related typedefs. */
  typedef   TInputImage                           InputImageType;
  typedef   typename InputImageType::Pointer      InputImagePointer;
  typedef   typename InputImageType::ConstPointer InputImageConstPointer;
  typedef   typename InputImageType::RegionType   InputImageRegionType;
  typedef   typename InputImageType::PixelType    InputImagePixelType;
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Some PointSet related typedefs. */
  typedef   typename Superclass::OutputPointSetType     OutputPointSetType;
  typedef   typename Superclass::OutputPointSetPointer  OutputPointSetPointer;
  typedef   typename Superclass::PointsContainerType    PointsContainerType;
  typedef   typename Superclass::PointDataContainerType PointDataContainerType;
  typedef   itk::ProcessObject                          ProcessObjectType;

  /** Set the input image of this process object.  */
  using Superclass::SetInput;
  void SetInput(unsigned int idx, const InputImageType *input);
  void SetInput(const InputImageType *input);

  /** Get the input image of this process object.  */
  const InputImageType * GetInput(unsigned int idx);
  const InputImageType * GetInput();

  /** Prepare the output */
  void GenerateOutputInformation(void) override;

protected:
  ImageToPointSetFilter();
  ~ImageToPointSetFilter() override {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  void GenerateData(void) override;

  /** Multi-threading implementation */

  typedef std::vector<typename OutputPointSetType::PointsContainer::Pointer>    OutputPointsContainerForThreadType;
  typedef std::vector<typename OutputPointSetType::PointDataContainer::Pointer> OutputPointDataContainerForThreadType;

  virtual void BeforeThreadedGenerateData();

  virtual void AfterThreadedGenerateData();

  virtual int SplitRequestedRegion(int i, int num, InputImageRegionType& splitRegion);

  virtual void ThreadedGenerateData(const InputImageRegionType& inputRegionForThread, itk::ThreadIdType threadId);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
  static ITK_THREAD_RETURN_TYPE ThreaderCallback(void *arg);

  /** Internal structure used for passing image data into the threading library */
  struct ThreadStruct
  {
    Pointer Filter;
  };

  OutputPointsContainerForThreadType    m_PointsContainerPerThread;
  OutputPointDataContainerForThreadType m_PointDataContainerPerThread;

  /** End Multi-threading implementation */

  /** Setup for streaming */
  typedef RAMDrivenAdaptativeStreamingManager<InputImageType> StreamingManagerType;
  typedef typename StreamingManagerType::Pointer StreamingManagerPointer;

  StreamingManagerPointer m_StreamingManager;

private:
  ImageToPointSetFilter(const ImageToPointSetFilter &) = delete;
  void operator =(const ImageToPointSetFilter&) = delete;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPointSetFilter.hxx"
#endif

#endif
