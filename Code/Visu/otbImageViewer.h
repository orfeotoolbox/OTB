/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe de type Viewer, permettant de visualiser une image
  $Id$

=========================================================================*/
#ifndef __otbImageViewer_h
#define __otbImageViewer_h

#include <string>

#include "itkShrinkImageFilter.h"

#include "otbPrincipalImageView.h"
#include "otbScrollImageView.h"
#include "otbZoomImageView.h"
#include "otbImageViewerGUI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbGLVectorImageViewBase.h"

namespace otb
{

template <class TPixel, class TOverlayPixel>
class ImageViewer :             public itk::ProcessObject,
                                public otbImageViewerGUI
                                
{
public:

   /**
   * Standard "Self" typedef.
   */
    typedef ImageViewer                 Self;
    typedef itk::ProcessObject          Superclass;
  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(ImageViewer,itk::ProcessObject);

    typedef TPixel                      InputPixelType;
    typedef TOverlayPixel               OverlayPixelType;


  typedef otb::MultiChannelExtractROI< InputPixelType, 
                                             InputPixelType >   ExtractROIFilterType;

  typedef typename ExtractROIFilterType::Pointer                  ExtractROIFilterPointer;
//  typedef typename ExtractROIFilterType::InputImageType         InputImageType;
//  typedef typename InputImageType::Pointer                      InputImagePointer;
 
  typedef itk::Image< OverlayPixelType, 3 >                     OverlayType;
  typedef PrincipalImageView<InputPixelType, OverlayPixelType>  PrincipalImageViewType;
  typedef typename PrincipalImageViewType::Pointer              PrincipalImageViewPointer;
  typedef ScrollImageView< InputPixelType, OverlayPixelType >   ScrollImageViewType;
  typedef typename ScrollImageViewType::Pointer                 ScrollImageViewPointer;
  typedef ZoomImageView< InputPixelType, OverlayPixelType >     ZoomImageViewType;
  typedef typename ZoomImageViewType::Pointer                   ZoomImageViewPointer;

  typedef GLVectorImageViewBase<InputPixelType, OverlayPixelType>  ImageViewBaseType;

  typedef typename ExtractROIFilterType::InputImageType         ImageType;
  typedef typename ImageType::Pointer                           ImagePointer;
  typedef typename ImageType::ConstPointer                      ImageConstPointer;
  typedef typename ImageType::RegionType                        RegionType;
  typedef typename ImageType::SizeType                          SizeType;
  typedef typename ImageType::IndexType                         IndexType;
  typedef typename ImageType::PixelType                         PixelType;

  typedef itk::ShrinkImageFilter<ImageType,ImageType>           ShrinkImageFilterType;
  typedef typename ShrinkImageFilterType::Pointer               ShrinkImageFilterPointer;

    
  ImageViewer();
  virtual ~ImageViewer();
  virtual void SetImage(itk::ImageBase<2> * img);
  virtual void Show(void);
  virtual void Hide(void);
  virtual void Update(void);
  virtual void PrepareIHM(void);
  virtual void CreatePrincipalImageView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void CreateScrollImageView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void CreateZoomImageView( Fl_Group * g , Fl_Gl_Window * w );
  virtual void SetLabel( const char * );
  virtual void ClickSelectCallBack(
                void (*newClickSelectArgCallBack)(float, float,
                                                  /*float,*/ float,
                                                  void *),
                     void * newClickSelectArg);
  virtual void ZoomAction(void);
  virtual void PrincipalResize(void);
  virtual void ScrollResize(void);
  virtual void PrincipalAction(const IndexType & index);
  virtual void ScrollAction(const IndexType & index);
  virtual void ExtractImagePrincipal(const RegionType & zone);

  virtual void GenerateChannelsInformations(void);
  
  typedef typename std::vector<int> ChannelsType;
  
  typedef typename ImageViewBaseType::ModeViewType ModeViewType; 
  typedef typename ImageViewBaseType::ChannelsType ChannelsType; 

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
  virtual void ClearSelectChannels(void);

private:

        virtual void SetChannelsInformationsToImagesView(void);
        virtual bool GenereImageScroll(unsigned int & pShrinkFactors);

        int m_NbDim;

        int  m_RedChannel;
        int  m_GreenChannel;
        int  m_BlueChannel;
        int  m_GrayLevelChannel;

        ModeViewType  m_ModeView;
        ChannelsType m_ChannelsWorks;

        //  GLImageViewType * glSliceView;
        PrincipalImageViewPointer       m_PrincipalView;
        ScrollImageViewPointer          m_ScrollView;
        ZoomImageViewPointer            m_ZoomView;
 
        typedef unsigned long WinSizeType;

        //Dimension de la fenêtre maximale, au dela de laquelle on créer une fenêtre Scroll
//        WinSizeType m_ScrollWinSizeMax;
        //Dimension par défaut de la fenetre principal
        WinSizeType m_InitPrincipalWinSizeMax;
        //Initialisation de la dimension de la fenêtre dans le cas où la fenetre Scroll est créé.
        WinSizeType m_InitScrollWinSizeMax;




        ShrinkImageFilterPointer m_ShrinkImageFilter;
        ExtractROIFilterPointer m_ExtractROIFilter;
        ImageType * m_InputImage;
        
        ImageType * m_PrincipalImage;
        ImageType * m_ScrollImage;
        
        //Coordonnées du point central de l'image Principal
        IndexType m_CenterPrincipalImage;
        //Coordonnées du point central de l'image Zoom
        IndexType m_CenterZoomImage;
        
        bool m_ScrollImageView;
        unsigned int m_ShrinkFactors;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewer.txx"
#endif


#endif
