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
#ifndef __otbBinaryImageToDensityImageFilter_h
#define __otbBinaryImageToDensityImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkDataObject.h"

namespace otb
{
/** \class 
 *  \brief This class performs a streaming isotropic shrinking operation without smoothing.
 *
 */
template <class TInputImage,class TOutputImage   , class TCountFunction>
class ITK_EXPORT BinaryImageToDensityImageFilter
      : public itk::ImageToImageFilter<TInputImage,TOutputImage  >
{
public:
  /** Standard typedefs */
  typedef BinaryImageToDensityImageFilter                     Self;
  typedef itk::ImageToImageFilter<TInputImage,TOutputImage>   Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(BinaryImageToDensityImageFilter,itk::ImageToImageFilter);

  /** Template parameter typedefs */
  typedef TInputImage                                          InputImageType;
  typedef typename InputImageType::RegionType                  InputImageRegionType;
  typedef typename InputImageType::Pointer                     InputImagePointerType;

  typedef TOutputImage                                         OutputImageType;
  typedef typename OutputImageType::Pointer                    OutputImagePointerType;

  typedef TCountFunction                                       CountFunctionType;
  typedef typename CountFunctionType::Pointer                  CountFunctionPointerType;             

  /** Shrink factor accessor */
  itkSetMacro(NeighborhoodRadius,unsigned int);
  itkGetMacro(NeighborhoodRadius, unsigned int);



  /** Main computation method */

  virtual void ThreadedGenerateData( const InputImageRegionType &outputRegionForThread, int threadId ) ;


protected:
  /** Constructor */
  BinaryImageToDensityImageFilter();
  /** Destructor */
  ~BinaryImageToDensityImageFilter();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  BinaryImageToDensityImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  CountFunctionPointerType    m_CountFunction;

  /** The shrink factor */
  unsigned int m_NeighborhoodRadius;

};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBinaryImageToDensityImageFilter.txx"
#endif

#endif
