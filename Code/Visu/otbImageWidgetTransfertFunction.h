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
#ifndef _otbImageWidgetTransfertFunction_h
#define _otbImageWidgetTransfertFunction_h

#include "itkProcessObject.h"
#include "itkMacro.h"
#include "otbMath.h"

namespace otb
{
/** \class ImageWidgetTransfertFunction
 *  \brief Base class for visualisation transfert functions.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetTransfertFunction
  : public itk::Object
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetTransfertFunction      Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetTransfertFunction,Object);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;

  /** Set/Get the lower and upper bounds */
  itkSetMacro(LowerBound,PixelType);
  itkGetMacro(LowerBound,PixelType);
  itkSetMacro(UpperBound,PixelType);
  itkGetMacro(UpperBound,PixelType);

  /**
   * Map the input value to screen printable value.
   * Pure virtual method, should be implemented in subclasses.
   * \param value the value to map
   * \return the screen printable value
   */
  virtual unsigned char Map(PixelType value){};

protected:
  /** Constructor */
  ImageWidgetTransfertFunction()
    {
      m_LowerBound = 0;
      m_UpperBound = 255;
    }
  /** Destructor */
  virtual ~ImageWidgetTransfertFunction() {}
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
      os<<indent<<"Lower bound: "<<m_LowerBound<<std::endl;
      os<<indent<<"Upper bound: "<<m_UpperBound<<std::endl;
    }

private:
  ImageWidgetTransfertFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /// lower bound (set to 0)
  PixelType m_LowerBound;
  /// upper bound (set to 255)
  PixelType m_UpperBound;
};

/** \class ImageWidgetAffineTransfertFunction
 *  \brief Affine transfert function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetAffineTransfertFunction
  : public ImageWidgetTransfertFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetAffineTransfertFunction   Self;
  typedef ImageWidgetTransfertFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetAffineTransfertFunction,ImageWidgetTransfertFunction);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;

  virtual unsigned char Map(PixelType value)
    {
      if(value<this->GetLowerBound())
	{
	  return 0;
	}
      else if(value>this->GetUpperBound())
	{
	  return 255;
	}
      else
	{
	  return static_cast<unsigned char>(vcl_floor(255.*(value-this->GetLowerBound())/(this->GetUpperBound()-this->GetLowerBound())+0.5));
	}

    }
 protected:
  /** Constructor and  destructor */
  ImageWidgetAffineTransfertFunction(){}
  ~ImageWidgetAffineTransfertFunction(){}
 private:
  ImageWidgetAffineTransfertFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


/** \class ImageWidgetSquareRootTransfertFunction
 *  \brief SquareRoot transfert function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetSquareRootTransfertFunction
  : public ImageWidgetTransfertFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetSquareRootTransfertFunction Self;
  typedef ImageWidgetTransfertFunction<TPixel>   Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetSquareRootTransfertFunction,ImageWidgetTransfertFunction);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;

  virtual unsigned char Map(PixelType value)
    {
      if(value<this->GetLowerBound())
	{
	  return 0;
	}
      else if(value>this->GetUpperBound())
	{
	  return 255;
	}
      else
	{
	  double b = vcl_sqrt(static_cast<double>(this->GetLowerBound()));
	  double a = 255. /(vcl_sqrt(static_cast<double>(this->GetUpperBound()))-b);
	  return static_cast<unsigned char>(vcl_floor(a * (vcl_sqrt(static_cast<double>(value))-b)+0.5));
	}

    }
 protected:
  /** Constructor and  destructor */
  ImageWidgetSquareRootTransfertFunction(){}
  ~ImageWidgetSquareRootTransfertFunction(){}
 private:
  ImageWidgetSquareRootTransfertFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


/** \class ImageWidgetLogTransfertFunction
 *  \brief Log transfert function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetLogTransfertFunction
  : public ImageWidgetTransfertFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetLogTransfertFunction      Self;
  typedef ImageWidgetTransfertFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetLogTransfertFunction,ImageWidgetTransfertFunction);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;

  virtual unsigned char Map(PixelType value)
    {
      if(value<this->GetLowerBound())
	{
	  return 0;
	}
      else if(value>this->GetUpperBound())
	{
	  return 255;
	}
      else
	{
	  double b = vcl_log(1.+static_cast<double>(this->GetLowerBound()));
	  double a = 255. /(vcl_log(1.+static_cast<double>(this->GetUpperBound()))-b);
	  return static_cast<unsigned char>(vcl_floor(a * (vcl_log(1.+static_cast<double>(value))-b)+0.5));
	}

    }
 protected:
  /** Constructor and  destructor */
  ImageWidgetLogTransfertFunction(){}
  ~ImageWidgetLogTransfertFunction(){}
 private:
  ImageWidgetLogTransfertFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

/** \class ImageWidgetSquareTransfertFunction
 *  \brief Log transfert function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetSquareTransfertFunction
  : public ImageWidgetTransfertFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetSquareTransfertFunction      Self;
  typedef ImageWidgetTransfertFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;
  
  /** Type macro */
  itkNewMacro(Self);
  
  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetSquareTransfertFunction,ImageWidgetTransfertFunction);
  
  /** Template parameters typedefs */
  typedef TPixel PixelType;


  virtual unsigned char Map(PixelType value)
    {
      if(value<this->GetLowerBound())
	{
	  return 0;
	}
      else if(value>this->GetUpperBound())
	{
	  return 255;
	}
      else
	{
	  double b = vcl_pow(static_cast<double>(this->GetLowerBound()),2);
	  double a = 255. /(vcl_pow(static_cast<double>(this->GetUpperBound()),2)-b);
	  return static_cast<unsigned char>(vcl_floor(a * (vcl_pow(static_cast<double>(value),2)-b)+0.5));
	}

    }
 protected:
  /** Constructor and  destructor */
  ImageWidgetSquareTransfertFunction(){}
  ~ImageWidgetSquareTransfertFunction(){}
 private:
  ImageWidgetSquareTransfertFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb

#endif
