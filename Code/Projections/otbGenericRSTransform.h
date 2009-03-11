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
#ifndef __otbGenericRSTransform_h
#define __otbGenericRSTransform_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "projection/ossimProjection.h"
#include "itkTransform.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
// #include "base/ossimGpt.h"
// #include "base/ossimDpt.h"
// #include "projection/ossimProjection.h"
// #include "base/ossimEllipsoid.h"
// #include "base/ossimEllipsoidFactory.h"
// #include "base/ossimString.h"
// #include "ossimOgcWktTranslator.h"
#include "otbCompositeTransform.h"

namespace otb
{

  /** \class GenericRSTransform
   *  \brief This is the class to handle generic remote sensing transform
   *
   * The transformation will transform the coordinate from any map projection or sensor
   * model to any map projection or sensor model.
   *
   *
   * \ingroup Projection
   *
   **/

  template <class TScalarType = double,
    unsigned int NInputDimensions=2,
    unsigned int NOutputDimensions=2>
    class ITK_EXPORT GenericRSTransform: public itk::Transform<TScalarType,       // Data type for scalars
    NInputDimensions,  // Number of dimensions in the input space
    NOutputDimensions> // Number of dimensions in the output space
    {
      public :
      /** Standard class typedefs. */
      typedef itk::Transform< TScalarType,
      NInputDimensions,
      NOutputDimensions >       Superclass;
      typedef GenericRSTransform                      Self;
      typedef itk::SmartPointer<Self>                   Pointer;
      typedef itk::SmartPointer<const Self>             ConstPointer;

      typedef typename Superclass::ScalarType           ScalarType;
      typedef ossimProjection                           OssimMapProjectionType;
      typedef itk::Point<ScalarType,NInputDimensions >  InputPointType;
      typedef itk::Point<ScalarType,NOutputDimensions > OutputPointType;

      typedef itk::Vector<double, 2> SpacingType;
      typedef itk::Point<double, 2> OriginType;

      typedef itk::Transform<double, 2, 2> GenericTransformType;
      typedef typename GenericTransformType::Pointer GenericTransformPointerType;
      typedef otb::CompositeTransform<GenericTransformType, GenericTransformType> TransformType;
      typedef typename TransformType::Pointer                                     TransformPointerType;

//       typedef itk::MetaDataDictionary MetaDataDictionary;
//       typedef itk::SmartPointer<MetaDataDictionary> MetaDataDictionaryPointer;
//       typedef itk::SmartPointer<const MetaDataDictionary> MetaDataDictionaryConstPointer;
      /** Method for creation through the object factory. */
      itkNewMacro( Self );

      /** Run-time type information (and related methods). */
      itkTypeMacro( GenericRSTransform, itk::Transform );

      typedef InverseOrForwardTransformationEnum DirectionOfMappingEnumType;

      itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
      itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
      itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
      itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));

      /** Set/Get for input and output projections.  */
      itkSetStringMacro(InputProjectionRef);
      itkGetStringMacro(InputProjectionRef);

      itkSetStringMacro(OutputProjectionRef);
      itkGetStringMacro(OutputProjectionRef);

      itkSetStringMacro(DEMDirectory);
      itkGetStringMacro(DEMDirectory);

      /** Set/Get Dictionary*/

//       itkSetConstObjectMacro(InputDictionary, MetaDataDictionary);
//       itkGetConstObjectMacro(InputDictionary, MetaDataDictionary);
//       itkSetObjectMacro(OutputDictionary, MetaDataDictionary);
//       itkGetObjectMacro(OutputDictionary, MetaDataDictionary);

      const itk::MetaDataDictionary& GetInputDictionary() const
      {
        return m_InputDictionary;
      }

      void SetInputDictionary(const itk::MetaDataDictionary & dictionary)
      {
        m_InputDictionary = dictionary;
        this->Modified();
      }

      const itk::MetaDataDictionary& GetOutputDictionary() const
      {
        return m_OutputDictionary;
      }

      void SetOutputDictionary(itk::MetaDataDictionary & dictionary)
      {
        m_OutputDictionary = dictionary;
        this->Modified();
      }



      itkGetMacro(InputKeywordList,ImageKeywordlist);
      void SetInputKeywordList(const ImageKeywordlist& kwl)
      {
        this->m_InputKeywordList = kwl;
        this->Modified();
      }

      itkGetMacro(OutputKeywordList,ImageKeywordlist);
      void SetOutputKeywordList(const ImageKeywordlist& kwl)
      {
        this->m_OutputKeywordList = kwl;
        this->Modified();
      }

      /** Set the origin of the vector data.
      * \sa GetOrigin() */
      itkSetMacro(InputOrigin, OriginType);
      virtual void SetInputOrigin( const double origin[2] );
      virtual void SetInputOrigin( const float origin[2] );

      itkGetConstReferenceMacro(InputOrigin, OriginType);


      /** Set the spacing (size of a pixel) of the vector data.
        * \sa GetSpacing() */
      virtual void SetInputSpacing (const SpacingType & spacing);
      virtual void SetInputSpacing (const double spacing[2]);
      virtual void SetInputSpacing (const float spacing[2]);

      itkGetConstReferenceMacro(InputSpacing, SpacingType);


      /** Set the origin of the vector data.
      * \sa GetOrigin() */
      itkSetMacro(OutputOrigin, OriginType);
      virtual void SetOutputOrigin( const double origin[2] );
      virtual void SetOutputOrigin( const float origin[2] );

      itkGetConstReferenceMacro(OutputOrigin, OriginType);


      /** Set the spacing (size of a pixel) of the vector data.
      * \sa GetSpacing() */
      virtual void SetOutputSpacing (const SpacingType & spacing);
      virtual void SetOutputSpacing (const double spacing[2]);
      virtual void SetOutputSpacing (const float spacing[2]);

      itkGetConstReferenceMacro(OutputSpacing, SpacingType);

      /** Methods prototypes */
      virtual const TransformType * GetTransform() const;

      OutputPointType TransformPoint(const InputPointType &point) const;

      virtual void  InstanciateTransform();


      protected:
      GenericRSTransform();
      virtual ~GenericRSTransform()
      {};

      virtual void Modified()
      {
        this->Superclass::Modified();
        m_TransformUpToDate = false;
      }

      private :
      GenericRSTransform(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

      ImageKeywordlist m_InputKeywordList;
      ImageKeywordlist m_OutputKeywordList;

      itk::MetaDataDictionary  m_InputDictionary;
      itk::MetaDataDictionary  m_OutputDictionary;

      std::string m_InputProjectionRef;
      std::string m_OutputProjectionRef;
      std::string m_DEMDirectory;

      SpacingType         m_InputSpacing;
      OriginType          m_InputOrigin;
      SpacingType         m_OutputSpacing;
      OriginType          m_OutputOrigin;

      TransformPointerType    m_Transform;

      GenericTransformPointerType m_InputTransform;
      GenericTransformPointerType m_OutputTransform;
      bool                        m_TransformUpToDate;
    };



} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSTransform.txx"
#endif

#endif
