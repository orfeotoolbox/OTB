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

#ifndef __otbSpatialisationFilter_h
#define __otbSpatialisationFilter_h

#include "itkLabelObject.h"
#include "otbLabelMapSource.h"
#include "itkSize.h"
#include "itkImageRegionIterator.h"
#include "otbImage.h"

namespace otb
{

/**
 * \class SpatialisationFilter
 * \brief Create a label map from objects description.
 *
 * Objects are described by:
 * - Vector of area: indicating the spectral type of the object (rock, grass, forest, ..).
 * - Vector of paths: indicating the path of files containing associated spectra or "" if spectrum will be simulated.
 *
 * SpatialisationFilter creates a labelMap containing NumberOfObjects[0]*NumberOfObject[1] objects (number of objects by line, number of objects by column).
 * Each object is rectangular of size ObjectSize.
 * Two attributes "path" and "area" are added to each object using SetPathVector and SetAreaVector.
 * The Label of each object is set using SetLabels.
 *
 * \sa LabelMapSource
 */

template <class TLabelMap>
class ITK_EXPORT SpatialisationFilter :
  public LabelMapSource<TLabelMap>

{
public:
   /**
   * Standard "Self" & Superclass typedef.
   */
   typedef SpatialisationFilter Self;
   typedef LabelMapSource<TLabelMap>     Superclass;
   typedef itk::SmartPointer<Self>       Pointer;
   typedef itk::SmartPointer<const Self> ConstPointer;

   /** Some convenient typedefs. */
   typedef TLabelMap                                 OutputLabelMapType;
   typedef typename OutputLabelMapType::Pointer      OutputLabelMapPointer;
   typedef typename OutputLabelMapType::ConstPointer OutputLabelMapConstPointer;
   typedef typename OutputLabelMapType::IndexType    IndexType;
   typedef typename OutputLabelMapType::RegionType   RegionType;
   typedef typename OutputLabelMapType::LabelObjectType::LabelType  LabelType;
   typedef typename OutputLabelMapType::LabelObjectType  LabelObjectType;
   typedef typename LabelObjectType::AttributesValueType  AttributesValueType;

   typedef itk::Size<2>              SizeType;
   typedef std::vector<AttributesValueType>  VectorType;
   typedef std::vector<LabelType>    LabelVectorType;


   /** Standard Macro*/
   itkTypeMacro(SpatialisationFilter, LabelMapSource);
   itkNewMacro(Self);

   /**Set input vector data*/
   itkSetMacro(ObjectSize, SizeType);
   itkGetMacro(ObjectSize, SizeType);

   itkSetMacro(NumberOfObjects, SizeType);
   itkGetMacro(NumberOfObjects, SizeType);

//    itkSetMacro(PathVector, VectorType);
//    itkGetMacro(PathVector, VectorType);
//
//    itkSetMacro(AreaVector, VectorType);
//    itkGetMacro(AreaVector, VectorType);
   void SetPathVector(VectorType & v)
   {
      m_PathVector=v;
   }

   void SetAreaVector(VectorType & v)
   {
      m_AreaVector=v;
   }

   void SetLabels(LabelVectorType & v)
   {
      m_Labels=v;
   }




protected:
   SpatialisationFilter();
   virtual ~SpatialisationFilter() {}
   void PrintSelf(std::ostream& os, itk::Indent indent) const;

   virtual void GenerateData();
   virtual void ProcessObject(unsigned int obj);
  virtual void GenerateOutputInformation();

private:
   SpatialisationFilter(const Self &); //purposely not implemented
   void operator =(const Self&); //purposely not implemented

   /** Common size for all objects in number of pixels (number of rows, number of columns).*/
   SizeType m_ObjectSize;
   /** Number of objects contained in the image (number of objects by line, number of objects by column).*/
   SizeType m_NumberOfObjects;
   /**Vector containing the path for spectra files (if any) for each object. */
   /** If no path is given (empty path: "") spectrum is simulated from default parameters.*/
   VectorType m_PathVector; //vector containing the spectrum path (if any) for each object
   /** Vector containing the spectral type of each object.*/
   VectorType m_AreaVector; //vector containing the the type of each object
   /** Vector containing the label of each object (ex.: 1, 2, 25, etc.).*/
   LabelVectorType m_Labels; //vector containing the label of each object.

};

} // end namespace itk

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpatialisationFilter.txx"
#endif

#endif
