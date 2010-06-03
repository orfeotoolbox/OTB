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
#ifndef __otbDEMToImageGenerator_h
#define __otbDEMToImageGenerator_h

#include <iostream>
#include <stdio.h>

#include "itkIndent.h"
#include "itkImageSource.h"
#include "otbImage.h"
#include "otbDEMHandler.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace otb
{
/** \class DEMToImageGenerator
 *
 * \brief Class to generate an image from DEM data
 *
 * This class is based on ossimElevManager. It takes in input the upper left longitude
 * and latitude, the spacing and the output image size.
 * Handle DTED and SRTM formats.
 *
 * \ingroup Images
 *
 * \example IO/DEMToImageGenerator.cxx
 *
 */
template <class TDEMImage>
class ITK_EXPORT DEMToImageGenerator :
  public itk::ImageSource<TDEMImage>
{
public:
  /** Standard class typedefs. */
  typedef TDEMImage                        DEMImageType;
  typedef typename DEMImageType::Pointer   DEMImagePointerType;
  typedef typename DEMImageType::PixelType PixelType;

  typedef DEMToImageGenerator            Self;
  typedef itk::ImageSource<DEMImageType> Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef DEMImageType                   OutputImageType;

  typedef typename Superclass::Pointer                    OutputImagePointer;
  typedef typename OutputImageType::SpacingType           SpacingType;
  typedef typename OutputImageType::SizeType              SizeType;
  typedef typename OutputImageType::PointType             PointType;
  typedef typename OutputImageType::IndexType             IndexType;
  typedef typename Superclass::OutputImageRegionType      OutputImageRegionType;
  typedef itk::ImageRegionIteratorWithIndex<DEMImageType> ImageIteratorType;

  typedef otb::DEMHandler DEMHandlerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMToImageGenerator, ImageSource);

  /** Set/Get the Output Origin coordinates. */
  itkSetMacro(OutputOrigin, PointType);
  itkGetConstReferenceMacro(OutputOrigin, PointType);

  /** Set/Get the Output Size. */
  itkSetMacro(OutputSize, SizeType);
  itkGetConstReferenceMacro(OutputSize, SizeType);

  /** Set/Get the Output Spacing. */
  itkSetMacro(OutputSpacing, SpacingType);
  itkGetConstReferenceMacro(OutputSpacing, SpacingType);

  /** Set/Get the Default Unknown Value. */
  itkSetMacro(DefaultUnknownValue, PixelType);
  itkGetConstReferenceMacro(DefaultUnknownValue, PixelType);

  /** Set the DEM directory. */
  virtual void SetDEMDirectoryPath(const char* DEMDirectory);
  virtual void SetDEMDirectoryPath(const std::string& DEMDirectory);

protected:
  DEMToImageGenerator();
  virtual ~DEMToImageGenerator(){}

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId);
  virtual void GenerateOutputInformation();

  DEMHandlerType::Pointer m_DEMHandler;
  PointType               m_OutputOrigin;
  SpacingType             m_OutputSpacing;
  SizeType                m_OutputSize;
  PixelType               m_DefaultUnknownValue;

private:
  DEMToImageGenerator(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMToImageGenerator.txx"
#endif

#endif
