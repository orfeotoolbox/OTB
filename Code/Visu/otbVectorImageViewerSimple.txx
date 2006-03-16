#ifndef _otbVectorImageViewerSimple_txx
#define _otbVectorImageViewerSimple_txx

#include "otbVectorImageViewerSimple.h"

namespace otb
{
  
template <class TPixel, class OverlayPixelType>
VectorImageViewerSimple<TPixel,OverlayPixelType>
::VectorImageViewerSimple()
{
//  glSliceView = 0;
  CreateGUI();
}


  
template <class TPixel, class OverlayPixelType>
VectorImageViewerSimple<TPixel,OverlayPixelType>
::~VectorImageViewerSimple()
{

}




template <class TPixel, class OverlayPixelType>
void
VectorImageViewerSimple<TPixel,OverlayPixelType>
::SetImage(itk::ImageBase<3> * img)
{
  ImageType * image = dynamic_cast<ImageType *>( img );
//  glSliceView->SetInputImage( image );
  glSliceView->SetInput( image );
  this->Modified();
  Synchronize();
}

template <class TPixel, class OverlayPixelType>
void 
VectorImageViewerSimple<TPixel,OverlayPixelType>
::Show(void)
{
  this->Update();
/*  static bool firstTime = true;
  iviewWindow->show();
  glSliceView->show();
  glSliceView->update();
  if( firstTime )
  {
    firstTime = false;
    Fl::check();
    glSliceView->redraw();
    Fl::check();
  }
 */
}





template <class TPixel, class OverlayPixelType>
void 
VectorImageViewerSimple<TPixel,OverlayPixelType>
::Hide(void)
{
  iviewWindow->hide();
}


template <class TPixel, class OverlayPixelType>
void 
VectorImageViewerSimple<TPixel,OverlayPixelType>
::Update(void)
{
/*  this->Superclass::Update();
  glSliceView->TreatInputImage();
  glSliceView->update();*/
  this->Superclass::Update();
  static bool firstTime = true;
  iviewWindow->show();
  glSliceView->TreatInputImage();
  glSliceView->show();
  glSliceView->update();
  if( firstTime )
  {
    firstTime = false;
    Fl::check();
    glSliceView->redraw();
    Fl::check();
  }
  
}

template <class TPixel, class OverlayPixelType>
void
VectorImageViewerSimple<TPixel,OverlayPixelType>
::Synchronize(void) 
{
  float iwDiff  = glSliceView->iwMax() - glSliceView->iwMin();
  float b       = (float)((int)log10(iwDiff)-2);
  double iwMin  = ((int)(glSliceView->iwMin()*pow((float)10, (float)-b)))/pow((float)10,(float)-b);
  double iwMax  = ((int)(glSliceView->iwMax()*pow((float)10, (float)-b)))/pow((float)10,(float)-b);
  double iwStep = (iwMax-iwMin)/100.0;
}

template <class TPixel, class OverlayPixelType>
void
VectorImageViewerSimple<TPixel,OverlayPixelType>
::SetLabel(const char * label)
{
  iviewWindow->label( label );
}

template <class TPixel, class OverlayPixelType>
void
VectorImageViewerSimple<TPixel,OverlayPixelType>
::CreateGLSliceView( Fl_Group * group, Fl_Gl_Window * original )
{

  short int x = original->x();
  short int y = original->y();
  short int w = original->w();
  short int h = original->h();

  const char * label = original->label();


//  this->glSliceView = new GLImageViewType(x,y,w,h,label);
  this->glSliceView = GLVectorImageViewType::New();
  this->glSliceView->Init(x,y,w,h,label);
  this->glSliceView->box( FL_EMBOSSED_BOX );

  group->remove( original );

  delete original;


}


template <class TPixel, class OverlayPixelType>
void 
VectorImageViewerSimple<TPixel,OverlayPixelType>
::ClickSelectCallBack( void (*newClickSelectArgCallBack)(float, float,
                                                         float, float,
                                                         void *),
                                                         void * newClickSelectArg)
{
  glSliceView->clickSelectCallBack( newClickSelectArgCallBack, 
                                    newClickSelectArg           ); 
}



} // end namespace otb
#endif

