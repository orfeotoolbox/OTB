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
#ifndef __otbDrawLineSpatialObjectFilter_h
#define __otbDrawLineSpatialObjectFilter_h

#include "itkSpatialObjectToImageFilter.h"
#include "itkLineSpatialObject.h"

#include <list>

namespace otb
{

/** \class DrawLineSpatialObjectFilter
 * \brief Application of a filter which draw line in a binary image.
 *
 * This class implements a filter that draws line in a binary image.
 * Inputs are a LineSpatialObject and an input image that is used to
 * allocate the output image. This filter copies the input image in
 * the output image.
 *
 */

template <class TInputImage, class TOutputImage>
class  ITK_EXPORT DrawLineSpatialObjectFilter :
public itk::SpatialObjectToImageFilter< itk::LineSpatialObject<2>, TOutputImage >
{
public:
  /** 	Extract dimensions as well of the images of entry of exit. */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension,
  				unsigned int,
                      		TOutputImage::ImageDimension);


  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;


  /** typedef for the classes standards. */
  typedef DrawLineSpatialObjectFilter Self;
  typedef itk::SpatialObjectToImageFilter< itk::LineSpatialObject<2>, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  typedef typename Superclass::InputSpatialObjectType InputLineType;
  typedef typename InputLineType::PointListType  PointListType;

  typedef itk::ProcessObject ProcessObjectType;


  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the name of the class. */
  itkTypeMacro(DrawLineSpatialObjectFilter, SpatialObjectToImageFilter);

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** Definition of the size of the images. */
  typedef typename InputImageType::SizeType SizeType;

  typedef typename OutputImageType::IndexType IndexType;

  /** Set/Get the image input of this process object. */
  virtual void SetInputImage(const InputImageType *image);
  const InputImageType * GetInputImage(void);

  /** Get the input LineSpatialObjet (not const) */
  virtual void SetInputLine(const InputLineType *line);
  InputLineType * GetInput(void);

  // Set/Get pixel value
  itkSetMacro(Value, OutputPixelType);
  itkGetConstReferenceMacro(Value, OutputPixelType);


protected:
  DrawLineSpatialObjectFilter();
  virtual ~DrawLineSpatialObjectFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateData();

private:
  DrawLineSpatialObjectFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputPixelType m_Value;


};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDrawLineSpatialObjectFilter.txx"
#endif


#endif
