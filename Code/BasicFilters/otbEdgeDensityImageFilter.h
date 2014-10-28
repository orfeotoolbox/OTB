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
#ifndef __otbEdgeDensityImageFilter_h
#define __otbEdgeDensityImageFilter_h

#include "otbBinaryImageToDensityImageFilter.h"
#include "itkProcessObject.h"


namespace otb
{

/** \class EdgeDensityImageFilter
 *  \brief This composite filter computes the density of the edges around a pixel.
 *
 *
 */

template <class TInputImage, class TOutputImage, class TEdgeDetector, class TDensityCount>
class ITK_EXPORT EdgeDensityImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef EdgeDensityImageFilter                             Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EdgeDensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                        InputImageType;
  typedef typename InputImageType::Pointer   InputImagePointerType;
  typedef typename InputImageType::PixelType InputImagePixelType;
  typedef typename InputImageType::SizeType  InputImageSizeType;

  /** OutputImageType typedef support*/
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;

  /** Edge Detector typedef Support*/
  typedef TEdgeDetector                  DetectorType;
  typedef typename DetectorType::Pointer DetectorPointerType;

  /** Count Density Function typedef support*/
  typedef TDensityCount DensityCountFunctionType;

  /** PointSetToDensityImageFilter support*/
  typedef otb::BinaryImageToDensityImageFilter<InputImageType,
      OutputImageType,
      DensityCountFunctionType>   DensityImageType;

  typedef typename DensityImageType::Pointer DensityImagePointerType;

  /** Get/Set the radius of the neighborhood over which the
  statistics are evaluated */
  itkSetMacro(NeighborhoodRadius, InputImageSizeType);
  itkGetConstReferenceMacro(NeighborhoodRadius, InputImageSizeType);
  void SetNeighborhoodRadius(unsigned int rad)
  {
    m_NeighborhoodRadius.Fill(rad);
    this->Modified();
  }

  /**Set/Get detector */
  itkSetObjectMacro(Detector, DetectorType);
  itkGetObjectMacro(Detector, DetectorType);
  itkGetObjectMacro(DensityImageFilter, DensityImageType);

protected:

  /**
   * Constructor.
   */
  EdgeDensityImageFilter();
  /**
   * Destructor.
   */
  virtual ~EdgeDensityImageFilter();
  /**
   * Standard PrintSelf method.
   */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * Main computation method.
   */
  virtual void GenerateData();

private:

  EdgeDensityImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DetectorPointerType     m_Detector;
  DensityImagePointerType m_DensityImageFilter;
  InputImageSizeType      m_NeighborhoodRadius;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEdgeDensityImageFilter.txx"
#endif

#endif
