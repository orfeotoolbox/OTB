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
#ifndef __otbHistogramActionHandler_h
#define __otbHistogramActionHandler_h

#include "otbCurves2DWidget.h"
#include "otbVerticalAsymptoteCurve.h" 


namespace otb
{
/** \class HistogramActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*  \ingroup Visualization
 */

template <class TModel, class TView,class TRenderingFunction>
class ITK_EXPORT HistogramActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef HistogramActionHandler            Self;
  typedef ImageWidgetActionHandler          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(HistogramActionHandler,ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TView                                           ViewType;
  typedef typename ViewType::Pointer                      ViewPointerType;
  typedef typename ViewType::ImageWidgetType::PointType   PointType;

  /** Curve Widget*/
  typedef Curves2DWidget                       CurveWidgetType;
  typedef typename CurveWidgetType::Pointer    CurveWidgetPointerType;

  /** Rendering Function Type */
  typedef TRenderingFunction                       SRenderingFunctionType;
  typedef typename SRenderingFunctionType::Pointer SRenderingFunctionPointerType;
  
  /** */
  typedef VerticalAsymptoteCurve                   VerticalAsymptoteType;
  typedef typename VerticalAsymptoteType::Pointer  VerticalAsymptotePointerType;

  

   /** Handle Boundaries translation
   * \param widgetId The id of the resized widget
   * \param x new x location
   * \param y new y location
   */
virtual bool HandleWidgetEvent(std::string widgetId, int event)
  {
    // Variable declaration
    PointType             spacePoint, screenPoint;
    bool modifyLeft       = false;
    bool modifyRight      = false;
    
    // Left screen abcisse 
    spacePoint[0]     = m_LeftAsymptote->GetAbcisse();
    double  abcisseL  = m_Curve->TransformPoint(spacePoint)[0];
    
    // Right screen abcisse
    spacePoint[0]     = m_RightAsymptote->GetAbcisse();
    double  abcisseR  = m_Curve->TransformPoint(spacePoint)[0];
    
    if(widgetId == m_Curve->GetIdentifier())
      {
	switch (event)
	  {
	  case FL_PUSH:
	    {
	      // Position Clicked 
	      double x = Fl::event_x();
	      double y = Fl::event_y();
		
	      //std::cout <<"I clicked in position " << x << "," << y << std::endl;
	      //typename SRenderingFunctionType::ParametersType param = m_RenderingFunction->GetParameters();
		
	      if ((vcl_abs(x-abcisseL)<50) || (vcl_abs(x-abcisseR)<50))
		{
		  if (vcl_abs(x-abcisseL)<vcl_abs(x-abcisseR))
		    {
		      modifyLeft = true;
		      std::cout <<" Modify Left"<< std::endl;
		    }
		  else
		    {
		      modifyRight = true;
		      std::cout <<" Modify Right"<< std::endl;
		    }
		}
	      return true;
	    }
	  case FL_RELEASE:
	    {
	      modifyLeft  = false;
	      modifyRight = false;
	      return true;
	    }
	  case FL_DRAG:
	    {
	      //std::cout <<" space Point abcisseR " << abcisseR  << " abcisseL " << abcisseL << std::endl;
	      //std::cout <<"screen abcisseR " <<  m_RightAsymptote->GetAbcisse() << " abcisseL " << m_LeftAsymptote->GetAbcisse() << std::endl;
	      
	      
	      double x = Fl::event_x();
	      
	      if(modifyLeft)
		{
		  double tx = x - abcisseL;
		  m_LeftAsymptote->SetAbcisse(m_LeftAsymptote->GetAbcisse() + tx);
		  
		  //Update The Rendering Function min and max
		  //....
		}
		
	      if(modifyRight)
		{
		  double tx = x - abcisseR;
		  m_RightAsymptote->SetAbcisse(m_RightAsymptote->GetAbcisse() + tx); 
		  m_Curve->redraw();
		    
		  //Update The Rendering Function min and max
		  //....
		}
	      
	      return true;
	    }
	  }
      }
    return false;
  }

  /** Set/Get the pointer to the view */
  itkSetObjectMacro(View,ViewType);
  itkGetObjectMacro(View,ViewType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model,ModelType);
  itkGetObjectMacro(Model,ModelType);

  /** Set/Get the CurveWidget*/
  itkSetObjectMacro(Curve,CurveWidgetType);

  /** Get The left asymptote*/
  itkSetObjectMacro(LeftAsymptote,VerticalAsymptoteType);
  itkSetObjectMacro(RightAsymptote,VerticalAsymptoteType);
  
  
  /** Set/Get the rendering Function */
  itkSetObjectMacro(RenderingFunction , SRenderingFunctionType);

protected:
  /** Constructor */
  HistogramActionHandler() : m_View(), m_Model(), m_RenderingFunction()
  {}

  /** Destructor */
  virtual ~HistogramActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }

private:
  HistogramActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  // Curve Widget To Handle Events on
  CurveWidgetPointerType m_Curve; 

  // StandardRenderingFunction
  SRenderingFunctionPointerType  m_RenderingFunction;  

  // Left And Rigth Asymptote
  VerticalAsymptotePointerType  m_LeftAsymptote;
  VerticalAsymptotePointerType  m_RightAsymptote;
  

}; // end class
} // end namespace otb
#endif


