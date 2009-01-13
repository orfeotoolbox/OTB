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
#ifndef __otbImageWidgetTransferFunction_h
#define __otbImageWidgetTransferFunction_h

#include "itkProcessObject.h"
#include "itkMacro.h"
#include "otbMath.h"

namespace otb
{
/** \class ImageWidgetTransferFunction
 *  \brief Base class for visualisation transfer functions.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetTransferFunction
  : public itk::Object
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetTransferFunction      Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetTransferFunction,Object);

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
  virtual unsigned char Map(PixelType value){return 0;};

protected:
  /** Constructor */
  ImageWidgetTransferFunction()
    {
      m_LowerBound = 0;
      m_UpperBound = 255;
    }
  /** Destructor */
  virtual ~ImageWidgetTransferFunction() {}
 /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
      os<<indent<<"Lower bound: "<<m_LowerBound<<std::endl;
      os<<indent<<"Upper bound: "<<m_UpperBound<<std::endl;
    }

private:
  ImageWidgetTransferFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /// lower bound (set to 0)
  PixelType m_LowerBound;
  /// upper bound (set to 255)
  PixelType m_UpperBound;
};

/** \class ImageWidgetAffineTransferFunction
 *  \brief Affine transfer function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetAffineTransferFunction
  : public ImageWidgetTransferFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetAffineTransferFunction   Self;
  typedef ImageWidgetTransferFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetAffineTransferFunction,ImageWidgetTransferFunction);

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
  ImageWidgetAffineTransferFunction(){}
  ~ImageWidgetAffineTransferFunction(){}
 private:
  ImageWidgetAffineTransferFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


/** \class ImageWidgetSquareRootTransferFunction
 *  \brief SquareRoot transfer function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetSquareRootTransferFunction
  : public ImageWidgetTransferFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetSquareRootTransferFunction Self;
  typedef ImageWidgetTransferFunction<TPixel>   Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetSquareRootTransferFunction,ImageWidgetTransferFunction);

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
  ImageWidgetSquareRootTransferFunction(){}
  ~ImageWidgetSquareRootTransferFunction(){}
 private:
  ImageWidgetSquareRootTransferFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


/** \class ImageWidgetLogTransferFunction
 *  \brief Log transfer function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetLogTransferFunction
  : public ImageWidgetTransferFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetLogTransferFunction      Self;
  typedef ImageWidgetTransferFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetLogTransferFunction,ImageWidgetTransferFunction);

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
  ImageWidgetLogTransferFunction(){}
  ~ImageWidgetLogTransferFunction(){}
 private:
  ImageWidgetLogTransferFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

/** \class ImageWidgetSquareTransferFunction
 *  \brief Log transfer function.
 */
template <class TPixel>
class ITK_EXPORT ImageWidgetSquareTransferFunction
  : public ImageWidgetTransferFunction<TPixel>
{
 public:
  /** Standard typedefs */
  typedef ImageWidgetSquareTransferFunction      Self;
  typedef ImageWidgetTransferFunction<TPixel> Superclass;
  typedef itk::SmartPointer<Self>              Pointer;
  typedef itk::SmartPointer<const Self>        ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ImageWidgetSquareTransferFunction,ImageWidgetTransferFunction);

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
  ImageWidgetSquareTransferFunction(){}
  ~ImageWidgetSquareTransferFunction(){}
 private:
  ImageWidgetSquareTransferFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
}// End namespace otb

#endif
