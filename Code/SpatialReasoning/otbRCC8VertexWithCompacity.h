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
#ifndef _otbRCC8VertexWithCompacity_h
#define _otbRCC8VertexWithCompacity_h

#include "otbRCC8VertexBase.h"
#include "itkObjectFactory.h"

namespace otb
{
  /** \class RCC8VertexWithCompacity
   *  \brief Class to represent a vertex in a RCC8 Graph with the compacity value of
   *  the represented region.
   *  
   * \sa RCC8Graph, RCC8Edge, RCC8VertexBase
   */
  template <class TLabel, class TPrecision = float>
    class RCC8VertexWithCompacity : public RCC8VertexBase<TLabel>
    {
      public:
      /** Standard class typedefs */
      typedef RCC8VertexWithCompacity Self;
      typedef otb::RCC8VertexBase<TLabel>  Superclass;
      typedef itk::SmartPointer<Self>  Pointer;
      typedef itk::SmartPointer<const Self> ConstPointer;
      /** Method for creation through the object factory. */
      itkNewMacro(Self);
      /** Run-time type information (and related methods). */
      itkTypeMacro(RCC8VertexWithCompacity,RCC8VertexBase);
      /** Input image associated typedefs*/
      typedef TLabel LabelType;
      /** Precision  typedef */
      typedef TPrecision PrecisionType;
      /** char* vector attributes */
      typedef  typename Superclass::AttributesMapType  AttributesMapType;
 
      /** Segmentation image index accessors */
      itkGetMacro(Compacity,PrecisionType);
      itkSetMacro(Compacity,PrecisionType);
      /**
       * Set the VertexWithCompacity attributes from the attributes vector.
       * \param attributes The vector containing the parsed attributes.
       */
      void SetAttributesMap(AttributesMapType attributes);
      /**
       * Get an attributes vector representing the VertexWithCompacity attributes.
       * \return The attributes vector
       */
      AttributesMapType GetAttributesMap(void);

      protected:
      /** Constructor */
      RCC8VertexWithCompacity();
      /** Desctructor */
      ~RCC8VertexWithCompacity(){};
      /** PrintSelf method */
      void PrintSelf(std::ostream& os, itk::Indent indent) const;

      private:
      /** The compacity */
      PrecisionType m_Compacity;
    };
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbRCC8VertexWithCompacity.txx"
#endif

#endif
