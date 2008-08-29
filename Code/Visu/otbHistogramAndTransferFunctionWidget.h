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
#ifndef _otbHistogramAndTransferFunctionWidget_h
#define _otbHistogramAndTransferFunctionWidget_h

#include "itkProcessObject.h"
#include "itkFixedArray.h"
#include "otbImageWidgetTransferFunction.h"
#include "FL/Fl_Gl_Window.H"
#include "otbMacro.h"


namespace otb
{
/** \class HistogramAndTransferFunctionWidget
 *  \brief 
 *
 * \ingroup Visu
 */
template <class THistogram, class TPixel>
class ITK_EXPORT HistogramAndTransferFunctionWidget
  : public Fl_Gl_Window, public itk::ProcessObject
{
 public:
  /** Standard typedefs */
  typedef HistogramAndTransferFunctionWidget Self;
  typedef itk::ProcessObject                  Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(HistogramAndTransferFunctionWidget,ProcessObject);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;
  typedef THistogram HistogramType;
  typedef typename HistogramType::ValueType HistogramValueType;
  typedef typename HistogramValueType::ValueType ValueType;
  typedef typename HistogramType::ConstPointer HistogramConstPointerType;
  typedef typename HistogramType::ConstIterator HistogramIteratorType;
  typedef itk::FixedArray<double,3> ColorType;

  typedef otb::ImageWidgetTransferFunction<PixelType> TransferFunctionType;
  typedef typename TransferFunctionType::Pointer TransferFunctionPointerType;
  typedef otb::ImageWidgetAffineTransferFunction<PixelType> AffineTransferFunctionType;


  itkSetMacro(Histogram,HistogramConstPointerType);
  itkGetMacro(Histogram,HistogramConstPointerType);
  itkGetMacro(TransferFunction,TransferFunctionPointerType);
  
  virtual void SetTransferFunction(TransferFunctionType * function)
    {
      m_TransferFunction = function;
      this->Modified();
    }

  itkSetMacro(HistogramColor,ColorType);
  itkGetMacro(HistogramColor,ColorType);
  itkSetMacro(BackgroundColor,ColorType);
  itkGetMacro(BackgroundColor,ColorType);
  itkSetMacro(AxisColor,ColorType);
  itkGetMacro(AxisColor,ColorType);
  itkSetMacro(TextColor,ColorType);
  itkGetMacro(TextColor,ColorType);
  itkSetMacro(TransferFunctionColor,ColorType);
  itkGetMacro(TransferFunctionColor,ColorType);
  
  itkSetMacro(Label,std::string);
  itkGetMacro(Label,std::string);
  itkSetMacro(TransferFunctionLabel,std::string);
  itkGetMacro(TransferFunctionLabel,std::string);

  itkSetMacro(MarginX,double);
  itkGetMacro(MarginX,double);
  itkSetMacro(MarginY,double);
  itkGetMacro(MarginY,double);
  itkSetMacro(OutputHistogramMargin,double);
  itkGetMacro(OutputHistogramMargin,double);

  itkSetMacro(GridSizeX,unsigned int);
  itkGetMacro(GridSizeX,unsigned int);
  itkSetMacro(GridSizeY,unsigned int);
  itkGetMacro(GridSizeY,unsigned int);
  
protected:
  /** Constructor */
  HistogramAndTransferFunctionWidget();
  /** Destructor */
  virtual ~HistogramAndTransferFunctionWidget() {};
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Draw the histogram */
  virtual void draw(void);

  virtual void OpenGlSetup();

  virtual void AxisRendering();

  virtual void GridRendering(double gridXSpacing, double gridYSpacing);

  virtual void HistogramRendering(double binWidth, double binHeightRatio, double maxFrequency);

  virtual void LegendRendering(double gridXSpacing, double gridYSpacing, double maxFrequency);
  
  virtual void TransferFunctionRendering(void);

  virtual void OutputHistogramRendering(void);

  virtual void BoundRendering(void);

  virtual void TransferFunctionLabelRendering(void);

private:
  HistogramAndTransferFunctionWidget(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** The histogram to draw */
  HistogramConstPointerType m_Histogram;

  /** The transfert function */
  TransferFunctionPointerType m_TransferFunction;

  /** Colors */
  ColorType m_HistogramColor;
  ColorType m_BackgroundColor;
  ColorType m_AxisColor;
  ColorType m_GridColor;
  ColorType m_TextColor;
  ColorType m_TransferFunctionColor;
  ColorType m_BoundColor;
  /** Margin around histogram */
  double m_MarginX;
  double m_MarginY;
  double m_OutputHistogramMargin;
  /** Set grid spatial frequency */
  unsigned int m_GridSizeX;
  unsigned int m_GridSizeY;
  /** Percentage of the max frequency beeing thresholded */
  double  m_HistogramClamping;
  /** Updating flag */
  bool m_Updating;
  /** Label of the histogram */
  std::string m_Label;
/** Label of the histogram */
  std::string m_TransferFunctionLabel;
  
};
}// End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbHistogramAndTransferFunctionWidget.txx"
#endif

#endif
