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
#ifndef __otbEdgeDetectorImageFilter_h
#define __otbEdgeDetectorImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

namespace otb
{

/** \class EdgeDetectorImageFilter
 *  \brief This composite filter binaries a edge detection image output.
 *  The used edge detection filter is given as template of the class.
 *  The class only supports Image.
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage, class TEdgeDetection>
class ITK_EXPORT EdgeDetectorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{

public:

  /** Standard class typedefs. */
  typedef EdgeDetectorImageFilter                            Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EdgeDetectorImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs*/
  typedef TInputImage                          InputImageType;
  typedef TOutputImage                         OutputImageType;
  typedef typename InputImageType::Pointer     InputImagePointerType;
  typedef typename InputImageType::PixelType   InputImagePixelType;
  typedef typename InputImageType::SizeType    InputImageSizeType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::PixelType  OutputImagePixelType;

  /** Edge Detection */
  typedef TEdgeDetection                  DetectionType;
  typedef typename DetectionType::Pointer DetectionPointerType;

  /** Thresholder filter : Otsu */
  typedef itk::BinaryThresholdImageFilter<InputImageType, InputImageType> BinaryFilterType;
  typedef typename BinaryFilterType::Pointer                              BinaryFilterPointerType;

  /**Set/Get detector */
  itkSetObjectMacro(Detector, DetectionType);
  itkGetObjectMacro(Detector, DetectionType);
  itkGetConstObjectMacro(Detector, DetectionType);
  /**Set/Get binary filter */
  itkSetObjectMacro(BinaryFilter, BinaryFilterType);
  itkGetObjectMacro(BinaryFilter, BinaryFilterType);
  //itkGetObjectMacro(DetectorImageFilter, DetectorImageType);

  /** Set lower threshold value. */
  void SetLowerThreshold(InputImagePixelType val)
  {
    m_BinaryFilter->SetLowerThreshold(val);
    this->Modified();
  }
  /** Set upper threshold value. */
  void SetUpperThreshold(InputImagePixelType val)
  {
    m_BinaryFilter->SetUpperThreshold(val);
    this->Modified();
  }
  /** Set Inside value. */
  void SetInsideValue(InputImagePixelType val)
  {
    m_BinaryFilter->SetInsideValue(val);
    this->Modified();
  }
  /** Set Outside value. */
  void SetOutsideValue(InputImagePixelType val)
  {
    m_BinaryFilter->SetOutsideValue(val);
    this->Modified();
  }
protected:

  EdgeDetectorImageFilter();
  virtual ~EdgeDetectorImageFilter();
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual void GenerateData();

private:

  EdgeDetectorImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  DetectionPointerType    m_Detector;
  BinaryFilterPointerType m_BinaryFilter;
};
}
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbEdgeDetectorImageFilter.txx"
#endif

#endif
