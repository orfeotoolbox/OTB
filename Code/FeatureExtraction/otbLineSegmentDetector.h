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
#ifndef __otbLineSegmentDetector_h
#define __otbLineSegmentDetector_h


#include "otbImageToLineSpatialObjectListFilter.h"
#include "otbLineSpatialObjectList.h"
#include "otbImage.h"
#include "itkPointSet.h"

#include "itkUnaryFunctorImageFilter.h"
#include "itkGradientImageFilter.h"

namespace otb
{

namespace Functor
  {
    /** \class MagnitudeFunctor
     *  \brief This functor computes the magnitude of a covariant vector.
     */
    template <class TInputPixel,class TOutputPixel>
      class MagnitudeFunctor
      {
      public:
	
	inline TOutputPixel operator()(const TInputPixel& input)
	  {
	    return vcl_sqrt((input[0]+input[1])*(input[0]+input[1])  +(input[0]*input[1])*(input[0]*input[1]) );
	  }
      };
    
    /** \class OrientationFunctor
     *  \brief This functor computes the orientation of a cavariant vector<br>
     *   Orientation values lies between 0 and 2*Pi.
     */
    template <class TInputPixel,class TOutputPixel>
      class OrientationFunctor
      {
      public:
	
	inline TOutputPixel operator()(const TInputPixel& input)
	  {
	    TOutputPixel resp = vcl_atan2(input[1],-input[0]);
/* 	    if (resp<0) */
/* 	      { */
/* 		resp+=2*M_PI; */
/* 	      } */
	    
	    return resp;
	  }
      };
  }// end namespace Functor
/** \class LineSegmentDetector
 *  \brief this class implement a fast line detector with false detection control
 *
 *
 * 
 */

template <class TInputImage,class TPrecision>
class ITK_EXPORT LineSegmentDetector :
      public otb::ImageToLineSpatialObjectListFilter< TInputImage >
{
public:
  
  /** typedef for the classes standards. */
  typedef LineSegmentDetector                               Self;
  typedef ImageToLineSpatialObjectListFilter< TInputImage>  Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;
  
  /** Method for management of the object factory. */
  itkNewMacro(Self);
  
  /** Return the name of the class. */
  itkTypeMacro(LineSegmentDetector,ImageToLineSpatialObjectListFilter );

  /** Definition of the input image and the output ObjectList*/
  typedef TInputImage                                                   InputImageType;
  typedef typename InputImageType::PixelType                            InputPixelType;
  typedef typename InputImageType::IndexType                            InputIndexType;
  typedef typename InputImageType::SizeType                             SizeType;

  /** Definition of the list of lines. */
  typedef LineSpatialObjectList                                         LineSpatialObjectListType;
  typedef LineSpatialObjectListType::Pointer                            LineSpatialObjectListPointer;
  typedef LineSpatialObjectListType::LineType                           LineSpatialObjectType;
  typedef LineSpatialObjectType::PointListType                          PointListType;
  typedef LineSpatialObjectType::LinePointType                          PointType;
  

  /** Definition of temporary image ised to store LABELS*/
  typedef otb::Image<TPrecision ,2>                                     OutputImageType;
  typedef typename OutputImageType::PixelType                           OutputPixelType;
   typedef typename OutputImageType::IndexType                          OutputIndexType;
  typedef typename OutputImageType::SizeType                            OutputSizeType;
  
  /** Histogram to store the coordinate of ordered pixels*/
  typedef std::vector<OutputIndexType>                                  IndexVectorType;
  typedef typename IndexVectorType::iterator                            IndexVectorIteratorType;
  typedef std::vector<IndexVectorType >                                 CoordinateHistogramType;
  typedef typename CoordinateHistogramType::iterator                    CoordinateHistogramIteratorType;

   
  /** typedef structure to store REGION*/
  typedef std::vector<IndexVectorType>                                  VectorOfIndexVectorType;
  typedef std::vector<float>                                            DirectionVectorType;
  typedef typename DirectionVectorType::iterator                        DirectionVectorIteratorType; 

  /** */ 
  typedef itk::GradientImageFilter<InputImageType,InputPixelType,InputPixelType > GradientFilterType;
  typedef typename GradientFilterType::Pointer GradientFilterPointerType;
  typedef typename GradientFilterType::OutputImageType GradientOutputImageType;


  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
  Functor::MagnitudeFunctor<typename GradientOutputImageType::PixelType,typename  InputImageType::PixelType> > MagnitudeFilterType;
  typedef typename MagnitudeFilterType::Pointer                                   MagnitudeFilterPointerType;
  typedef typename MagnitudeFilterType::OutputImageType::PixelType                MagnitudePixelType;
            
  typedef itk::UnaryFunctorImageFilter<GradientOutputImageType,InputImageType,
  Functor::OrientationFunctor<typename GradientOutputImageType::PixelType,typename InputImageType::PixelType> > OrientationFilterType;
  typedef typename OrientationFilterType::Pointer OrientationFilterPointerType;
  typedef typename OrientationFilterType::OutputImageType                         OutputImageDirType;
  typedef typename OutputImageDirType::RegionType                                 OutputImageDirRegionType ;

  /** Create an image to store the label USED(1) or notUsed (0)*/
  typedef otb::Image<unsigned char, 2>                              LabelImageType;
  typedef typename LabelImageType::Pointer                          LabelImagePointerType;
  
  /** Vector to store the rectangle characteization  center, width, orientation ,( begin ,end ) of the central line*/
  typedef std::vector<float>                                        RectangleType;
  typedef typename RectangleType::iterator                          RectangleIteratorType;
  typedef std::vector< RectangleType>                               RectangleListType;
  typedef typename RectangleListType::iterator                      RectangleListTypeIterator; 
protected:
  LineSegmentDetector();
  virtual ~LineSegmentDetector() {};

  /** Generate Data method*/
  virtual void GenerateData();
  
  /** Sort the image and store the coordinates in a histogram
   *  this method is used to determine the seeds where to begin the search segments
   *  Points with large gradient modulus are more able to belong to a segment
   */
  virtual  CoordinateHistogramType SortImageByModulusValue(OutputImageType * modulusImage);

  /** */
  virtual void LineSegmentDetection(CoordinateHistogramType * CoordinateHistogram);
  
  /** */
  virtual bool IsUsed(InputIndexType  index);
  
   /** Set Pixel flag to USED*/
  virtual void SetPixelToUsed(InputIndexType  index);
  
 
  /** search for a segment which begins from a seed "index "*/
  virtual void GrowRegion(InputIndexType  index);
  
  /** Define if two are aligned */
  virtual bool IsAligned(float Angle, float regionAngle, float prec);
  
  /** For each region of the region List it builds a rectangle */
  virtual int ComputeRectangles();
  
  /** */
  virtual void Region2Rect(IndexVectorType  region , float angleRegion);
  
  /** */
  virtual float ComputeRegionOrientation(IndexVectorType  region , float x, float y , float angleRegion);

  /** */
  virtual float angle_diff(float a, float b);

  /**  Compute the Number Of False Alarm for a rectangle*/
  virtual float ComputeRectNFA(RectangleType  rec);

  /** */
  virtual float ImproveRectangle(RectangleType  * rec , float NFA);

  /** NFA For a rectangle*/
  virtual float NFA(int n, int k, double p, double logNT);
  
  /** Create a copy of a rectangle*/
  virtual void CopyRectangle(RectangleType * rDst , RectangleType  *rSrc );
  

  /** Rutines from numerical recipies*/
  virtual float betacf(float a, float b, float x);
  virtual float gammln(float xx);
  virtual float betai(float a, float b, float x);
  virtual float factln(int n);

  /** Printself method*/
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  LineSegmentDetector(const Self&);  //purposely not implemented
  void operator=(const Self&);      //purposely not implemented

  VectorOfIndexVectorType           m_RegionList;
  DirectionVectorType               m_DirectionVector;
  LabelImagePointerType             m_UsedPointImage;
  RectangleListType                 m_RectangleList;
  
  float                             m_Threshold;
  unsigned int                      m_MinimumRegionSize;
  unsigned int                      m_NumberOfImagePixels;
  
  /** Gradient filter */
  GradientFilterPointerType m_GradientFilter;

  /** Magnitude filter */
  MagnitudeFilterPointerType m_MagnitudeFilter;

  /** Orientation filter */
  OrientationFilterPointerType m_OrientationFilter;

  /** Output*/
  LineSpatialObjectListPointer      m_LineList;
  
  
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLineSegmentDetector.txx"
#endif


#endif

