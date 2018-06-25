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

#ifndef otbImageToSURFKeyPointSetFilter_h
#define otbImageToSURFKeyPointSetFilter_h

#include "itkConstNeighborhoodIterator.h"
#include "otbImageToHessianDeterminantImageFilter.h"
#include "itkPointSet.h"
#include <itkRescaleIntensityImageFilter.h>
#include <otbImageList.h>
#include "itkResampleImageFilter.h"
#include "otbImageToPointSetFilter.h"
#include "itkVector.h"

#include <iostream>
namespace otb
{

/** \class ImageToSURFKeyPointSetFilter
 *  \brief This class extracts key points from an image through a pyramidal gaussian based decomposition
 *
 * This class implements the SURF Key point detector proposed by Tuytelaars and Vangool from the university
 * of Leuven, 2005
 *
 * \li Gaussian Second order derivative Hessian images are computed in each
 *     level and each octave for the input image.
 * \li For each octave, an extremum search is launched on each 3 adjacent scales.
 * \li The Key points detected are the ones extremum in the current , previous and
 *     next scale of reserach. 3 scales are needed for the computation (NumberOfScales >=3).
 * \li Orientation and descriptors are computed for each key point detected.
 *
 * Selected Key Points are stored in an itk::PointSet structure.
 * Points contains the coordinate of the detected point.
 * DataPoints contain the values of the 64 element descriptor for each key point
 * detected through the pyramidal analysis.
 *
 * Orientation is expressed in degree in the range of [0, 360]
 *
 *  \sa otb::ImageToDeterminantHessianImage
 *
 * \ingroup OTBDescriptors
 */

template <class TInputImage, class TOutputPointSet>
class ITK_EXPORT ImageToSURFKeyPointSetFilter
  : public ImageToPointSetFilter<TInputImage, TOutputPointSet>
{

public:

  /** Standard class typedefs. */
  typedef ImageToSURFKeyPointSetFilter                        Self;
  typedef ImageToPointSetFilter<TInputImage, TOutputPointSet> Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToSURFKeyPointSetFilter, ImageToPointSetFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                          InputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::IndexType   PixelIndex;
  typedef typename InputImageType::IndexType   IndexType;
  typedef typename InputImageType::PixelType   PixelValue;
  typedef typename InputImageType::SpacingType SpacingType;
  typedef typename InputImageType::SizeType    SizeType;
  typedef typename InputImageType::PointType   PointImageType;

  typedef TOutputPointSet                           OutputPointSetType;
  typedef typename TOutputPointSet::Pointer         OutputPointSetPointerType;
  typedef typename TOutputPointSet::PixelType       OutputPixelType;
  typedef typename TOutputPointSet::PointType       OutputPointType;
  typedef typename TOutputPointSet::PointIdentifier OutputPointIdentifierType;

  /** Set/Get the number of Octaves */
  itkSetMacro(OctavesNumber, int);
  itkGetMacro(OctavesNumber, int);

  /** Set/Get the number of scales*/
  itkSetMacro(ScalesNumber, int);
  itkGetMacro(ScalesNumber, int);

  /** Get the number of KeyPoints detected*/
  itkGetMacro(NumberOfPoints, int);

  /** Internal filters typedefs */
  typedef itk::ConstNeighborhoodIterator<InputImageType>      NeighborhoodIteratorType;
  typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;
  typedef typename NeighborhoodType::OffsetType               OffsetType;

  typedef otb::ImageToHessianDeterminantImageFilter<InputImageType, InputImageType> ImageToDetHessianImageType;
  typedef typename ImageToDetHessianImageType::Pointer                              DetHessianPointerFilter;

  /** Filter for  resampling images for the multi-scale analysis */
  typedef itk::ResampleImageFilter<InputImageType, InputImageType> ResampleFilterType;
  typedef typename ResampleFilterType::Pointer                     ResampleFilterPointerType;

  /** ImageList  to store the Hessian determinant image at each scale (sigma width)*/
  typedef otb::ImageList<InputImageType>  ImageListType;
  typedef typename ImageListType::Pointer ImageListTypePointer;

  /** 3D vector to store the keypoints translations */
  typedef std::vector<double> VectorType;
  typedef itk::Vector<PixelValue, 3> VectorPointType;

protected:

  /**
   * Constructor.
   */
  ImageToSURFKeyPointSetFilter();
  /**
   * Destructor.
   */
  ~ImageToSURFKeyPointSetFilter() override;
  /**
   * Standard PrintSelf method.
   */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;
  /**
   * Main computation method.
   */
  void  GenerateData() override;

  /** Check local extremum for 8 neighbors (current)
   *
   *  \param neigh Current scale neighborhood
   *
   *  \return true if the Central pixel is extremum
   */
  virtual bool IsLocalExtremum(const NeighborhoodType& neigh);

  /** Check local extremum for 8 neighbors (Previous or Scale)
   *
   *  \param neigh Neighborhood at previous or next scale
   *  \param CenterValue value at current scale center
   *
   *  \return true if the Central pixel is extremum
   */
  virtual bool IsLocalExtremumAround(const NeighborhoodType& neigh,
                                     double CenterValue);

  /** Refine location key point
   *
   *  \li Discard keypoints with low contrats DoH < DoHThreshold
   *  \li Discard keypoints that lie outside the centre cubic pixel
   *
   *  \param currentScale iterator
   *  \param previousScale iterator
   *  \param nextScale iterator
   *  \param solution refined keypoint location
   *
   *  \return true if key point is accepted, false otherwise
   */
  bool RefineLocationKeyPoint(const NeighborhoodIteratorType& currentScale,
                              const NeighborhoodIteratorType& previousScale,
                              const NeighborhoodIteratorType& nextScale,
                              VectorPointType& solution);

  /** AssignOrientation
   *
   * \param neigh current scale neighborhood
   * \param S scale affected to the keypoint
   *
   * \return  key point orientation
   */
  virtual double AssignOrientation(const NeighborhoodType& neigh,
                                   double S);

  /** ComputeDescriptor
   *
   * \param neigh current scale neighboorhood
   * \param O orientation assigned to the key point
   * \param S scale
   *
   * \return hsitogram descriptor
   */
  virtual VectorType ComputeDescriptor(const NeighborhoodType& neigh,
                                       double O,
                                       double S);
  /**
   * Compute min a b c
   */
  virtual int GetMin(int a, int b, int c);

private:

  ImageToSURFKeyPointSetFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** Number of octaves */
  int m_OctavesNumber;

  /** Number of scale for each octave */
  int m_ScalesNumber;

  /** Number of key points detected */
  int m_NumberOfPoints;

  /** Minimum threshold for determinant of Hessian **/
  double m_DoHThreshold;

  /** Those images */
  InputImagePointerType m_determinantImage;
  InputImagePointerType m_ImageCurrent;
  InputImagePointerType m_ImageMovedPrev;
  InputImagePointerType m_ImageMovedNext;

  /** ImageToDeterminantHessianFilter filter */
  DetHessianPointerFilter m_DetHessianFilter;

  /*Resample Filter*/
  ResampleFilterPointerType m_ResampleFilter;

  /** ImageList*/
  ImageListTypePointer m_ImageList;

  /** Number of shifted keypoints*/
  unsigned int m_DifferentSamplePoints;

  /** Offsets vector, conveniance datas */
  OffsetType m_Offsets[8];
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToSURFKeyPointSetFilter.hxx"
#endif

#endif
