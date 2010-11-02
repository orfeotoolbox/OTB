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
class TOutput,
class TCoordRep = float
>
class ElevDatabaseHeightAboveMSLFunction :
    public itk::FunctionBase< itk::Point<TCoordRep,
                               ::itk::GetImageDimension<TOutput>::ImageDimension>,
                       typename TOutput::PixelType >
{
public:
    /** Dimension underlying input image. */
    itkStaticConstMacro(ImageDimension, unsigned int,
                        TOutput::ImageDimension);


    /** Standard class typedefs. */
  typedef ElevDatabaseHeightAboveMSLFunction                            Self;
  typedef itk::FunctionBase<
    itk::Point<TCoordRep, itkGetStaticConstMacro(ImageDimension)>,
    TOutput >                                                   Superclass;
  typedef itk::SmartPointer<Self>                               Pointer;
  typedef itk::SmartPointer<const Self>                         ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(ElevDatabaseHeightAboveMSLFunction, itk::FunctionBase);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** OutputType typedef support. */
  typedef TOutput OutputImageType;

  /** InputPixel typedef support */
  typedef typename OutputImageType::PixelType PixelType;

  /** InputImagePointer typedef support */
  typedef typename OutputImageType::ConstPointer ImageConstPointer;

  /** CoordRepType typedef support. */
  typedef TCoordRep CoordRepType;

  /** Index Type. */
  typedef typename OutputImageType::IndexType      IndexType;
  typedef typename OutputImageType::IndexValueType IndexValueType;

  /** Point Type. */
  typedef itk::Point<TCoordRep,itkGetStaticConstMacro(ImageDimension)> PointType;

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

  /** Set the default height above ellipsoid in case no information is available*/
  virtual void SetDefaultHeightAboveEllipsoid(double h);

  /** Set/Get the DEM Function. */
  itkSetMacro(DefaultUnknownValue, PixelType);
  itkGetConstMacro(DefaultUnknownValue, PixelType);

protected:
  ElevDatabaseHeightAboveMSLFunction();
  ~ElevDatabaseHeightAboveMSLFunction() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ElevDatabaseHeightAboveMSLFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ossimElevManager* m_ElevManager;
  PixelType         m_DefaultUnknownValue;
};

}// end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
# include "otbElevDatabaseHeightAboveMSLFunction.txx"
#endif

#endif
