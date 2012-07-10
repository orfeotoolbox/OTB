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
#include "itkNumericTraits.h"

namespace otb
{
/** \class HistogramActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*  \ingroup Visualization
 */

template <class TModel, class TView, class TRenderingFunction>
class ITK_EXPORT HistogramActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef HistogramActionHandler        Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(HistogramActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TView                                         ViewType;
  typedef typename ViewType::Pointer                    ViewPointerType;
  typedef typename ViewType::ImageWidgetType::PointType PointType;

  /** Curve Widget*/
  typedef Curves2DWidget                    CurveWidgetType;
  typedef typename CurveWidgetType::Pointer CurveWidgetPointerType;

  /** Rendering Function Type */
  typedef TRenderingFunction                             RenderingFunctionType;
  typedef typename RenderingFunctionType::Pointer        RenderingFunctionPointerType;
  typedef typename RenderingFunctionType::ParametersType ParametersType;

  /** */
  typedef VerticalAsymptoteCurve                  VerticalAsymptoteType;
  typedef typename VerticalAsymptoteType::Pointer VerticalAsymptotePointerType;

  /** Handle vertical asymptotes translation
   * \param widgetId The id of the handled Curve widget
   * \param event kind of event ot handle : FL_DRAG , FL_PUSH, FL_RELEASE
   */
  virtual bool HandleWidgetEvent(const std::string& widgetId, int event)
  {
    // Variable declaration
    PointType spacePoint;
    spacePoint.Fill(0);

    // Left screen abcisse
    spacePoint[0]     = m_LeftAsymptote->GetAbcisse();
    double abcisseL  = m_Curve->TransformPoint(spacePoint)[0];

    // Right screen abcisse
    spacePoint[0]     = m_RightAsymptote->GetAbcisse();
    double abcisseR  = m_Curve->TransformPoint(spacePoint)[0];

    if (widgetId == m_Curve->GetIdentifier() && this->GetIsActive())
      {
      switch (event)
        {
        case FL_PUSH:
          {
          // Position Clicked
          double x = Fl::event_x();

          if ((vcl_abs(x - abcisseL) < 50) || (vcl_abs(x - abcisseR) < 50))
            {
            if (vcl_abs(x - abcisseL) < vcl_abs(x - abcisseR))
              {
              m_ModifyLeft = true;
              }
            else
              {
              m_ModifyRight = true;
              }
            }
          return true;
          }
        case FL_RELEASE:
          {
          if (m_ModifyLeft || m_ModifyRight)
            {
            m_Model->Update();
            }

          m_ModifyLeft  = false;
          m_ModifyRight = false;
          return true;
          }
        case FL_DRAG:
          {
          double x = Fl::event_x();

          if (m_ModifyLeft)
            {
            double tx = x - abcisseL;
            double newLeftAbcisse = m_LeftAsymptote->GetAbcisse() + tx;
            if ( newLeftAbcisse < m_RightAsymptote->GetAbcisse()
                 && newLeftAbcisse >= m_MinimumAbcisse)
              {
              m_LeftAsymptote->SetAbcisse(newLeftAbcisse);
              m_Curve->redraw();

              //  Update The Rendering Function min and max
              ParametersType param = m_RenderingFunction->GetParameters();
              param.SetElement(2 * m_Channel, newLeftAbcisse);
              param.SetElement(2 * m_Channel + 1, m_RightAsymptote->GetAbcisse());
              m_RenderingFunction->SetParameters(param);
              }
            }

          if (m_ModifyRight)
            {
            double tx = x - abcisseR;
            double newRightAbcisse = m_RightAsymptote->GetAbcisse() + tx;
            if (newRightAbcisse > m_LeftAsymptote->GetAbcisse()
                &&  newRightAbcisse <  m_MaximumAbcisse)
              {
              m_RightAsymptote->SetAbcisse(newRightAbcisse);
              m_Curve->redraw();
              
              //  Update The Rendering Function min and max
              ParametersType param = m_RenderingFunction->GetParameters();
              param.SetElement(2 * m_Channel, m_LeftAsymptote->GetAbcisse());
              param.SetElement(2 * m_Channel + 1, newRightAbcisse);
              m_RenderingFunction->SetParameters(param);
              }
            }
          return true;
          }
        }
      }
    return false;
  }

  /** Set/Get the pointer to the view */
  itkSetObjectMacro(View, ViewType);
  itkGetObjectMacro(View, ViewType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);

  /** Set/Get the CurveWidget*/
  itkSetObjectMacro(Curve, CurveWidgetType);

  /** Get The left asymptote*/
  itkSetObjectMacro(LeftAsymptote, VerticalAsymptoteType);
  itkSetObjectMacro(RightAsymptote, VerticalAsymptoteType);

  /** Set/Get the rendering Function */
  itkSetObjectMacro(RenderingFunction, RenderingFunctionType);

  /** Set Extremum of the histogram (used as limit for the
    * asymptotes). If not set, values are initialized to double type extremum
    */
  itkSetMacro(MaximumAbcisse, double);
  itkSetMacro(MinimumAbcisse, double);

  /** Set/Get the channel dealed with in the image*/
  itkSetMacro(Channel, unsigned int);

protected:
  /** Constructor */
  HistogramActionHandler() : m_View(), m_Model(), m_RenderingFunction()
    {
    m_Channel = 0;
    m_ModifyLeft  = false;
    m_ModifyRight = false;
    m_MaximumAbcisse = itk::NumericTraits<double>::max();
    m_MinimumAbcisse = itk::NumericTraits<double>::NonpositiveMin();
    }

  /** Destructor */
  virtual ~HistogramActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  HistogramActionHandler(const Self &);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  // Curve Widget To Handle Events on
  CurveWidgetPointerType m_Curve;

  // StandardRenderingFunction
  RenderingFunctionPointerType m_RenderingFunction;

  // Left And Rigth Asymptote
  VerticalAsymptotePointerType m_LeftAsymptote;
  VerticalAsymptotePointerType m_RightAsymptote;

  // Flags
  bool m_ModifyLeft;
  bool m_ModifyRight;

  //Channel we're dealing handling
  unsigned int m_Channel;

  // extremum of the histogram used as limit for the abcisse
  double         m_MaximumAbcisse;
  double         m_MinimumAbcisse;

}; // end class
} // end namespace otb
#endif
