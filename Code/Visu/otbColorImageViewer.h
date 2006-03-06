#ifndef __otbColorImageViewer_h
#define __otbColorImageViewer_h

#include "otbGLColorImageView.h"
#include "otbImageViewerGUI.h"
#include "itkImage.h"
#include "itkRGBPixel.h"

namespace otb
{

template <class ImagePixelType, class OverlayPixelType>
class ColorImageViewer : public otbImageViewerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef ColorImageViewer         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

 
  typedef itk::Image< itk::RGBPixel<ImagePixelType>, 3 >   ImageType;
  typedef itk::Image< OverlayPixelType, 3 > OverlayType;
  typedef GLColorImageView< ImagePixelType, OverlayPixelType > GLColorImageViewType;
  typedef typename GLColorImageViewType::ColorTablePointer ColorTablePointer;


    
  ColorImageViewer();
  virtual ~ColorImageViewer();
  virtual void SetImage(itk::ImageBase<3> * img);
  virtual void SetOverlay(itk::ImageBase<3> * img);
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Update(void);
  virtual void Synchronize(void);
  virtual void ImageMode(ImageModeType mode);
  virtual void SelectSlice(unsigned int num);
  virtual void SetIntensityWindowingMin(float val);
  virtual void SetIntensityWindowingMax(float val);
  virtual void CenterWindow(void);
  virtual void ZoomIn(void);
  virtual void ZoomOut(void);
  virtual void ShiftUp(void);
  virtual void ShiftDown(void);
  virtual void ShiftLeft(void);
  virtual void ShiftRight(void);
  virtual void SetOrientation(void);
  virtual void CreateGLSliceView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void SetLabel( const char * );
  virtual void ShowClickedPointsWindow(void);
  virtual void UpdateClickedPoints(void);
  virtual void ClearClickedPoints(void);
  virtual void ClickSelectCallBack(
                void (*newClickSelectArgCallBack)(float, float,
                                                  float, float,
                                                  void *),
                     void * newClickSelectArg);

  
  virtual void ViewDetails(bool detail);
  virtual void ViewValue(bool value);
  virtual void ViewCrosshairs(bool crosshairs);

  /** Specify the opacity of the overlay */
  virtual void  SetOverlayOpacity(float newOverlayOpacity);
  
  /** Get the opacity of the overlay */
  virtual float GetOverlayOpacity(void) const;
  
  /** Show slider to control overlay opacity */
  virtual void ShowOverlayOpacityControl(void);
  
  /** Get the ColorTable for the Overlay */
  virtual ColorTablePointer GetOverlayColorTable(void);

  /** Set the ColorTable for the Overlay */
  virtual void SetOverlayColorTable(ColorTablePointer newColorTable);

  void SetOverlayColorIndex( unsigned int c )
    { 
    glSliceView->SetOverlayColorIndex( c );
    }


private:
  GLColorImageViewType * glSliceView;
};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include <otbColorImageViewer.txx>
#endif




#endif
