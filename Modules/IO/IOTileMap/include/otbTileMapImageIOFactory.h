/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbTileMapImageIOFactory_h
#define otbTileMapImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "OTBIOTileMapExport.h"

namespace otb
{
/** \class TileMapImageIOFactory
 * \deprecated
 * \brief Creation of a TileMapImageIO object using object factory.
 *
 * \ingroup OTBIOTileMap
 */
class OTBIOTileMap_DEPRECATED_EXPORT TileMapImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef TileMapImageIOFactory         Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static TileMapImageIOFactory * FactoryNew() { return new TileMapImageIOFactory; }

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
  ~TileMapImageIOFactory() override;

private:
  TileMapImageIOFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
