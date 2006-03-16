#ifndef __otbVectorImageViewerSimple_h
#define __otbVectorImageViewerSimple_h

#include "otbGLVectorImageView.h"
#include "otbVectorImageViewerGUI.h"

namespace otb
{

template <class TPixel, class OverlayPixelType>
class VectorImageViewerSimple :       public itk::ProcessObject,
                                public otbVectorImageViewerGUI
                                
{
public:

   /**
   * Standard "Self" typedef.
   */
    typedef VectorImageViewerSimple         Self;
    typedef itk::ProcessObject       Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorImageViewerSimple,itk::ProcessObject);

 
  typedef itk::Image< OverlayPixelType, 3 >                     OverlayType;
  typedef GLVectorImageView< TPixel, OverlayPixelType >         GLVectorImageViewType;
  typedef typename GLVectorImageViewType::Pointer               GLVectorImageViewPointer;
  typedef typename GLVectorImageViewType::ColorTablePointer     ColorTablePointer;
  typedef typename GLVectorImageViewType::ImageType             ImageType;


    
  VectorImageViewerSimple();
  virtual ~VectorImageViewerSimple();
  virtual void SetImage(itk::ImageBase<3> * img);
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Update(void);
  virtual void Synchronize(void);
  virtual void CreateGLSliceView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void SetLabel( const char * );
  virtual void ClickSelectCallBack(
                void (*newClickSelectArgCallBack)(float, float,
                                                  float, float,
                                                  void *),
                     void * newClickSelectArg);
  void SetGrayLevelChannel(int pChannel)
  {
        glSliceView->SetGrayLevelChannel(pChannel);
  }
  void SetRedChannel(int pChannel)
  {
        glSliceView->SetRedChannel(pChannel);
  }
  void SetGreenChannel(int pChannel)
  {
        glSliceView->SetGreenChannel(pChannel);
  }
  void SetBlueChannel(int pChannel)
  {
        glSliceView->SetBlueChannel(pChannel);
  }
  void SetRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        glSliceView->SetRGBChannels(pRedChannel,pGreenChannel,pBlueChannel);
  }

private:
//  GLImageViewType * glSliceView;
 GLVectorImageViewPointer glSliceView;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageViewerSimple.txx"
#endif


#endif
