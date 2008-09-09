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
#ifndef _otbMeanShiftVectorImageFilter_h
#define _otbMeanShiftVectorImageFilter_h

#include "itkInPlaceImageFilter.h"

namespace otb
{

  /** \class MeanShiftVectorImageFilter
   *   
   *
   *
   *
   * \ingroup Streamed
   * \ingroup Threaded
   */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MeanShiftVectorImageFilter
  : public itk::InPlaceImageFilter<TInputImage,TOutputImage>
  {
  public:
    /** Standard class typedef */
    typedef MeanShiftVectorImageFilter                        Self;
    typedef itk::InPlaceImageFilter<TInputImage,TOutputImage> Superclass;
    typedef itk::SmartPointer<Self>                           Pointer;
    typedef itk::SmartPointer<const Self>                     ConstPointer;

    /** New and Type macros */
    itkNewMacro(Self);
    itkTypeMacro(MeanShiftVectorImageFilter,InPlaceImageFilter);
    
    /** Template parameters typedefs */
    typedef TInputImage                                  InputImageType;
    typedef typename InputImageType::Pointer             InputImagePointerType;
    typedef typename InputImageType::PixelType           InputPixelType;
    typedef typename InputImageType::InternalPixelType   InputInternalPixelType;
    typedef typename InputImageType::PointType           PointType;
    typedef TOutputImage                                 OutputImageType;
    typedef typename OutputImageType::Pointer            OutputImagePointerType;
    typedef typename OutputImageType::PixelType          OutputPixelType;
    typedef typename OutputImageType::InternalPixelType  OutputInternalPixelType;
    typedef typename OutputImageType::RegionType         RegionType;
    typedef typename RegionType::SizeType                SizeType;

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
    MeanShiftVectorImageFilter();

    /** destructor */
    ~MeanShiftVectorImageFilter(){};

   /**PrintSelf method */
    virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  private:
    MeanShiftVectorImageFilter(const Self&); //purposely not implemented
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

    /** Distance threshold for convergence in the spatial domain */
    double m_ConvergenceDistanceThreshold;
  };
}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMeanShiftVectorImageFilter.txx"
#endif

#endif
