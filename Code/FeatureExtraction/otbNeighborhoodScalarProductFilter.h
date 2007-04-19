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
#ifndef _otbNeighborhoodScalarProductFilter_h
#define _otbNeighborhoodScalarProductFilter_h

#include "otbImageToModulusAndDirectionImageFilter.h"

namespace otb
{
/** \class NeighborhoodScalarProductFilter
 *  \brief 
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 */
template <class TInputImage, class TOutputModulus, class TOutputDirection>
class ITK_EXPORT NeighborhoodScalarProductFilter
  : public ImageToModulusAndDirectionImageFilter<TInputImage,TOutputModulus,TOutputDirection>
{
 public:
  /** Standard typedefs */
  typedef NeighborhoodScalarProductFilter                                                    Self;
  typedef ImageToModulusAndDirectionImageFilter<TInputImage,TOutputModulus,TOutputDirection> Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(NeighborhoodScalarProductFilter,ImageToModulusAndDirectionImageFilter);
  
  /** Template parameters typedefs */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::PixelType InputPixelType;
  typedef TOutputModulus OutputModulusType;
  typedef typename OutputModulusType::Pointer OutputModulusPointerType;
  typedef typename OutputModulusType::RegionType RegionType;
  typedef typename OutputModulusType::SizeType SizeType;
  typedef typename OutputModulusType::IndexType IndexType;
  typedef TOutputDirection OutputDirectionType;
  typedef typename OutputDirectionType::Pointer OutputDirectionPointerType;
  typedef typename OutputDirectionType::RegionType OutputImageRegionType;
 
protected:
  /** Constructor */
  NeighborhoodScalarProductFilter();
  /** Destructor */
  virtual ~NeighborhoodScalarProductFilter() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** NeighborhoodScalarProductImageFilter can be implemented as a multithreaded filter.
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
                            int threadId );
private:
  NeighborhoodScalarProductFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNeighborhoodScalarProductFilter.txx"
#endif

#endif
