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
#ifndef __otbAngularProjectionBinaryImageFilter_h
#define __otbAngularProjectionBinaryImageFilter_h

#include <vector>

#include "itkImageToImageFilter.h"


namespace otb {
/** \class AngularProjectionBinaryImageFilter
 * \brief Performs \f$ y_i = \cos \theta_i x_1 + \sin \theta_i x_2\f$
 *
 * This class performs the projections of the 2 input images to a set of N
 * output images according to N angular values
 *
 */
template < class TInputImage, class TOutputImage, class TPrecision >
class ITK_EXPORT AngularProjectionBinaryImageFilter
  : public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef AngularProjectionBinaryImageFilter                 Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(AngularProjectionBinaryImageFilter, ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::Pointer    InputImagePointerType;
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename InputImageType::PixelType  InputPixelType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::SizeType   OutputSizeType;
  typedef typename OutputImageType::IndexType  OutputIndexType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  typedef TPrecision PrecisionType;

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  /** Set/Get Angle set */
  std::vector<PrecisionType> GetAngleSet() const
  {
    return m_AngleSet;
  }
  void SetAngleSet ( std::vector<PrecisionType> & angle );

  void SetInput1 ( const InputImageType * );
  const InputImageType * GetInput1 () const;

  void SetInput2 ( const InputImageType * );
  const InputImageType * GetInput2 () const;

protected:
  AngularProjectionBinaryImageFilter();
  virtual ~AngularProjectionBinaryImageFilter() { }

  virtual void GenerateOutputInformation();
  virtual void ThreadedGenerateData( const OutputImageRegionType & outputRegionForThread, itk::ThreadIdType threadID );

private:
  AngularProjectionBinaryImageFilter(const Self&); // not implemented
  void operator=(const Self&); // not implemented

  std::vector<PrecisionType> m_AngleSet;

}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAngularProjectionBinaryImageFilter.txx"
#endif


#endif

