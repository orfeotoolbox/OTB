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

#ifndef __otbTileMapImageIOFactory_h
#define __otbTileMapImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "otbImageIOBase.h"

namespace otb
{
/** \class TileMapImageIOFactory
 * \brief Creation of a TileMapImageIO object using object factory.
 */
class ITK_EXPORT TileMapImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef TileMapImageIOFactory         Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(TileMapImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    TileMapImageIOFactory::Pointer TileMapFactory = TileMapImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(TileMapFactory);
  }

protected:
  TileMapImageIOFactory();
  virtual ~TileMapImageIOFactory();

private:
  TileMapImageIOFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
