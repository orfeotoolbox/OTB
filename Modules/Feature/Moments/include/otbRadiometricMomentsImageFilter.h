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
#ifndef __otbRadiometricMomentsImageFilter_h
#define __otbRadiometricMomentsImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbRadiometricMomentsFunctor.h"
#include "itkConstNeighborhoodIterator.h"

namespace otb
{

/**
 * \class RadiometricMomentsImageFilter
 * \brief Calculate the radiometric moments.
 *
 * Calculate the radiometric moments over a specified neighborhood
 *
 * This class is templated over the input image and the output image.
 *
 * \ingroup ImageFilters
 */


template <class TInputImage, class TOutputImage>
class ITK_EXPORT RadiometricMomentsImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
    typedef RadiometricMomentsImageFilter                      Self;
    typedef itk::ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
    typedef itk::SmartPointer<Self>                            Pointer;
    typedef itk::SmartPointer<const Self>                      ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(RadiometricMomentsImageFilter, ImageToImageFilter);

    /** Some convenient typedefs. */
    typedef typename Superclass::InputImageType           InputImageType;
    typedef typename InputImageType::ConstPointer         InputImagePointer;
    typedef typename InputImageType::RegionType           InputImageRegionType;
    typedef typename InputImageType::PixelType            InputImagePixelType;
    typedef typename InputImageType::SizeType             InputImageSizeType;
    typedef typename Superclass::OutputImageType          OutputImageType;
    typedef typename OutputImageType::Pointer             OutputImagePointer;
    typedef typename OutputImageType::RegionType          OutputImageRegionType;
    typedef typename OutputImageType::PixelType           OutputImagePixelType;
    typedef typename OutputImageType::InternalPixelType   ScalarType;

    typedef Functor::RadiometricMomentsFunctor< itk::ConstNeighborhoodIterator<InputImageType>, ScalarType>
                                                          FunctorType;

    typedef itk::ProcessObject ProcessObjectType;

    /**Set/Get the radius of neighborhood.*/
    itkSetMacro(Radius, InputImageSizeType);
    itkGetMacro(Radius, InputImageSizeType);

    /** Set unsinged int radius */
    void SetRadius(unsigned int radius)
    {
      m_Radius.Fill(radius);
    }

    typedef itk::ConstNeighborhoodIterator<TInputImage>   NeighborhoodIteratorType;
    typedef typename NeighborhoodIteratorType::RadiusType RadiusType;
    typedef unsigned char                                 RadiusSizeType;

protected:
  RadiometricMomentsImageFilter();
  virtual ~RadiometricMomentsImageFilter() {}
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);
  virtual void GenerateInputRequestedRegion(void);
  virtual void GenerateOutputInformation(void);

private:
  RadiometricMomentsImageFilter(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented

  InputImageSizeType m_Radius;
  FunctorType m_Functor;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRadiometricMomentsImageFilter.txx"
#endif

#endif
