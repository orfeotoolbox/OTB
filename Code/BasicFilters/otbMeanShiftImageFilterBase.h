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
#ifndef _otbMeanShiftImageFilterBase_h
#define _otbMeanShiftImageFilterBase_h

#include "itkImageToImageFilter.h"
#include "itkVariableLengthVector.h"

namespace otb
{

  /** \class MeanShiftImageFilterBase
   *   
   *  This class is the base class for a mean shift edge-preserving smoothing filter. You can not instantiate
   *  it directly, since it contains pure virtual method. Consider using the MeanShiftImageFilter to
   *  process otb::Image or itk::Image, or the MeanShiftVectorImageFilter to process otb::VectorImage.
   * 
   * Mean shift is a data clustering algorithm ofthen used in image processing and segmentation.
   * For a given pixel, the mean shift will build a set of neighboring pixels within a given spatial 
   * radius (can be set using SetSpatialRadius()) and a color range (can be set using SetRangeRadius()). 
   * The spatial and color center of this set is then computed and the algorithm iterates with this new spatial
   * and color center. 
   * 
   * The algorithm ends when the maximum number of iteration has been reached (can be set using SetMaxNumberOfIterations())
   * or when the spatial center stops moving significantly according to a convegence distance threshold (can be set using
   * SetDistanceConvergenceThreshold()). 
   *
   * Pixel spatial distances are computed in pixels or physical coordinates, according to the flag UseImageSpacing (can be
   * et or unset by using UseImageSpacingOn() or UseImageSpacingOff()).
   *
   * For more information about mean shift techniques, one might consider reading the following article:
   * 
   * D. Comaniciu, P. Meer, "Mean Shift: A Robust Approach Toward Feature Space Analysis," IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 24, no. 5, pp. 603-619, May, 2002
   * D. Comaniciu, P. Meer, "Robust analysis of feature spaces: color image segmentation," cvpr, p. 750, 1997 IEEE Computer Society Conference on Computer Vision and Pattern Recognition (CVPR'97), 1997
   * D. Comaniciu, P. Meer, "Mean Shift Analysis and Applications," iccv, p. 1197, Seventh International Conference on Computer Vision (ICCV'99) - Volume 2, 1999
   *
   * \sa MeanShiftImageFilter
   * \sa MeanShiftVectorImageFilter
   *
   * \ingroup ImageEnhancement
   * \ingroup Streamed
   * \ingroup Threaded
   */

  template <class TInputImage, class TOutputImage, class TPrecision = double>
    class ITK_EXPORT MeanShiftImageFilterBase
    : public itk::ImageToImageFilter<TInputImage,TOutputImage>
    {
      public:
      /** Standard class typedef */
      typedef MeanShiftImageFilterBase                          Self;
      typedef itk::ImageToImageFilter<TInputImage,TOutputImage> Superclass;
      typedef itk::SmartPointer<Self>                           Pointer;
      typedef itk::SmartPointer<const Self>                     ConstPointer;

      /** Type macro */
      itkTypeMacro(MeanShiftImageFilterBase,ImageToImageFilter);
    
      /** Template parameters typedefs */
      typedef TInputImage                                  InputImageType;
      typedef typename InputImageType::Pointer             InputImagePointerType;
      typedef typename InputImageType::PixelType           InputPixelType;
      typedef typename InputImageType::PointType           PointType;
      typedef TOutputImage                                 OutputImageType;
      typedef typename OutputImageType::Pointer            OutputImagePointerType;
      typedef typename OutputImageType::PixelType          OutputPixelType;
      typedef typename OutputImageType::RegionType         RegionType;
      typedef typename RegionType::SizeType                SizeType;
      typedef typename RegionType::IndexType               IndexType;
      typedef typename InputImageType::SpacingType         SpacingType;
      typedef TPrecision                                   PrecisionPixelType;

      /** Setters / Getters */
      itkSetMacro(SpatialRadius,double);
      itkGetMacro(SpatialRadius,double);
      itkSetMacro(RangeRadius,double);
      itkGetMacro(RangeRadius,double);
      itkSetMacro(MaxNumberOfIterations,unsigned int);
      itkGetMacro(MaxNumberOfIterations,unsigned int);
      itkGetMacro(ConvergenceDistanceThreshold,double);
      itkSetMacro(ConvergenceDistanceThreshold,double);
      itkSetMacro(UseImageSpacing,bool);
      itkGetMacro(UseImageSpacing,bool);
      itkBooleanMacro(UseImageSpacing);

      protected:
      /** This filters use a neighborhood around the pixel, so it needs to redfine the 
       * input requested region */
      virtual void GenerateInputRequestedRegion();

      /** Threaded generate data */
      virtual void ThreadedGenerateData(const RegionType & outputRegionForThread,int threadId);

      /** Constructor */
      MeanShiftImageFilterBase();

      /** destructor */
      ~MeanShiftImageFilterBase(){};

      /**PrintSelf method */
      virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

      /** 
       * This method must be defined in order to handle VectorImage or Image.
       * \return the number of components per pixel of the input image.
       */
      virtual unsigned int GetNumberOfComponentsPerPixel() = 0;

      /** 
       * This method must be defined in order to handle VectorImage or Image.
       * \param value The value to intialize (please note that the reference will be modified)
       * \param nbComponents the number of components of the value to intialize.
       */
      virtual void InitValue(PrecisionPixelType & value, const unsigned int& nbComponents) = 0;

      /**
       * This method must be defined in order to handle VectorImage or Image.
       * \return The sqaured norm of the given value 
       */
      virtual double SquaredNorm(const PrecisionPixelType& value) = 0;

      /**
       * This method must be defined in order to handle VectorImage or Image.
       * It casts a pixel of type InputPixelType to a pixel of type PrecisionPixelType.
       * \param inputPixel the pixel to cast
       * \return the casted pixel.
       */
      virtual const PrecisionPixelType CastInputPixelToPrecisionPixel(const InputPixelType & pixel) = 0;

      /**
       * This method must be defined in order to handle VectorImage or Image.
       * It casts a pixel of type PrecisionPixelType to a pixel of type OutputPixelType.
       * \param pixel the pixel to cast
       * \return the casted pixel.
       */
      virtual const OutputPixelType CastPrecisionPixelToOutputPixel(const PrecisionPixelType & pixel) = 0;

      private:
      MeanShiftImageFilterBase(const Self&); //purposely not implemented
      void operator=(const Self&);             //purposely not implemented
 
      /** Spatial radius for mean shift convergence */
      double m_SpatialRadius;
      /** Range radius for mean shift convergence */
      double m_RangeRadius;
      /** Max number of iterations for convergence */
      unsigned int m_MaxNumberOfIterations;

      /** Internal radius used by the iterator. The value depends on the spatial radius
       *  and the maximum number of iterations. This value is not intended to be accessed
       *  by users, thus no getter or setter is provided */
      SizeType m_InternalRadius;

      /** use image spacing */
      bool m_UseImageSpacing;

      /** Internal spacing is [1.,1.] if UseImageSpacing is off */
      SpacingType m_InternalSpacing;

      /** Distance threshold for convergence in the spatial domain */
      double m_ConvergenceDistanceThreshold;
    };
}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftImageFilterBase.txx"
#endif

#endif
