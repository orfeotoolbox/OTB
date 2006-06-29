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
#ifndef otbGLVectorImageViewBase_h
#define otbGLVectorImageViewBase_h

#include <FL/gl.h>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/Fl_Double_Window.H>

#include <vector>
#include <string>

#include "itkImage.h"
#include "itkMacro.h"

#include "otbVectorImageView.h"

namespace otb
{


template <class TPixel> class ImageViewer;
/** \class GLVectorImageViewBase
* \brief Base class for image view
* Contain all algorithms and functionnalities for display an image in a window.
*/
template <class TPixel>
class ITK_EXPORT GLVectorImageViewBase :        public VectorImageView<TPixel>, 
                                                public Fl_Gl_Window
{
public:
    typedef GLVectorImageViewBase               Self;
    typedef VectorImageView<TPixel>             Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(GLVectorImageViewBase,VectorImageView);

  typedef ImageViewer<TPixel>                           ImageViewerType;
  typedef ImageViewerType *                             ImageViewerPointer;


  typedef typename Superclass::ImageType                ImageType;
  typedef typename Superclass::ImagePointer             ImagePointer;
  typedef typename Superclass::ImageConstPointer        ImageConstPointer;
  typedef typename Superclass::RegionType               RegionType;
  typedef typename Superclass::SizeType                 SizeType;
  typedef typename Superclass::IndexType                IndexType;
  typedef typename Superclass::PixelType                PixelType;

  /** Typedef Liste des canaux */
  typedef typename std::vector<int>                     ChannelsType;
  
  typedef enum { GRAY_LEVEL = 1, RGB_LEVEL = 3 }        ModeViewType;

  virtual void ClearSelectChannels(void);

  //Méthode d'accès sur le Viewer
  itkSetMacro(Viewer,ImageViewerPointer);
  itkGetConstMacro(Viewer,ImageViewerPointer);

  itkSetMacro(NbDim,int);
  itkSetMacro(ModeView,ModeViewType);

  itkSetMacro(GrayLevelChannel,int);
  itkSetMacro(RedChannel,int);
  itkSetMacro(GreenChannel,int);
  itkSetMacro(BlueChannel,int);

  virtual void SetRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        SetRedChannel(pRedChannel);
        SetGreenChannel(pGreenChannel);
        SetBlueChannel(pBlueChannel);
  }

//  itkSetMacro(ChannelsWorks, ChannelsType);
  virtual void SetChannelsWorks(const ChannelsType & pChannelsWorks)
  {
        m_ChannelsWorks = pChannelsWorks;
  }

  virtual IndexType     GetCenterRegion( const RegionType & zone );
  static  RegionType    ShrinkRegion( const RegionType & zone, const float shrinkfactor );
  virtual RegionType    GetViewRegion( const IndexType & centerPointImage );
  virtual RegionType    GetViewRegion( const RegionType & region, const IndexType & centerPointImage );
  virtual RegionType    TranslateImageRegionToWindowRegion( const RegionType & imageRegion, const  RegionType & sousimageRegion, const int windowWidth, const int windowHeight);

  /** Update the view region, with the new zoom and new Window sizes */
  virtual void          MajViewRegion(void);
  virtual void          PrintInfos(void);
  virtual void          BuildWithImageRegion();
  virtual void          BuildWithWindowRegion(const int zoom);
  
  virtual IndexType  WindowCoord2ImageCoord( const IndexType & index )const;
  virtual SizeType   WindowSize2ImageSize( const SizeType & size )const;
  virtual RegionType WindowZone2ImageRegion( const RegionType & zone )const;

  virtual void Init(int x, int y, int w, int h, const char *l);

  virtual void FinalizeInitialisation(void);
  
  virtual void size(int w, int h);
  virtual void resize(int x, int y, int w, int h);
  
  virtual void update();
  virtual void draw();
//  virtual int  handle(int event);
  virtual void SetDoubleWindow( Fl_Double_Window * pflDoubleWindow)
  {
        m_flDoubleWindow = pflDoubleWindow;
  }

  itkSetMacro(CenterPointImage,IndexType);
  itkGetConstReferenceMacro(CenterPointImage,IndexType);
  itkSetMacro(DrawViewRectangle,RegionType);
  itkGetConstReferenceMacro(DrawViewRectangle,RegionType);

  itkSetMacro(Label,std::string);
//  itkGetConstReferenceMacro(Label,std::string);
  itkSetStringMacro(Label);
  itkGetStringMacro(Label);

protected:

  //Méthode Get en écriture sur le Viewer
  itkGetMacro(Viewer,ImageViewerPointer);


  virtual void CalculeDataMinMax(const RegionType & region, double & pMin, double & pMax);
  virtual void SetWinImData(const RegionType & zone);
  
/*! FLTK required constructor - must use imData() to complete 
  definition */
  GLVectorImageViewBase();
  /*! Standard destructor */
  virtual ~GLVectorImageViewBase(void);

  /** Liste des canaux qui seront réellement traités [1...] */
  ChannelsType          m_ChannelsWorks;
  
  /** Nombre de dimensions de l'image : = 1 si GRAY_LEVEL, sinon 3 si RGB_LEVEL */
  int                   m_NbDim;
  int                   m_RedChannel;
  int                   m_GreenChannel;
  int                   m_BlueChannel;
  int                   m_GrayLevelChannel;

  bool                  m_ViewRectangle;
  RegionType            m_DrawViewRectangle;
  Fl_Double_Window *    m_flDoubleWindow;
  IndexType             m_CenterPointImage;

  ModeViewType          m_ModeView;


private:
    // Pointeur sur le Viewer
    ImageViewerPointer  m_Viewer;

    std::string         m_Label;   
};


} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGLVectorImageViewBase.txx"
#endif


#endif

