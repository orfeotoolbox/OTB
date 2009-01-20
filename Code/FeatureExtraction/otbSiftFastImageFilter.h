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
#ifndef __otbSiftFastImageFilter_h
#define __otbSiftFastImageFilter_h

#include "otbImageToPointSetFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbImage.h"

namespace otb
{


  /** \class SiftFastImageFilter
   *  \brief This class extracts key points from an input image through a pyramidal decomposition
   *  
   * This class implements the SIFT key point detector proposed by David G. Lowe in proceedings of International
   * Conference on Computer Vision, Corfu, september 1999.
   *
   * \example FeatureExtraction/SIFTFastExample.cxx
   */
  template <class TInputImage, class TOutputPointSet>
    class ITK_EXPORT SiftFastImageFilter
    : public ImageToPointSetFilter<TInputImage,TOutputPointSet>
    {
    public:
      /** Standard typedefs */
      typedef SiftFastImageFilter                                Self;
      typedef ImageToPointSetFilter<TInputImage,TOutputPointSet> Superclass;
      typedef itk::SmartPointer<Self>                            Pointer;
      typedef itk::SmartPointer<const Self>                      ConstPointer;

      /** Creation through object factory macro */
      itkNewMacro(Self);

      /** Type macro */
      itkTypeMacro(SiftFastImageFilter,ImageToPointSetFilter);

      /** Template parameters typedefs */
     
      typedef TInputImage                     InputImageType;
      typedef typename TInputImage::Pointer   InputImagePointerType;
      typedef typename TInputImage::PixelType PixelType;

      typedef TOutputPointSet OutputPointSetType;
      typedef typename TOutputPointSet::Pointer OutputPointSetPointerType;
      typedef typename TOutputPointSet::PixelType OutputPixelType;
      typedef typename TOutputPointSet::PointType OutputPointType;
      typedef typename TOutputPointSet::PointIdentifier OutputPointIdentifierType;

      typedef otb::Image<float,2> FloatImageType;

      // Used to rescale data in the [0,1] range
      typedef itk::RescaleIntensityImageFilter<InputImageType,FloatImageType> RescalerType;

      itkSetMacro(NumberOfScales,unsigned int);
      itkGetMacro(NumberOfScales,unsigned int);
     
    protected:
      /** Actually process the input */
      virtual void GenerateData();

      /** Constructor */
      SiftFastImageFilter();

      /** Destructor */
      virtual ~SiftFastImageFilter() {}

      /** PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

    private:
      /** The number of scales */
      unsigned int m_NumberOfScales;


    };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSiftFastImageFilter.txx"
#endif

#endif
