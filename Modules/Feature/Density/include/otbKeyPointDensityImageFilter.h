/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbKeyPointDensityImageFilter_h
#define __otbKeyPointDensityImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbPointSetToDensityImageFilter.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class KeyPointDensityImageFilter
 *  \brief This class computes the density of the characteristic
 *  points detected in the image.
 *
 * The density computed is a ratio of the number of keypoints over
 * the total number of pixels within a region.
 * It uses the class PointSetToDensityImageFilter to do so.
 *
 * This class use the key point detector specified in the 3rd argument
 * template to detect the keypoints. Then for every pixel, the density of the key points
 * within its neighborhood is computed.
 *
 * The output is a map of key points density.
 */

template <class TInputImage, class TOutputImage, class TDetector>
class ITK_EXPORT KeyPointDensityImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef KeyPointDensityImageFilter                         Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(KeyPointDensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                      InputImageType;
  typedef typename InputImageType::Pointer InputImagePointerType;

  /** OutputImageType typedef support*/
  typedef TOutputImage                      OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointerType;

  /** Detector typedef Support*/
  typedef TDetector                                 DetectorType;
  typedef typename DetectorType::OutputPointSetType PointSetType;
  typedef typename DetectorType::Pointer            DetectorPointerType;

  /** PointSetToDensityImageFilter support*/
  typedef otb::PointSetToDensityImageFilter<PointSetType, OutputImageType> PointSetToDensityImageType;
  typedef typename PointSetToDensityImageType::Pointer                     PointSetToDensityImagePointerType;

  /** Get/Set the radius of the neighborhood over which the
  statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, unsigned int);
  itkGetConstReferenceMacro(NeighborhoodRadius, unsigned int);

  /**Set/Get Descriptor from the otbCountmageFunction*/
  virtual void SetDetector(DetectorType* detector);
  virtual DetectorType* GetDetector();

protected:

  /**
   * Constructor.
   */
  KeyPointDensityImageFilter();
  /**
   * Destructor.
   */
  virtual ~KeyPointDensityImageFilter();
  /**
   * Standard PrintSelf method.
   */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * Main computation method.
   */
  //virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId );
  virtual void GenerateData();

private:

  KeyPointDensityImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DetectorPointerType               m_Detector;
  PointSetToDensityImagePointerType m_PointSetToDensityImageFilter;
  unsigned int                      m_NeighborhoodRadius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbKeyPointDensityImageFilter.txx"
#endif

#endif
