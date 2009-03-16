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
#ifndef __otbBlendingFunction_h
#define __otbBlendingFunction_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
namespace Function
{
/**\class BlendingFunction
 * \brief Base class for blending functions.
 *  Please note that this class is pure virtual, and should be
 *  subclassed.
 */
template <class TInputRGBPixel1, class TInputRGBPixel2 = TInputRGBPixel1, class TOutputRGBPixel = TInputRGBPixel1>
class BlendingFunction
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef BlendingFunction              Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** type macro */
  itkTypeMacro(BlendingFunction,Object);

  /** PixelType macros */
  typedef TInputRGBPixel1              InputPixel1Type;
  typedef TInputRGBPixel2              InputPixel2Type;
  typedef TOutputRGBPixel              OutputRGBPixelType;

  /** Evaluate method  */
  virtual const OutputRGBPixelType Evaluate(const InputPixel1Type& input1, const InputPixel2Type & input2) = 0;
    
protected:
  /** Constructor */
  BlendingFunction()  {}
  /** Destructor */
  virtual ~BlendingFunction() {}

private:
  BlendingFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace Function
} // end namepsace otb

#endif


