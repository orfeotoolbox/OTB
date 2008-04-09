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
#ifndef _otbImageToSIFTKeyPointSetFilter_h
#define _otbImageToSIFTKeyPointSetFilter_h

#include "itkExpandImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkShrinkImageFilter.h"
#include "itkConstNeighborhoodIterator.h"

#include "otbImageToPointSetFilter.h"
#include "otbImageList.h"

namespace otb
{

  /** \class ImageToSIFTKeyPointSetFilter
   *  \brief This class extracts key points from an input image, trough a pyramidal decomposition.
   *
   * This class implements the SIFT key point detector proposed by David G. Lowe in proceedings of International
   * Conference on Computer Vision, Corfu, september 1999.
   *
   * Selected key points are stored in a itk::PointSet structure.
   * Data points contains a list of (magnitude, main orientation) for each level
   * of the pyramidal decomposition.
   * 
   * Orientation is expressed in degree in the range [0,360] with a precision of 10 degrees.
   *
   */
  template <class TInputImage, class TOutputPointSet>
    class ITK_EXPORT ImageToSIFTKeyPointSetFilter
    : public ImageToPointSetFilter<TInputImage,TOutputPointSet>
    {
    public:
      /** Standard typedefs */
      typedef ImageToSIFTKeyPointSetFilter                       Self;
      typedef ImageToPointSetFilter<TInputImage,TOutputPointSet> Superclass;
      typedef itk::SmartPointer<Self>                            Pointer;
      typedef itk::SmartPointer<const Self>                      ConstPointer;
  
      /** Creation through object factory macro */
      itkNewMacro(Self);  

      /** Type macro */
      itkTypeMacro(ImageToSIFTKeyPointSetFilter,ImageToPointSetFilter);
  
      /** Template parameters typedefs */
      typedef TInputImage InputImageType;
      typedef typename TInputImage::Pointer InputImagePointerType;
      
      typedef TOutputPointSet OutputPointSetType;
      typedef typename TOutputPointSet::Pointer OutputPointSetPointerType;
      typedef typename TOutputPointSet::PixelType OutputPixelType;
      typedef typename TOutputPointSet::PointType OutputPointType;
      typedef typename TOutputPointSet::PointIdentifier OutputPointIdentifierType;
      	
      /** Set/Get the number of octaves */
      itkSetMacro(OctavesNumber, unsigned int);
      itkGetMacro(OctavesNumber, unsigned int);
      
      /** Set/Get the number of scales */
      itkSetMacro(ScalesNumber, unsigned int);
      itkGetMacro(ScalesNumber, unsigned int);

      /** Set/Get the expand factors */
      itkSetMacro(ExpandFactors, unsigned int);
      itkGetMacro(ExpandFactors, unsigned int);
      
      /** Set/Get the sigma 0 */
      itkSetMacro(Sigma0, double);
      itkGetMacro(Sigma0, double);
      
      /** Internal typedefs */
      typedef itk::ExpandImageFilter<TInputImage, TInputImage> ExpandFilterType;
      typedef typename ExpandFilterType::Pointer ExpandFilterPointerType;
      
      typedef itk::RecursiveGaussianImageFilter<InputImageType,InputImageType> GaussianFilterType;
      typedef typename GaussianFilterType::Pointer GaussianFilterPointerType;
      
      typedef otb::ImageList<InputImageType> ImageListType;
      typedef typename ImageListType::Pointer ImageListPointerType;
      
      typedef itk::SubtractImageFilter<InputImageType,InputImageType,InputImageType> SubtractFilterType;
      typedef typename SubtractFilterType::Pointer SubtractFilterPointerType;

      typedef itk::ShrinkImageFilter<InputImageType, InputImageType> ShrinkFilterType;
      typedef typename ShrinkFilterType::Pointer ShrinkFilterPointerType;
      
      typedef itk::ConstNeighborhoodIterator<InputImageType> NeighborhoodIteratorType;
      typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;
      typedef typename NeighborhoodType::OffsetType OffsetType;
      
    protected:
      /** Actually process the input */
      virtual void GenerateData();
      
      /** Constructor */
      ImageToSIFTKeyPointSetFilter();
      
      /** Destructor */
      virtual ~ImageToSIFTKeyPointSetFilter() {}
      
      /** PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

      /** Initialize input image */
      void initializeInputImage();
      
      /** Compute differenec of gaussian
       * 
       *  \param input, current input in process
       */
      void computeDifferenceOfGaussian(InputImagePointerType input);
      
      /** Localize key point */
      void localizeKeyPoint();
      
      /** Check local extremum for 26 neighbors (current and adjacents scales)
       *
       *  \param currentScale
       *  \param previousScale
       *  \param nextScale
       *
       *  \return true if the pixel is extremum
       */
      bool isLocalExtremum( const NeighborhoodIteratorType& currentScale,
			    const NeighborhoodIteratorType& previousScale,
			    const NeighborhoodIteratorType& nextScale) const;
      
    private:
      ImageToSIFTKeyPointSetFilter(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      /** Number of octaves */
      unsigned int m_OctavesNumber;
      
      /** Number of scale for aech octave */
      unsigned int m_ScalesNumber;
      
      /** Expand factors */
      unsigned int m_ExpandFactors;

      /** Sigma 0 */
      typename GaussianFilterType::ScalarRealType m_Sigma0;
      
      /** Sigma k */
      double m_Sigmak;
      
      /** Number of key points */
      OutputPointIdentifierType m_ValidatedKeyPoints;
      
      /** Expand filter */
      ExpandFilterPointerType m_ExpandFilter;
      
      /** Shrink filter */
      ShrinkFilterPointerType m_ShrinkFilter;
      
      /** Gaussian filter */
      GaussianFilterPointerType m_XGaussianFilter;
      GaussianFilterPointerType m_YGaussianFilter;
      
      /** Gaussian image list */
      ImageListPointerType m_GaussianList;
      
      /** Difference of gaussian list */
      ImageListPointerType m_DoGList;
      
      /** Subtract filter */
      SubtractFilterPointerType m_SubtractFilter;

      
    };
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageToSIFTKeyPointSetFilter.txx"
#endif

#endif
