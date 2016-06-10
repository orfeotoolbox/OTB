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
#ifndef __otbHoughTransform2DLinesImageFilter_h
#define __otbHoughTransform2DLinesImageFilter_h


#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "itkLineSpatialObject.h"

namespace otb
{

/**
 * \class HoughTransform2DLinesImageFilter
 * \brief Performs the Hough Transform to find 2D straight lines
 *        in a 2D image.
 *
 * This filter derives from ImageToImageFilter
 * The input is an image, and all pixels above some threshold are those
 * to be extracted. The output is the image of the accumulator.
 * GetLines() returns a list of LinesSpatialObjects
 *
 * Lines are parameterized in the form: R = x*vcl_cos(Teta)+y*vcl_sin(Teta)
 * where R is the perpendicular distance from the origin and Teta
 * the angle with the normal.
 *
 * The output is the accumulator array:
 *    -The first dimension (X) represents the distance R from the origin
 *     to the line. This is the distance axis. Its size depends on the size
 *     of the diagonal of the input image.
 *
 *    -The second dimension (Y) represents the angle between the X axis
 *     and the normal to the line. This is the angle axis. Its size depends
 *     on the AngleAxisSize parameter (500 by default) and its bounds can be
 *     set with the AngleAxisMinimum and AngleAxisMaximum parameters
 *     (-PI and +PI by default).
 *
 * \ingroup ImageFeatureExtraction
 * \sa LineSpatialObject
 *
 *
 * \ingroup OTBEdge
 * */

template<typename TInputPixelType, typename TOutputPixelType>
class ITK_EXPORT HoughTransform2DLinesImageFilter :
  public itk::ImageToImageFilter< Image<TInputPixelType, 2>, Image<TOutputPixelType, 2> >
{
public:

  /** Standard "Self" typedef. */
  typedef HoughTransform2DLinesImageFilter Self;

  /** Input Image typedef */
  typedef otb::Image<TInputPixelType, 2>        InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;

  /** Output Image typedef */
  typedef otb::Image<TOutputPixelType, 2>   OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;

  /** Smart pointer typedef support. */
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Line typedef */
  typedef itk::LineSpatialObject<2>  LineType;
  typedef typename LineType::Pointer LinePointer;
  typedef std::list<LinePointer>     LinesListType;
  typedef LineType::LinePointType    LinePointType;

  /** Standard "Superclass" typedef. */
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;

  /** Image index typedef */
  typedef typename InputImageType::IndexType IndexType;

  /** Image pixel value typedef */
  typedef typename InputImageType::PixelType PixelType;

  /** Typedef to describe the output image region type. */
  typedef typename InputImageType::RegionType OutputImageRegionType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(HoughTransform2DLinesImageFilter, ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Method for evaluating the implicit function over the image. */
  void GenerateData() ITK_OVERRIDE;

  /** Accessors for the threshold above which the filter should consider
      the point as a valid point */
  itkSetMacro(Threshold, float);
  itkGetMacro(Threshold, float);

  /** Accessors for the parameters of the angle axis.
      The Hough space describes (in the angle direction)
      [AngleAxisMinimum, AngleAxisMaximum[ with AngleAxisSize values */
  /** Accessors for the size of the angle axis */
  itkSetMacro(AngleAxisSize, unsigned int);
  itkGetMacro(AngleAxisSize, unsigned int);

  /** Accessors for the minimum of the angle axis */
  itkSetMacro(AngleAxisMinimum, double);
  itkGetMacro(AngleAxisMinimum, double);

  /** Accessors for the maximum of the angle axis */
  itkSetMacro(AngleAxisMaximum, double);
  itkGetMacro(AngleAxisMaximum, double);

  /** Accessors for the parameters of the distance axis.
      The Hough space describes (in the distance direction)
      [DistanceAxisMinimum, DistanceAxisMaximum[ with DistanceAxisSize values */
  /** Accessors for the actual size of the Distance axis */
  itkSetMacro(DistanceAxisSize, unsigned int);
  itkGetMacro(DistanceAxisSize, unsigned int);

  /** Accessors for the maximum size of the Distance axis */
  itkGetMacro(DistanceAxisMaximumSize, unsigned int);

  /** Accessors for the minimum of the Distance axis */
  itkSetMacro(DistanceAxisMinimum, double);
  itkGetMacro(DistanceAxisMinimum, double);

  /** Accessors for the maximum of the Distance axis */
  itkSetMacro(DistanceAxisMaximum, double);
  itkGetMacro(DistanceAxisMaximum, double);

  /** Set/Get the number of lines to extract */
  itkSetMacro(NumberOfLines, unsigned int);
  itkGetMacro(NumberOfLines, unsigned int);

  /** Set/Get the radius of the disc to remove from the accumulator
   *  for each line found */
  itkSetMacro(DiscRadius, float);
  itkGetMacro(DiscRadius, float);

  /** Set/Get the variance of the gaussian bluring for the accumulator */
  itkSetMacro(Variance, float);
  itkGetMacro(Variance, float);

  /** Simplify the accumulator */
  void Simplify(void);

  /** Get the Simplified accumulator */
  itkGetObjectMacro(SimplifyAccumulator, OutputImageType);

  /** Get the list of lines. This recomputes the lines */
  LinesListType& GetLines(unsigned int n = 0);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(IntConvertibleToOutputCheck,
                  (itk::Concept::Convertible<int, TOutputPixelType>));
  itkConceptMacro(InputGreaterThanFloatCheck,
                  (itk::Concept::GreaterThanComparable<PixelType, float>));
  itkConceptMacro(OutputPlusIntCheck,
                  (itk::Concept::AdditiveOperators<TOutputPixelType, int>));
  /** End concept checking */
#endif

protected:

  HoughTransform2DLinesImageFilter();
  ~HoughTransform2DLinesImageFilter() ITK_OVERRIDE {}

  HoughTransform2DLinesImageFilter(const Self &) {}
  void operator =(const Self&) {}

  void PrintSelf(std::ostream& os, itk::Indent indent) const ITK_OVERRIDE;

  /** HoughTransform2DLinesImageFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ITK_OVERRIDE;

  /** HoughTransform2DLinesImageFilter's output is the accumulator
   * array.  The size of the output is a function of the size of the
   * input and of the angle and distance axis sizes. Since this output
   * has a different size than the input, it must provide an implementation
   * of GenerateOutputInformation.
   * \sa ProcessObject::GenerateOutputRequestedRegion() */
  void GenerateOutputInformation() ITK_OVERRIDE;

  /** HoughTransform2DLinesImageFilter must produce the entire output */
  void EnlargeOutputRequestedRegion(itk::DataObject *output) ITK_OVERRIDE;

  int GetAngleIndex(double);

  double GetAngleValue(int);

  int GetDistanceIndex(double);

  double GetDistanceValue(int);

private:

  unsigned int       m_AngleAxisSize;
  double             m_AngleAxisMinimum;
  double             m_AngleAxisMaximum;
  double             m_AngleAxisIncrement;
  unsigned int       m_DistanceAxisSize;
  unsigned int       m_DistanceAxisMaximumSize;
  double             m_DistanceAxisMinimum;
  double             m_DistanceAxisMaximum;
  double             m_DistanceAxisIncrement;
  float              m_Threshold;
  OutputImagePointer m_SimplifyAccumulator;
  LinesListType      m_LinesList;
  unsigned int       m_NumberOfLines;
  float              m_DiscRadius;
  float              m_Variance;
  unsigned long      m_OldModifiedTime;
  unsigned long      m_OldNumberOfLines;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHoughTransform2DLinesImageFilter.txx"
#endif

#endif
