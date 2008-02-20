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
#ifndef _otbImageViewerHistogramAndTransfertFunctionWidget_h
#define _otbImageViewerHistogramAndTransfertFunctionWidget_h

#include "otbHistogramAndTransfertFunctionWidget.h"

namespace otb
{
template <class TPixel, class TLabel> class ImageViewerBase;
/** 
 * \brief Custom histogram widget for the image viewer.
 *
 * This class derives from otb::HistogramAndTransfertFunctionWidget, 
 * and implements behaviours specific to the image viewer.
 *
 * \sa ImageViewer, HistogramAndTransfertFunctionWidget
 *
 */
template <class THistogram, class TPixel, class TLabel>
class ITK_EXPORT ImageViewerHistogramAndTransfertFunctionWidget
  : public HistogramAndTransfertFunctionWidget<THistogram,TPixel>
  {
    public:
  /** Standard typedefs */
  typedef ImageViewerHistogramAndTransfertFunctionWidget         Self;
  typedef HistogramAndTransfertFunctionWidget<THistogram,TPixel> Superclass;
  typedef itk::SmartPointer<Self>                                Pointer;
  typedef itk::SmartPointer<const Self>                          ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageViewerHistogramAndTransfertFunctionWidget,HistogramAndTransfertFunctionWidget);

  /** Parent app typedefs */
  typedef TPixel PixelType;
  typedef TLabel LabelType;
  typedef ImageViewerBase<PixelType, LabelType> ParentType;
  typedef ParentType* ParentPointerType;

  typedef typename Superclass::TransfertFunctionPointerType TransfertFunctionPointerType;
  typedef ImageWidgetAffineTransfertFunction<PixelType> AffineTransfertFunctionType;
  typedef typename AffineTransfertFunctionType::Pointer AffineTransfertFunctionPointerType;
  typedef ImageWidgetSquareRootTransfertFunction<PixelType> SquareRootTransfertFunctionType;
  typedef typename SquareRootTransfertFunctionType::Pointer SquareRootTransfertFunctionPointerType;
  typedef ImageWidgetLogTransfertFunction<PixelType> LogTransfertFunctionType;
  typedef typename LogTransfertFunctionType::Pointer LogTransfertFunctionPointerType;
  typedef ImageWidgetSquareTransfertFunction<PixelType> SquareonentialTransfertFunctionType;
  typedef typename SquareonentialTransfertFunctionType::Pointer SquareonentialTransfertFunctionPointerType;
    
  /** Set the parent application */
  itkSetMacro(Parent,ParentPointerType);
  itkGetMacro(Parent,ParentPointerType);

  protected:
  // event handling
  virtual int handle(int event)
    {
      double factor = (this->GetHistogram()->Quantile(0,1.)-this->GetHistogram()->Quantile(0,0.))
	/(static_cast<double>(this->w())-2*this->GetMarginX()-this->GetOutputHistogramMargin());
      double xupper = this->GetMarginX() + static_cast<double>(this->GetTransfertFunction()->GetUpperBound())/factor;
      double xlower = this->GetMarginX() + static_cast<double>(this->GetTransfertFunction()->GetLowerBound())/factor;
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
	    m_Parent->ChangeTransfertFunctions();
	    m_Parent->Update();
	    return 1;
	  }
	case FL_DRAG:
	  {
	    double x = Fl::event_x();
	    
	    if(m_ModifyLower && (x>this->GetMarginX()) && (x<static_cast<double>(this->w())-this->GetMarginX()-this->GetOutputHistogramMargin()))
	      {
		x = (x>xupper ? xupper : x);
		this->GetTransfertFunction()->SetLowerBound(static_cast<PixelType>((x-this->GetMarginX())*factor));
		this->redraw();
	      }
	    else if(m_ModifyUpper && (x<static_cast<double>(this->w())-this->GetMarginX()-this->GetOutputHistogramMargin()))
	      {
		x = (x<xlower ? xlower : x);
		this->GetTransfertFunction()->SetUpperBound(static_cast<PixelType>((x-this->GetMarginX())*factor));
		this->redraw();
	      }
	    return 1;
	  }
	case FL_MOUSEWHEEL:
	  {
	    int dy = Fl::event_dy();
	    if(dy>0)
	      {
		m_TransfertFunctionCode++;
	      }
	    else
	      {
		m_TransfertFunctionCode--;
	      }
	    m_TransfertFunctionCode = vcl_abs(m_TransfertFunctionCode%4);
	    TransfertFunctionPointerType newFunction;
	    switch(m_TransfertFunctionCode)
	      {
	      case 0:
		newFunction = AffineTransfertFunctionType::New();
		this->SetTransfertFunctionLabel("Affine");
		break;
	      case 1:
		newFunction = SquareRootTransfertFunctionType::New();
		this->SetTransfertFunctionLabel("Square Root");
		break;
	      case 2:
		newFunction = LogTransfertFunctionType::New();
		this->SetTransfertFunctionLabel("Logarithmic");
		break;
	      case 3:
		newFunction = SquareonentialTransfertFunctionType::New();
		this->SetTransfertFunctionLabel("Square");
		break;
	      }
	    newFunction->SetLowerBound(this->GetTransfertFunction()->GetLowerBound());
	    newFunction->SetUpperBound(this->GetTransfertFunction()->GetUpperBound());
	    this->SetTransfertFunction(newFunction);
	    m_Parent->ChangeTransfertFunctions();
	    m_Parent->Update();
	    return 1;

	  }
	}
      return 0;
    }
  /** Constructor */
  ImageViewerHistogramAndTransfertFunctionWidget()
    {
      m_ModifyLower = false;
      m_ModifyUpper = false;
      m_TransfertFunctionCode = 0;
    }
  /** Destructor */
  virtual ~ImageViewerHistogramAndTransfertFunctionWidget() 
    {
      m_Parent = NULL;
    }
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }
  private:
  ImageViewerHistogramAndTransfertFunctionWidget(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  /** Modify lower/upper threshold flag */
  bool m_ModifyLower;
  bool m_ModifyUpper;
  int m_TransfertFunctionCode;
  ParentPointerType m_Parent;
  };

} // end namespace otb

#endif
