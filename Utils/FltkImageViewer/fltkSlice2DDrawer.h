/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSlice2DDrawer.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:06 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __fltkSlice2DDrawer_h
#define __fltkSlice2DDrawer_h

#include <fltkSlice2DDrawerGUI.h>
#include <itkImage.h>
#include "itkCommand.h"
#include <FL/gl.h>



namespace fltk {


template <class TImage>
class Slice2DDrawer : public fltkSlice2DDrawerGUI
{
public:

   /**
   * Standard "Self" typedef.
   */
  typedef Slice2DDrawer<TImage>       Self;

 
  /**
   *  Type of the image to draw slices from
   */
  typedef  TImage   ImageType;
 

  /**
   * Standard "Superclass" typedef.
   */
  typedef fltkSlice2DDrawerGUI  Superclass;


  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;


  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( Slice2DDrawer, fltkSlice2DDrawerGUI ); 


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
   * Show
   */
  void Show(void);


  /**
   * Hide
   */
  void Hide(void);


  /**
   * Set Input Image
   */
  void SetInput( ImageType * );


  /**
   * Set Enabled
   */
  void SetEnabled( bool enabled );


  /**
   * Draw the target in Open Gl
   */
   void glDraw(void) const;


  /**
   * Bind texture for Slice 
   */
   void BindTexture(void) const;

  /**
   * Get the Observer/Command that will redraw the object
   */
  DrawCommandPointer GetDrawCommand(void);


protected:

  /**
   * Default constructor
   */
   Slice2DDrawer();


  /**
   * Destructor
   */
  ~Slice2DDrawer();



private:

   typename ImageType::Pointer      m_Image;
   
   int                    m_Nx;
   int                    m_Ny;
   
   int                    m_Nxr;
   int                    m_Nyr;
   
   float                  m_Dx;
   float                  m_Dy;

   float                  m_Cx;
   float                  m_Cy;
   
   float                  m_Ox;  // Origin in World Coordinates
   float                  m_Oy;
   
   unsigned char        * m_Slice;

   unsigned char        * m_Texture;
   mutable GLuint         m_TextureName;

   PixelType              m_Max_Value;

   mutable bool           m_TexturesGenerated;

   bool                   m_Enabled;

  DrawCommandPointer      m_DrawCommand;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSlice2DDrawer.txx>
#endif




#endif




