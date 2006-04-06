/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   4 avril 2005
  Version   :   
  Role      :   Classe de base, utilisee pour l'affichage d'une image dans une fenetre.
  $Id$

=========================================================================*/
#ifndef _otbVectorImageView_h
#define _otbVectorImageView_h

/*
* This is a cross-platform image viewer for 3D medical images.
* See the class definition for details...
*/

#include <itkProcessObject.h>
#include <itkVectorImage.h>
#include <list>
//#include <FL/fl_file_chooser.H>
#include <fstream>

namespace otb
{

/** \class VectorImageView
* \brief Base class for image view
*/
template <class TPixel>
class ITK_EXPORT VectorImageView : public itk::ProcessObject
{
  public:
    typedef VectorImageView                     Self;
    typedef itk::ProcessObject                  Superclass;
    typedef itk::SmartPointer<Self>             Pointer;
    typedef itk::SmartPointer<const Self>       ConstPointer;

    /** Method for creation through the object factory. */
    itkNewMacro(Self);

    /** Run-time type information (and related methods). */
    itkTypeMacro(VectorImageView,itk::ProcessObject);

    // some typedefs
    typedef itk::VectorImage<TPixel,2>          ImageType;
    typedef typename ImageType::Pointer         ImagePointer;
    typedef typename ImageType::ConstPointer    ImageConstPointer;
    typedef typename ImageType::RegionType      RegionType;
    typedef typename ImageType::SizeType        SizeType;
    typedef typename ImageType::IndexType       IndexType;
    typedef typename ImageType::PixelType       PixelType;

    itkSetMacro(ViewImageRegion, RegionType);
    itkGetConstReferenceMacro(ViewImageRegion, RegionType);

  protected:
    void   (* cSliceNumCallBack)(void);
    void    * cSliceNumArg;
    void   (* cSliceNumArgCallBack)(void * sliceNumArg);
    
    bool                     cValidImData;
    bool                     cViewImData;
    bool                     cViewClickedPoints;
    RegionType               m_ImageRegion;
    RegionType               m_ViewImageRegion;
    
    unsigned long            cDimSize[2];
    void                    (* cViewImDataCallBack)(void);
    void                     * cViewImDataArg;
    void                    (* cViewImDataArgCallBack)(void *viewImDataArg);
    
    float       cIWMin;
    float       cIWMax;

    void        (* cIWCallBack)(void);
    void         * cIWArg;
    void        (* cIWArgCallBack)(void * iwArg);

//    ImageModeType cImageMode;
    
    float               cWinZoom;
    unsigned int   cWinSizeX;
    unsigned int   cWinSizeY;
    int   cWinDataSizeX;
    int   cWinDataSizeY;
    unsigned char  *cWinImData;
    
    double cDataMax, cDataMin;
    
    int cX, cY, cW, cH;
    
    /*! VectorImageView Constructor */
    VectorImageView();
    /*! Standard destructor */
    virtual ~VectorImageView(void);

  public:

    virtual void Init(int x, int y, int w, int h, const char *l=0);
    
    /*! Return a pointer to the image data */
    
  virtual void SetInput( const ImageType *image);

//    const ImageConstPointer & GetInputImage(void) const;
    const ImageType * GetInput(void) const;
    
    /*! Dammit, give me a pointer to the image that's not const! */
    ImageType * GetInput(void);
    
    /*! Return a pointer to the pixels being displayed */
    unsigned char *         winImData(void);
    
    /*! Return the window x-dim */
    
    unsigned int    winDataSizeX(void);
    
    /*! Return the window y-dim */
    
    unsigned int    winDataSizeY(void);
    /*! Turn on/off the viewing of the image */
    void          viewImData(bool newViewImData);
    /*! Status of the image - viewed / not viewed */
    bool            viewImData(void);
    /*! Called when viewing of the image is toggled */
    void            viewImDataCallBack(
                          void (* newViewImDataCallBack)(void));
    void            viewImDataCallBack(
                          void (* newViewImDataArgCallBack)(void *),
                          void * viewImDataArg);
    
    /*! Return the minimum value for the image */
    double dataMin(void) const;

    /*! Return the maximum value for the image */
    double dataMax(void) const;

    
    /*! Specify a zoom factor */
    void    winZoom(float newWinZoom);
    /*! How much is the window zoomed */
    float   winZoom(void);
    

    virtual void        iwMin(float newIWMin);
    virtual float       iwMin(void);
    virtual void        iwMax(float newIWMax);
    virtual float       iwMax(void);
    void        iwCallBack(void (*newIWCallBack)(void));
    void        iwCallBack(void (*newIWArgCallBack)(void *),
                           void * newIWArg);
    
    virtual void size(int w, int h);
    virtual int sizeX(void){return cW;}
    virtual int sizeY(void){return cH;}
    virtual void resize(int x, int y, int w, int h);
    
    virtual void update(){};
    virtual void draw(){};
    
    virtual unsigned int WinSizeX(){return cWinSizeX;}
    virtual unsigned int WinSizeY(){return cWinSizeY;}

};

} //namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageView.txx"
#endif


#endif

