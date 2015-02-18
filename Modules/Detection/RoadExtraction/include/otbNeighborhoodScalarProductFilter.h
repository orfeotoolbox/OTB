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
#ifndef __otbNeighborhoodScalarProductFilter_h
#define __otbNeighborhoodScalarProductFilter_h

#include "otbImageToModulusAndDirectionImageFilter.h"

namespace otb
{
/** \class NeighborhoodScalarProductFilter
 *  \brief This filter is designed to be part of a pipeline aiming at linear feature extraction, such as roads.
 *
 *  This filter takes as input a two-band image resulting from a gradient operator. The first channel contains
 *  the gradient value in the first image direction, the second contains the gradient value in the second image direction.
 *
 *  Assuming that the linear feature we want to detect is darker than its surrounding environnement, we can deduce that
 *  the gradient direction will be opposite on each side of the road. Therefore, we compute for each pixel to compute the scalar
 *  product of the gradient vector for each opposite pixels in the neighborhood of radius 1 of the pixel to compute.
 *
 * The lower negativ scalar product value along these four pairs of pixel gives us the direction in which there is most
 * likely a linear feature.
 *
 * This filters has two outputs :
 * - The first output (from the GetOutput() method) gives the modulus of the lower negativ scalar product value for this pixel.
 * - The second output (from the GetOutputDirection() method) gives the direction in radian of this linear feature.
 *
 * Please note that there are only 8 possible values for the direction image, corresponding to two directions for each pair of opposite
 * pixels. In our conventions, negatives angle values represent opposite gradient vectors, whereas positive angle values represent convergent
 * gradient vectors.
 *  \ingroup Streamed
 *  \ingroup Threaded
 */
template <class TInputImage, class TOutputModulus, class TOutputDirection>
class ITK_EXPORT NeighborhoodScalarProductFilter
  : public ImageToModulusAndDirectionImageFilter<TInputImage, TOutputModulus, TOutputDirection>
{
public:
  /** Standard typedefs */
  typedef NeighborhoodScalarProductFilter                                                      Self;
  typedef ImageToModulusAndDirectionImageFilter<TInputImage, TOutputModulus, TOutputDirection> Superclass;
  typedef itk::SmartPointer<Self>                                                              Pointer;
  typedef itk::SmartPointer<const Self>                                                        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(NeighborhoodScalarProductFilter, ImageToModulusAndDirectionImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::ConstPointer    InputImageConstPointerType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef TOutputModulus                           OutputModulusType;
  typedef typename OutputModulusType::Pointer      OutputModulusPointerType;
  typedef typename OutputModulusType::RegionType   RegionType;
  typedef typename OutputModulusType::SizeType     SizeType;
  typedef typename OutputModulusType::IndexType    IndexType;
  typedef TOutputDirection                         OutputDirectionType;
  typedef typename OutputDirectionType::Pointer    OutputDirectionPointerType;
  typedef typename OutputDirectionType::RegionType OutputImageRegionType;

protected:
  /** Constructor */
  NeighborhoodScalarProductFilter();
  /** Destructor */
  virtual ~NeighborhoodScalarProductFilter() {}
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
                            itk::ThreadIdType threadId);
private:
  NeighborhoodScalarProductFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNeighborhoodScalarProductFilter.txx"
#endif

#endif
