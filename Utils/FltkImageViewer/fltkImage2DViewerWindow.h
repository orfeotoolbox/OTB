/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkImage2DViewerWindow.h,v $
  Language:  C++
  Date:      $Date: 2002/08/22 17:34:37 $
  Version:   $Revision: 1.10 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef fltk_Image2D_Viewer_h
#define fltk_Image2D_Viewer_h

#include <fltkGlWindow.h>
#include <itkRGBPixel.h>


namespace fltk {


class Image2DViewerWindow : public GlWindow 
{

public:

  typedef     unsigned char    ValueType;

  typedef itk::RGBPixel<float>  ColorType;

  class SelectionBox {
  public:
    int X1 ;
    int X2 ;
    int Y1 ;
    int Y2 ;
  public:
    SelectionBox():X1(0),X2(0),Y1(0),Y2(0) {};
    ~SelectionBox() {};
  } ;

  typedef SelectionBox SelectionBoxType ;

  Image2DViewerWindow(int x,int y,int w,int h, const char * label=0);
  virtual ~Image2DViewerWindow();

  virtual void draw(void);  // overload fltk methods
  virtual int  handle(int);
  virtual void  handlePopUpMenu( void );
  
  void SetBackground(GLfloat r, GLfloat g, GLfloat b);
  void SetBackground(const ColorType & color);
  void Allocate( unsigned int nx, unsigned int ny );
  void Update( void );
  
  const ColorType & GetBackground(void) const;

  ValueType * GetBuffer(void) 
  { return m_Buffer; }

  unsigned int GetNumberOfBytesPerPixel(void) const
  { return m_NumberOfBytesPerPixel; }

  unsigned int GetWidth(void) const;
  unsigned int GetHeight(void) const;
  void SetWidth(unsigned int);
  void SetHeight(unsigned int);
  void FitWindowToImage(void);
  void FitImageToWindow(void);
  void IntensityWindowing(void);
  void SetIntensityWindow( Fl_Window * window );
  void SetParentWindow( Fl_Window * window );
  void PanningEventHandling(int & p1x, int & p1y);
  void SelectEventHandling(int & p1x, int & p1y);
  void ClickEventHandling(int & p1x, int & p1y);
  void ZoomingEventHandling(int & p1x, int & p1y);
  void SetSelectionBox(SelectionBoxType* box) ;
  void SetSelectionCallBack(void* ptrObject, 
                            void (*selectionCallBack)
                            (void* ptrObject, 
                             SelectionBoxType* box)) ;
  void SetClickCallBack(void* ptrObject, 
                            void (*clickCallBack)
                            (void* ptrObject, 
                             int & x, int & y )) ;


  void (*clickCallBack)( void *ptrObject, int & px, int & py );


  typedef enum { 
      PanningMode, 
      ZoomingMode, 
      SelectMode,
      ClickMode
      } 
      InteractionModeType;

  void SetInteractionMode( InteractionModeType mode );

private:

  ColorType          m_Background;
  ValueType       *  m_Buffer;
  int                m_Width;
  int                m_Height;
  double             m_Zoom;
  int                m_ShiftX;
  int                m_ShiftY;
  Fl_Window       *  m_IntensityWindow;
  Fl_Window       *  m_ParentWindow;
  unsigned int       m_NumberOfBytesPerPixel;

  InteractionModeType m_InteractionMode;

  SelectionBoxType m_Box ;
  void* m_SelectionCallBackTargetObject ;
  void* m_ClickCallBackTargetObject ;
  void (*m_SelectionCallBack)(void* ptrObject, SelectionBoxType* box) ;
  void (*m_ClickCallBack)(void* ptrObject, int & px, int & py ) ;
};


} // end namespace fltk

#endif   
