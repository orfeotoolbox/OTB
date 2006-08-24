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
#ifndef __otbImageViewerAS_h
#define __otbImageViewerAS_h

#include <string>

#include "itkShrinkImageFilter.h"

#include "otbPrincipalImageViewAS.h"
#include "otbImageViewerASGUI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbGLVectorImageViewClick.h"

namespace otb
{
/** \class ImageViewerAS
 * \brief Base class for viewing an image, by Rom's way
 *
 * This viewer show the image with one window display. 
 * The purpose is to select points in this image, display them 
 * and finally save them in a file
 * 
 * Use FLTK gui.
 *
*/

template <class TPixel>
class ITK_EXPORT ImageViewerAS :  public itk::ProcessObject,
                                  public otbImageViewerASGUI
                                
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef ImageViewerAS                 Self;
  typedef itk::ProcessObject          Superclass;
  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerAS,itk::ProcessObject);

  typedef TPixel                      InputPixelType;
 
  typedef TPixel                      InputOverlayType;

  typedef otb::MultiChannelExtractROI< InputPixelType, 
                                             InputPixelType >   ExtractROIFilterType;

  typedef typename ExtractROIFilterType::Pointer                ExtractROIFilterPointer;
 
  typedef PrincipalImageViewAS<InputPixelType,InputOverlayType> PrincipalImageViewType;
  typedef typename PrincipalImageViewType::Pointer              PrincipalImageViewPointer;
  
  typedef GLVectorImageViewClick<InputPixelType,InputOverlayType> ImageViewBaseType;

  typedef typename ExtractROIFilterType::InputImageType         ImageType;
//  typedef typename PrincipalImageViewType::ImageType         ImageType;
  typedef typename ImageType::Pointer                           ImagePointer;
  typedef typename ImageType::ConstPointer                      ImageConstPointer;
  typedef typename ImageType::RegionType                        RegionType;
  typedef typename ImageType::SizeType                          SizeType;
  typedef typename ImageType::IndexType                         IndexType;
  typedef typename ImageType::PixelType                         PixelType;

  
  ImageViewerAS();
  virtual ~ImageViewerAS();
  /** Set the left input image */
  virtual void SetLeftImage(itk::ImageBase<2> * img);
  /** Set the right input image */
  virtual void SetRightImage(itk::ImageBase<2> * img);
  /** Show the viewer (Update) */
  virtual void Show(void);
  /** Hide all Image View Windows */
  virtual void Hide(void);
  /** Update */
  virtual void Update(void);
  /** Gestion des événements associés aux bouttons */
  void EraseLastClickedPoint();
  void EraseAllClickedPoints();
  void LearnAlgorythm();
  void ClassificationAlgorythm();
  void DisplayFirstClass();
  void DisplaySecondClass();
  void LoadLeftImage();
  void LoadRightImage();
    
  /** Build the HMI */
  virtual void BuildHMI(void);
  virtual void CreatePrincipalImageViewBeforeClassif( Fl_Group * g , Fl_Gl_Window * w );
  virtual void CreatePrincipalImageViewAfterClassif( Fl_Group * g , Fl_Gl_Window * w );
//  virtual void SetLabel( const char * );
  virtual void ClickSelectCallBack(
                void (*newClickSelectArgCallBack)(float, float,
                                                  /*float,*/ float,
                                                  void *),
                     void * newClickSelectArg);
  virtual void PrincipalAction(const IndexType & index);
  //virtual void ExtractImagePrincipal(const RegionType & zone);

  virtual void GenerateLeftChannelsInformations(void);
  virtual void GenerateRightChannelsInformations(void);
  
  typedef typename ImageViewBaseType::ModeViewType ModeViewType; 
  typedef typename ImageViewBaseType::ChannelsType ChannelsType; 

  itkSetMacro(LeftGrayLevelChannel,int);
  itkSetMacro(LeftRedChannel,int);
  itkSetMacro(LeftGreenChannel,int);
  itkSetMacro(LeftBlueChannel,int);
  
  itkSetMacro(RightGrayLevelChannel,int);
  itkSetMacro(RightRedChannel,int);
  itkSetMacro(RightGreenChannel,int);
  itkSetMacro(RightBlueChannel,int);
  
  itkSetStringMacro(Label);
  itkGetStringMacro(Label);

  virtual void SetLeftRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        SetLeftRedChannel(pRedChannel);
        SetLeftGreenChannel(pGreenChannel);
        SetLeftBlueChannel(pBlueChannel);
  }
  virtual void SetRightRGBChannels(int pRedChannel,int pGreenChannel, int pBlueChannel)
  {
        SetRightRedChannel(pRedChannel);
        SetRightGreenChannel(pGreenChannel);
        SetRightBlueChannel(pBlueChannel);
  }  
  
  virtual void ClearSelectChannels(void);
  
  /** Synchronize clicked points on images before and after classif */
  void SynchronizeClickedPoints(float x,float y,float z,ColorType c);
  
  /** Synchronize clicked points Box on images before and after classif */
  void SynchronizeClickedPointsBox(float x,float y,float z,ColorType c,
  									int width, int height);

private:

        virtual void SetChannelsInformationsToImagesView(void);
  
        /** Label of the windows */
        std::string m_Label;
        static const std::string m_SuffixPrincipalTitle;
        
	/** Informations for left image */
	int  m_LeftNbDim;
        int  m_LeftRedChannel;
        int  m_LeftGreenChannel;
        int  m_LeftBlueChannel;
        int  m_LeftGrayLevelChannel;
        ModeViewType  m_LeftModeView;
        ChannelsType m_LeftChannelsWorks;
		
	/** Informations for right image */
	int  m_RightNbDim;
        int  m_RightRedChannel;
        int  m_RightGreenChannel;
        int  m_RightBlueChannel;
        int  m_RightGrayLevelChannel;
        ModeViewType  m_RightModeView;
        ChannelsType m_RightChannelsWorks;
		
        // PrincipalImageViewPointer       m_PrincipalView;
        PrincipalImageViewPointer       m_PrincipalViewBeforeClassif;
        PrincipalImageViewPointer       m_PrincipalViewAfterClassif;
  
        typedef unsigned long WinSizeType;

        /** Default size for the principal image */
        WinSizeType m_InitPrincipalWinSizeMax;
  
        /** Extract image filter use to calculte the Principal image view */
        //ExtractROIFilterPointer m_ExtractROIFilter;
        ImageType * m_InputLeftImage;
		ImageType * m_InputRightImage;
        ImageType * m_PrincipalLeftImage;
        ImageType * m_PrincipalRightImage;

};


} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageViewerAS.txx"
#endif


#endif
