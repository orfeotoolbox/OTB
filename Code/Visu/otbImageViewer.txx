
#include "otbImageViewer.h"


namespace otb
{
  
template <class ImagePixelType, class OverlayPixelType>
ImageViewer<ImagePixelType,OverlayPixelType>
::ImageViewer()
{
//  glSliceView = 0;
  CreateGUI();
}


  
template <class ImagePixelType, class OverlayPixelType>
ImageViewer<ImagePixelType,OverlayPixelType>
::~ImageViewer()
{

}




template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetImage(itk::ImageBase<3> * img)
{
  ImageType * image = dynamic_cast<ImageType *>( img );
  glSliceView->SetInputImage( image );
  Synchronize();
}


template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetOverlay(itk::ImageBase<3> * img)
{
  OverlayType * overlay = dynamic_cast<OverlayType *>( img );
  glSliceView->SetInputOverlay( overlay );
  Synchronize();
}

/* Specify the opacity of the overlay */
template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetOverlayOpacity(float newOverlayOpacity)
{
  glSliceView->OverlayOpacity( newOverlayOpacity );
  glSliceView->update();
}


/* Get the opacity of the overlay */
template <class ImagePixelType, class OverlayPixelType>
float
ImageViewer<ImagePixelType,OverlayPixelType>
::GetOverlayOpacity(void) const
{
  return glSliceView->OverlayOpacity();
}


/* Get the ColorTable for the Overlay */
template <class ImagePixelType, class OverlayPixelType>
typename ImageViewer<ImagePixelType, OverlayPixelType>::ColorTablePointer
ImageViewer<ImagePixelType,OverlayPixelType>
::GetOverlayColorTable(void)
{
  return glSliceView->GetColorTable();
}

/* Set the ColorTable for the Overlay */
template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetOverlayColorTable(ColorTablePointer newColorTable)
{
  glSliceView->SetColorTable(newColorTable);
}



template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::Show(void)
{
  static bool firstTime = true;
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
}





template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::Hide(void)
{
  iviewWindow->hide();
  clickedPointsWindow->hide();
  overlayOpacityControlWindow->hide();
}






template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::Update(void)
{
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ImageMode(ImageModeType mode)
{
  glSliceView->imageMode(mode);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::Synchronize(void) 
{
  float iwDiff  = glSliceView->iwMax() - glSliceView->iwMin();
  float b       = (float)((int)log10(iwDiff)-2);
  double iwMin  = ((int)(glSliceView->iwMin()*pow((float)10, (float)-b)))/pow((float)10,(float)-b);
  double iwMax  = ((int)(glSliceView->iwMax()*pow((float)10, (float)-b)))/pow((float)10,(float)-b);
  double iwStep = (iwMax-iwMin)/100.0;
  sliceNumberSlider->range( 0.0f, glSliceView->numSlices() );
  intensityWindowingMinSlider->range(iwMin-iwStep,iwMax+iwStep);
  intensityWindowingMaxSlider->range(iwMin-iwStep,iwMax+iwStep);
  sliceNumberSlider->value((float)glSliceView->sliceNum());
  intensityWindowingMinSlider->step(iwStep);
  intensityWindowingMaxSlider->step(iwStep);
  intensityWindowingMinSlider->value(iwMin);
  intensityWindowingMaxSlider->value(iwMax);
}







template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::SelectSlice(unsigned int num)
{
  sliceNumberSlider->value(num);
  glSliceView->sliceNum((int)sliceNumberSlider->value());
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::SetIntensityWindowingMin(float val)
{
  intensityWindowingMinSlider->value(val);
  glSliceView->iwMin(intensityWindowingMinSlider->value());  
  glSliceView->update();
}








template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::SetIntensityWindowingMax(float val)
{
  intensityWindowingMaxSlider->value(val);
  glSliceView->iwMax(intensityWindowingMaxSlider->value());  
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::CenterWindow(void)
{
  glSliceView->winCenter();
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ZoomIn(void)
{
  glSliceView->winZoom(glSliceView->winZoom()*2.0f);
 glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ZoomOut(void)
{
  glSliceView->winZoom(glSliceView->winZoom()/2.0f);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ShiftUp(void)
{
  glSliceView->winShift(1,0);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ShiftDown(void)
{
  glSliceView->winShift(-1,0);
  glSliceView->update();
}








template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ShiftLeft(void)
{
  glSliceView->winShift(0,-1);
  glSliceView->update();
}







template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::ShiftRight(void)
{
  glSliceView->winShift(0,1);
  glSliceView->update();
}






template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetLabel(const char * label)
{
  iviewWindow->label( label );
}





template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::SetOrientation(void)
{
  glSliceView->orientation( orientationChoice->value() );
  glSliceView->update();
  Synchronize();
}









template <class ImagePixelType, class OverlayPixelType>
void
ImageViewer<ImagePixelType,OverlayPixelType>
::CreateGLSliceView( Fl_Group * group, Fl_Gl_Window * original )
{

  short int x = original->x();
  short int y = original->y();
  short int w = original->w();
  short int h = original->h();

  const char * label = original->label();


//  this->glSliceView = new GLImageViewType(x,y,w,h,label);
  this->glSliceView = GLImageViewType::New();
  this->glSliceView->Init(x,y,w,h,label);

  glSliceView->box( FL_EMBOSSED_BOX );

  group->remove( original );

  delete original;


}







template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ShowClickedPointsWindow(void)
{
  clickedPointsWindow->show();
}





template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ClearClickedPoints(void)
{
  glSliceView->clearClickedPointsStored();
  clickedPointsBrowser->clear();
}




template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::UpdateClickedPoints(void)
{
  clickedPointsBrowser->clear();
  char buffer[200];
  const int numPoints = glSliceView->numClickedPointsStored();

  for(int i=numPoints-1; i>=0; i--)
    {
    ClickPoint point;
    if( glSliceView->getClickedPoint(i,point) )
      {
      const int x = (int)(point.x);
      const int y = (int)(point.y);
      const int z = (int)(point.z);
      const float value = point.value;
      sprintf(buffer,"%3d %3d %3d %f",x,y,z,value);
      clickedPointsBrowser->add( buffer );
      }
    }

}



template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ClickSelectCallBack( void (*newClickSelectArgCallBack)(float, float,
                                                         float, float,
                                                         void *),
                                                         void * newClickSelectArg)
{
  glSliceView->clickSelectCallBack( newClickSelectArgCallBack, 
                                    newClickSelectArg           ); 
}


template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ViewDetails(bool detail)
{
  glSliceView->viewDetails(detail);
}
  
  
template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ViewValue(bool value)
{
  glSliceView->viewValue(value);
}
  
template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ViewCrosshairs(bool crosshairs)
{
  glSliceView->viewCrosshairs(crosshairs);
}




  
template <class ImagePixelType, class OverlayPixelType>
void 
ImageViewer<ImagePixelType,OverlayPixelType>
::ShowOverlayOpacityControl()
{
  overlayOpacitySlider->value( this->GetOverlayOpacity() );
  overlayOpacityControlWindow->show();
}



} // end namespace otb
