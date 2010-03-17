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
#ifndef __otbDEMToOrthoImageGenerator_h
#define __otbDEMToOrthoImageGenerator_h

#include <iostream>
#include <stdio.h>

#include "itkIndent.h"
#include "otbDEMToImageGenerator.h"
#include "otbImage.h"
#include "otbDEMHandler.h"
#include "itkImageRegionIteratorWithIndex.h"


namespace otb
{
/** \class DEMToOrthoImageGenerator
 *
 * \brief Class to generate an image from DEM data
 *
 * This class is based on ossimElevManager. It takes in input the upper left easting
 * and northing in a given map projection, the spacing and the output image size.
 * Handle DTED and SRTM formats.
 *
 * \ingroup Images
 *
 * \example IO/DEMToOrthoImageGenerator.cxx
 *
 */
template <class TDEMImage, class TMapProjection>
class ITK_EXPORT DEMToOrthoImageGenerator:
      public otb::DEMToImageGenerator<TDEMImage>
{
public :
  /** Standard class typedefs. */
  typedef itk::Indent                                        Indent;
  typedef TDEMImage                                          DEMImageType;
  typedef typename DEMImageType::Pointer                     DEMImagePointerType;
  typedef typename DEMImageType::PixelType                   PixelType;

  typedef TMapProjection                                     MapProjectionType;
  typedef typename MapProjectionType::Pointer                MapProjectionPointerType;

  typedef DEMToOrthoImageGenerator                           Self;
  typedef otb::DEMToImageGenerator<DEMImageType>             Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;
  typedef Image<PixelType,2>                                 OutputImageType;

  typedef typename Superclass::Pointer                       OutputImagePointer;
  typedef typename OutputImageType::SpacingType              SpacingType;
  typedef typename OutputImageType::SizeType                 SizeType;
  typedef typename OutputImageType::PointType                PointType;
  typedef typename OutputImageType::IndexType                IndexType;
  typedef typename Superclass::OutputImageRegionType         OutputImageRegionType;
  typedef itk::ImageRegionIteratorWithIndex< DEMImageType >  ImageIteratorType;

  typedef otb::DEMHandler                                    DEMHandlerType;
  typedef typename DEMHandlerType::Pointer                   DEMHandlerPointerType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DEMToOrthoImageGenerator,ImageSource);

  /** Set/Get the projection*/
  itkSetMacro(MapProjection, MapProjectionPointerType);
  itkGetMacro(MapProjection, MapProjectionPointerType);

protected:
  DEMToOrthoImageGenerator();
  virtual ~DEMToOrthoImageGenerator(){};

  void PrintSelf(std::ostream& os, Indent indent) const;
//  virtual void GenerateOutputInformation();
//   void AllocateOutputs();
  void BeforeThreadedGenerateData();
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId);

  MapProjectionPointerType m_MapProjection;

private:
  DEMToOrthoImageGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDEMToOrthoImageGenerator.txx"
#endif

#endif
