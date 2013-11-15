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
#ifndef __otbImageToPointSetFilter_h
#define __otbImageToPointSetFilter_h

#include "otbPointSetSource.h"
#include "itkImageRegionSplitter.h"
#include "otbStreamingTraits.h"

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
  void SetInput(unsigned int idx, const InputImageType *input);
  void SetInput(const InputImageType *input);

  /** Get the input image of this process object.  */
  const InputImageType * GetInput(unsigned int idx);
  const InputImageType * GetInput();

  /** Prepare the output */
  void GenerateOutputInformation(void);

protected:
  ImageToPointSetFilter();
  virtual ~ImageToPointSetFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData(void);

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
  typedef StreamingTraits<InputImageType>                                       StreamingTraitsType;
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(InputImageDimension)> SplitterType;
  typedef typename SplitterType::Pointer                                        RegionSplitterPointer;
  RegionSplitterPointer m_RegionSplitter;

private:
  ImageToPointSetFilter(const ImageToPointSetFilter &); //purposely not implemented
  void operator =(const ImageToPointSetFilter&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToPointSetFilter.txx"
#endif

#endif
