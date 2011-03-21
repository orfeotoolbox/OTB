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
#ifndef __otbElevDatabaseHeightAboveMSLFunction_h
#define __otbElevDatabaseHeightAboveMSLFunction_h

#include "itkFunctionBase.h"
#include "itkImageBase.h"
#include "itkPoint.h"
#include "itkIndex.h"
#include "itkContinuousIndex.h"


class ossimElevManager;

namespace otb
{

/** \class ElevDatabaseHeightAboveMSLFunction
 * \brief Evaluates a  the height above MSL(Mean Sea Level) of a geographic point
 *  of DTED and SRTM formats.
 *
 * \sa Point
 *
 * \ingroup ImageFunctions
 */
template <
class TOutputPixel,
class TCoordRep = float,
unsigned int NPointDimension=2
>
class ElevDatabaseHeightAboveMSLFunction :
    public itk::FunctionBase< itk::Point<TCoordRep, NPointDimension>,
                              TOutputPixel >
{
public:
    /** Dimension of the Space */
    itkStaticConstMacro(PointDimension, unsigned int, NPointDimension);

    /** Standard class typedefs. */
  typedef ElevDatabaseHeightAboveMSLFunction                    Self;
  typedef itk::FunctionBase<
      itk::Point<TCoordRep, NPointDimension>, TOutputPixel >    Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ElevDatabaseHeightAboveMSLFunction, itk::FunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Output Pixel typedef support */
  typedef TOutputPixel   PixelType;

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** Point Type. */
  typedef itk::Point<TCoordRep, NPointDimension>    PointType;

  /** Get the dimension (size) of the point. */
  static unsigned int GetPointDimension()
    { return NPointDimension; }


  /** Set the Elev Manager to the function. */
  virtual void SetElevManager(ossimElevManager * ptr )
    { m_ElevManager = ptr; }

  /** Get the input image. */
  const ossimElevManager * GetElevManager() const
    { return m_ElevManager; }

  /** Evaluate the function at specified Point position.
   * Subclasses must provide this method. */
  virtual PixelType Evaluate( const PointType& point) const;

  /** Try to open the DEM directory. */
  virtual void OpenDEMDirectory(const char* DEMDirectory);
  virtual void OpenDEMDirectory(const std::string&  DEMDirectory);

  /** Set the default height above ellipsoid in case no information is available*/
  virtual void SetDefaultHeightAboveEllipsoid(double h);

protected:
  ElevDatabaseHeightAboveMSLFunction();
  ~ElevDatabaseHeightAboveMSLFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ElevDatabaseHeightAboveMSLFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ossimElevManager* m_ElevManager;
};

}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbElevDatabaseHeightAboveMSLFunction.txx"
#endif

#endif
