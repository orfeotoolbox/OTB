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
#ifndef __otbHistogramCurve_h
#define __otbHistogramCurve_h

#include "otbCurve2D.h"
#include "itkContinuousIndex.h"

namespace otb
{
/** \class HistogramCurve
*   \brief Class for histogram rendering.
*   
*   \sa ImageViewerModel
*/

template <class THistogram> 
class HistogramCurve
  : public Curve2D
{
public:
  /** Standard class typedefs */
  typedef HistogramCurve                       Self;
  typedef Curve2D                              Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  // Standard class macros
  itkNewMacro(Self);
  itkTypeMacro(HistogramCurve,Curve2D);
 
  /// Superclass typedefs
  typedef typename Superclass::AffineTransformType AffineTransformType;
  typedef typename Superclass::RegionType          RegionType;
  typedef typename Superclass::PointType           PointType;
  typedef typename Superclass::VectorType          VectorType;
  typedef typename Superclass::ColorType           ColorType;
  typedef itk::ContinuousIndex<double,2>           ContinuousIndexType;

  /// Histogram typedef
  typedef THistogram                               HistogramType;
  typedef typename HistogramType::Pointer          HistogramPointerType;
  typedef typename HistogramType::Iterator         HistogramIteratorType;

  /// Render the curve according to display extent and axis characteristics
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform);
  
  /// Pre-computation
  virtual void BeforeRendering();

  /// Get the min for each axis from the data available 
  virtual PointType GetMinimum();

  /// Get the max for each axis from the data available
  virtual PointType GetMaximum();

  /** Set/Get the histogram */
  itkSetObjectMacro(Histogram,HistogramType);
  itkGetObjectMacro(Histogram,HistogramType);
  
  /** Set/Get the histogram color */
  itkSetMacro(HistogramColor,ColorType);
  itkGetMacro(HistogramColor,ColorType);

protected:
  /** Constructor */
  HistogramCurve();
  /** Destructor */
  virtual ~HistogramCurve();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

private:
  HistogramCurve(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // The histogram to render
  HistogramPointerType m_Histogram;
  
  // The histogram color
  ColorType            m_HistogramColor;

  PointType            m_Minimum;
  PointType            m_Maximum;
  double               m_BinWidth;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramCurve.txx"
#endif

#endif


