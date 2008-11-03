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
#ifndef __otbImageWidgetBase_h
#define __otbImageWidgetBase_h

#include <FL/gl.h>
#include "FL/Fl_Gl_Window.H"
#include "otbVectorImage.h"
#include "itkObject.h"
#include "otbList.h"
#include "otbImageWidgetFormBase.h"
#include "otbImageWidgetTransferFunction.h"

namespace otb
{
  /** \class ImageWidgetBase
   *  \brief 
   *
   */
template <class TPixel>
class ImageWidgetBase
  : public Fl_Gl_Window, public itk::Object
  {
  public:
    /** Standard class typedefs */
    typedef ImageWidgetBase Self;
    typedef itk::Object Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    
    /** Method for creation through the object factory */
    itkNewMacro(Self);
    
    /** Runtime information */
    itkTypeMacro(ImageWidgetBase,Object);

    /** Template related typedef */
    typedef TPixel     PixelType;
    typedef otb::VectorImage<PixelType,2> ImageType;
    typedef typename ImageType::PixelType VectorPixelType;
    typedef typename ImageType::Pointer ImagePointerType;
    typedef typename ImageType::SizeType SizeType;
    typedef typename ImageType::IndexType IndexType;
    typedef typename ImageType::RegionType RegionType;

    /// TODO: the type of this image should be templated
    typedef otb::VectorImage<unsigned char,2> OverlayImageType;
    typedef typename OverlayImageType::Pointer OverlayImagePointerType;

    /** View model */
    enum ViewModelType{GRAYSCALE,COMPLEX_MODULUS,COMPLEX_PHASE,RGB};

    /** Form overlay typedef */
    typedef ImageWidgetFormBase FormType;
    typedef FormType::Pointer FormPointerType;
    typedef List<FormType> FormListType;
    typedef typename FormListType::Pointer FormListPointerType;
    typedef typename FormListType::ReverseIterator ReverseIteratorType;
    typedef typename FormListType::Iterator IteratorType;

    typedef otb::ImageWidgetTransferFunction<PixelType> TransferFunctionType;
    typedef otb::ImageWidgetAffineTransferFunction<PixelType> AffineTransferFunctionType;
    typedef List<TransferFunctionType> TransferFunctionListType;
    typedef typename TransferFunctionListType::Pointer TransferFunctionListPointerType;
    
    itkSetMacro(BufferedRegion,RegionType);
    itkGetMacro(BufferedRegion,RegionType);

    itkSetMacro(ViewedRegion,RegionType);
    itkGetMacro(ViewedRegion,RegionType);

    itkSetMacro(RedChannelIndex,unsigned int);
    itkGetMacro(RedChannelIndex,unsigned int);

    itkSetMacro(GreenChannelIndex,unsigned int);
    itkGetMacro(GreenChannelIndex,unsigned int);

    itkSetMacro(BlueChannelIndex,unsigned int);
    itkGetMacro(BlueChannelIndex,unsigned int);

    itkSetMacro(FormOverlayVisible,bool);
    itkGetMacro(FormOverlayVisible,bool);
    
    itkSetMacro(ImageOverlayVisible,bool);
    itkGetMacro(ImageOverlayVisible,bool);
    
    itkSetMacro(BlackTransparency,bool);
    itkGetMacro(BlackTransparency,bool);

    itkGetMacro(SubSamplingRate, unsigned int);
    itkSetMacro(SubSamplingRate, unsigned int);

    itkSetMacro(ViewModel,ViewModelType);
    itkGetMacro(ViewModel,ViewModelType);
    
    itkGetMacro(ImageOverlayOpacity,unsigned char);
    itkSetMacro(ImageOverlayOpacity,unsigned char);

    itkGetObjectMacro(FormList,FormListType);
    itkGetObjectMacro(TransferFunctionList,TransferFunctionListType);

    itkGetMacro(OpenGlIsotropicZoom,double);
    
    /** Set the input image.
     * \param image The image to view.
     */
    void SetInput(ImageType* image);
    /** Get the input image.
     * \return The image to view.
     */
    ImageType * GetInput(void);
    
    /** Set the input overlay image.
     * \param image The image to view.
     */
    void SetInputOverlay(OverlayImageType* image);
    /** Get the input overlay image.
     * \return The image to view.
     */
    OverlayImageType * GetInputOverlay(void);

    /** Set the input overlay form list.
     * \param formList The form list to view.
     */
    void SetFormListOverlay(FormListType* formList);

    /** Set the transfer function list
     *  \param list The transfer function list.
     */
    void SetTransferFunctionList(TransferFunctionListType * list);

    /** Show The widget */
    void Show(void);
     /** Reset the widget (Clear buffered region and re init the transfer functions) */
    void Reset(void);
    /** Clear the buffered region */
    void ClearBufferedRegion(void);

    /**
     * Convert window to image coordinate.
     * \param index The window coordinate.
     * \return The image coordinate.
     */
    virtual IndexType WindowToImageCoordinates(IndexType index);
        
    /** Initialize the widget */
    virtual void Init(int x, int y, int w, int h, const char * l);
    
  protected:
    /** Constructor */
    ImageWidgetBase();
    /** Destructor */
    ~ImageWidgetBase();

    /** Draw the widget */
    virtual void draw(void);
    /** Rebuild opengl buffer */
    virtual void RebuildOpenGlBuffer(void);
    /** Rebuild opengl image overlay buffer */
    virtual void RebuildOpenGlImageOverlayBuffer(void);
   
    /** Normalization function */
    unsigned char Normalize(PixelType value, unsigned int channelIndex);

   /** Return the width and heigh of the displayed image (overloaded for scroll)*/
   inline virtual int wDisplayed()
   {
    return this->w();
   }

   inline virtual int hDisplayed()
   {
    return this->h();
   }

    // PURE VIRTUAL METHODS 

    // User is not supposed to be allowed to move the zoom in the generic implementation
    itkSetMacro(OpenGlIsotropicZoom,double);
    

    /** Unlarge OpenGlBuffer */
    virtual void UpdateOpenGlBufferedRegion(void){};
    /** Test if the buffer has to be enlarged */
    virtual bool UpdateOpenGlBufferedRegionRequested(void){return 1;};

    /** Resize the widget */
    virtual void resize(int x, int y, int w, int h){};

    // END PURE VIRTUAL METHODS
  private:
     ImageWidgetBase(const Self&); // purposely not implemented
     void operator=(const Self&); // purposely not implemented

     /** Pointer to the viewed image */
     ImagePointerType m_Image;
     /** OpenGl Buffer */
     unsigned char * m_OpenGlBuffer;
     /** OpenGl Zoom factor */
     double m_OpenGlIsotropicZoom;
     /** Buffered image region */
     RegionType m_BufferedRegion;
     /** Viewed image region */
     RegionType m_ViewedRegion;
     /** The view model */
     ViewModelType m_ViewModel;
     /** Red channel index */
     unsigned int m_RedChannelIndex;
     /** Green channel index */
     unsigned int m_GreenChannelIndex;
     /** Blue channel index */
     unsigned int m_BlueChannelIndex;
     /** Display the form overlay */
     bool m_FormOverlayVisible;
     /** The form list form the overlay */
     FormListPointerType m_FormList;
     /** Display the image overlay */
     bool m_ImageOverlayVisible;
     /** Set black to transparent*/
     bool m_BlackTransparency;
     /** The image Overlay opacity */
     unsigned char  m_ImageOverlayOpacity;
     /** Pointer to the overlay image */
     OverlayImagePointerType m_ImageOverlay;
     /** OpenGl image overlay buffer */
     unsigned char * m_OpenGlImageOverlayBuffer;
     /** Max value for normalization */
     VectorPixelType m_MaxComponentValues;
     /** Min value for normalization */
     VectorPixelType m_MinComponentValues;
     /** Subsampling rate in case of subsampled input image.
      *  Default is 1.
      */
     unsigned int m_SubSamplingRate;
     /** List of the transfer function by channel */
     TransferFunctionListPointerType m_TransferFunctionList;
  };
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageWidgetBase.txx"
#endif

#endif
