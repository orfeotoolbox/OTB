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
#ifndef __otbImageViewerHistogramAndTransferFunctionWidget_h
#define __otbImageViewerHistogramAndTransferFunctionWidget_h

#include "otbHistogramAndTransferFunctionWidget.h"

namespace otb
{
template <class TPixel, class TLabel> class ImageViewerBase;
/**
 * \class ImageViewerHistogramAndTransferFunctionWidget
 *
 * \brief Custom histogram widget for the image viewer.
 *
 * This class derives from otb::HistogramAndTransferFunctionWidget,
 * and implements behaviours specific to the image viewer.
 *
 * \sa ImageViewer, HistogramAndTransferFunctionWidget
 *
 */
template <class THistogram, class TPixel, class TLabel>
class ITK_EXPORT ImageViewerHistogramAndTransferFunctionWidget
  : public HistogramAndTransferFunctionWidget<THistogram,TPixel>
  {
    public:
  /** Standard typedefs */
  typedef ImageViewerHistogramAndTransferFunctionWidget         Self;
  typedef HistogramAndTransferFunctionWidget<THistogram,TPixel> Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageViewerHistogramAndTransferFunctionWidget,HistogramAndTransferFunctionWidget);

  /** Parent app typedefs */
  typedef TPixel PixelType;
  typedef TLabel LabelType;
  typedef ImageViewerBase<PixelType, LabelType> ParentType;
  typedef ParentType* ParentPointerType;

  typedef typename Superclass::TransferFunctionPointerType TransferFunctionPointerType;
  typedef ImageWidgetAffineTransferFunction<PixelType> AffineTransferFunctionType;
  typedef typename AffineTransferFunctionType::Pointer AffineTransferFunctionPointerType;
  typedef ImageWidgetSquareRootTransferFunction<PixelType> SquareRootTransferFunctionType;
  typedef typename SquareRootTransferFunctionType::Pointer SquareRootTransferFunctionPointerType;
  typedef ImageWidgetLogTransferFunction<PixelType> LogTransferFunctionType;
  typedef typename LogTransferFunctionType::Pointer LogTransferFunctionPointerType;
  typedef ImageWidgetSquareTransferFunction<PixelType> SquareonentialTransferFunctionType;
  typedef typename SquareonentialTransferFunctionType::Pointer SquareonentialTransferFunctionPointerType;

  /** Set the parent application */
  itkSetMacro(Parent,ParentPointerType);
  itkGetMacro(Parent,ParentPointerType);

  protected:
  // event handling
  virtual int handle(int event)
    {
      double factor = (this->GetHistogram()->Quantile(0,1.)-this->GetHistogram()->Quantile(0,0.))
  /(static_cast<double>(this->w())-2*this->GetMarginX()-this->GetOutputHistogramMargin()*static_cast<double>(this->w()));
      double xupper = this->GetMarginX() + static_cast<double>(this->GetTransferFunction()->GetUpperBound()-this->GetHistogram()->Quantile(0,0.))/factor;
      double xlower = this->GetMarginX() + static_cast<double>(this->GetTransferFunction()->GetLowerBound()-this->GetHistogram()->Quantile(0,0.))/factor;
      switch(event)
  {
  case FL_PUSH:
    {
      double x = Fl::event_x();
      if((vcl_abs(x-xlower)<50) || (vcl_abs(x-xupper)<50))
        {
    if(vcl_abs(x-xlower)<vcl_abs(x-xupper))
      {
        m_ModifyLower = true;
      }
    else
      {
        m_ModifyUpper = true;
      }
        }
      return 1;
    }
  case FL_RELEASE:
    {
      m_ModifyLower = false;
      m_ModifyUpper = false;
      m_Parent->ChangeTransferFunctions();
      m_Parent->Update();
      return 1;
    }
  case FL_DRAG:
    {
      double x = Fl::event_x();

      if(m_ModifyLower && (x>this->GetMarginX()) && (x<static_cast<double>(this->w())-this->GetMarginX()-this->GetOutputHistogramMargin()*static_cast<double>(this->w())))
        {
    x = (x>xupper ? xupper : x);
    this->GetTransferFunction()->SetLowerBound(static_cast<PixelType>(this->GetHistogram()->Quantile(0,0.)+(x-this->GetMarginX())*factor));
    this->redraw();
        }
      else if(m_ModifyUpper && (x<static_cast<double>(this->w())-this->GetMarginX()-this->GetOutputHistogramMargin()*static_cast<double>(this->w())))
        {
    x = (x<xlower ? xlower : x);
    this->GetTransferFunction()->SetUpperBound(static_cast<PixelType>(this->GetHistogram()->Quantile(0,0.)+(x-this->GetMarginX())*factor));
    this->redraw();
        }
      return 1;
    }
  case FL_MOUSEWHEEL:
    {
      int dy = Fl::event_dy();
      if(dy>0)
        {
    m_TransferFunctionCode++;
        }
      else
        {
    m_TransferFunctionCode--;
        }
      m_TransferFunctionCode = vcl_abs(m_TransferFunctionCode%4);
      TransferFunctionPointerType newFunction;
      switch(m_TransferFunctionCode)
        {
        case 0:
    newFunction = AffineTransferFunctionType::New();
    this->SetTransferFunctionLabel("Affine");
    break;
        case 1:
    newFunction = SquareRootTransferFunctionType::New();
    this->SetTransferFunctionLabel("Square Root");
    break;
        case 2:
    newFunction = LogTransferFunctionType::New();
    this->SetTransferFunctionLabel("Logarithmic");
    break;
        case 3:
    newFunction = SquareonentialTransferFunctionType::New();
    this->SetTransferFunctionLabel("Square");
    break;
        }
      newFunction->SetLowerBound(this->GetTransferFunction()->GetLowerBound());
      newFunction->SetUpperBound(this->GetTransferFunction()->GetUpperBound());
      this->SetTransferFunction(newFunction);
      m_Parent->ChangeTransferFunctions();
      m_Parent->Update();
      return 1;

    }
  }
      return 0;
    }
  /** Constructor */
  ImageViewerHistogramAndTransferFunctionWidget()
    {
      m_ModifyLower = false;
      m_ModifyUpper = false;
      m_TransferFunctionCode = 0;
    }
  /** Destructor */
  virtual ~ImageViewerHistogramAndTransferFunctionWidget()
    {
      m_Parent = NULL;
    }
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
  private:
  ImageViewerHistogramAndTransferFunctionWidget(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  /** Modify lower/upper threshold flag */
  bool m_ModifyLower;
  bool m_ModifyUpper;
  int m_TransferFunctionCode;
  ParentPointerType m_Parent;
  };

} // end namespace otb

#endif
