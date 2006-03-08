/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSlice3DDrawer.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:07 $
  Version:   $Revision: 1.11 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __fltkSlice3DDrawer_h
#define __fltkSlice3DDrawer_h

#include <fltkSlice3DDrawerGUI.h>
#include <itkImage.h>
#include "itkCommand.h"
#include <FL/gl.h>



namespace fltk {


template <class TImage>
class Slice3DDrawer : public fltkSlice3DDrawerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef Slice3DDrawer<TImage>       Self;

 
  /**
   *  Type of the image to draw slices from
   */
  typedef  TImage   ImageType;
 

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltkSlice3DDrawerGUI  Superclass;


  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(Slice3DDrawer, fltkSlice3DDrawerGUI);


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  


  /**
   *  Pixel type from the image
   */
  typedef  typename ImageType::PixelType  PixelType;
 

  /**
   *  Region Type from the image
   */
  typedef  typename ImageType::RegionType  RegionType;
 

  /**
   *  Index Type from the image
   */
  typedef  typename ImageType::IndexType  IndexType;
 
  
  /**
   *  Size Type from the image
   */
  typedef  typename ImageType::SizeType  SizeType;
 

  /**
   * Command that will draw the object
   */
  typedef itk::SimpleConstMemberCommand< Self >   DrawCommandType;
  typedef typename DrawCommandType::Pointer       DrawCommandPointer;


  /**
   * Set Input Image
   */
  void SetInput( ImageType * );

  /**
   * Update GUI with image data
   */
  void RefreshGUI( void );

  /**
   * Show
   */
  void Show(void);


  /**
   * Hide
   */
  void Hide(void);


  /**
   * Select slice X
   */
   void SelectSliceX(void);


   /**
   * Select slice Y
   */
   void SelectSliceY(void);


   /**
   * Select slice Z
   */
   void SelectSliceZ(void);


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void) const;


  /**
   * Bind texture for Slice X
   */
   void BindTextureX(void) const;

   
  /**
   * Bind texture for Slice Y
   */
   void BindTextureY(void) const;


  /**
   * Bind texture for Slice Z
   */
   void BindTextureZ(void) const;

  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  /**
   * Default constructor
   */
   Slice3DDrawer();


  /**
   * Destructor
   */
  ~Slice3DDrawer();



private:

   typename ImageType::Pointer      m_Image;
   
   int                    m_Nx;
   int                    m_Ny;
   int                    m_Nz;
   
   int                    m_Nxr;
   int                    m_Nyr;
   int                    m_Nzr;
   
   float                  m_Dx;
   float                  m_Dy;
   float                  m_Dz;

   float                  m_Cx;
   float                  m_Cy;
   float                  m_Cz;
   
   float                  m_Ox;  // Origin in World Coordinates
   float                  m_Oy;
   float                  m_Oz;
   
   unsigned char        * m_SliceX;
   unsigned char        * m_SliceY;
   unsigned char        * m_SliceZ;

   unsigned char        * m_TextureX;
   unsigned char        * m_TextureY;  
   unsigned char        * m_TextureZ;
   mutable GLuint         m_TextureName[3];

   mutable bool           m_TextureBindedX;
   mutable bool           m_TextureBindedY;
   mutable bool           m_TextureBindedZ;

   PixelType              m_Max_Value;

   mutable bool           m_TexturesGenerated;

  DrawCommandPointer      m_DrawCommand;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSlice3DDrawer.txx>
#endif




#endif




