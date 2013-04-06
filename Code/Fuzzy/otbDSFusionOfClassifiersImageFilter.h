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
#ifndef __otbDSFusionOfClassifiersImageFilter_h
#define __otbDSFusionOfClassifiersImageFilter_h

//#include "itkInPlaceImageFilter.h"
#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "itkVariableSizeMatrix.h"
#include "otbConfusionMatrixToMassOfBelief.h"

namespace otb
{
/** \class DSFusionOfClassifiersImageFilter
 *  \brief Performs Dempster-Shafer combination of n masses function.
 *
 *  This filter computes the joint mass of n input masses using
 *  Dempster-Shafer rule of combination.
 *
 *
 *  \sa MassOfBelief
 * \ingroup Streamed
 * \ingroup Threaded
 */
template <class TInputImage, class TOutputImage, class TMaskImage = TOutputImage>
class ITK_EXPORT DSFusionOfClassifiersImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef DSFusionOfClassifiersImageFilter                    Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(DSFusionOfClassifiersImageFilter, ImageToImageFilter);

  // TInputImage is expected to be a vector image
  typedef TInputImage                                 InputImageType;
  typedef typename InputImageType::ConstPointer       InputImageConstPointerType;
  typedef typename InputImageType::InternalPixelType  ValueType;
  typedef typename InputImageType::PixelType          PixelType;

  // TMaskImage is expected to be a mono band image
  typedef TMaskImage                                  MaskImageType;
  typedef typename MaskImageType::ConstPointer        MaskImageConstPointerType;
  typedef typename MaskImageType::Pointer             MaskImagePointerType;

  // TOutputImage is expected to be a mono band image
  typedef TOutputImage                                OutputImageType;
  typedef typename OutputImageType::Pointer           OutputImagePointerType;
  typedef typename OutputImageType::RegionType        OutputImageRegionType;
  typedef typename OutputImageType::PixelType         LabelType;


  typedef itk::VariableSizeMatrix<double>                                           ConfusionMatrixType;
  typedef typename otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType>          ConfusionMatrixToMassOfBeliefType;
  typedef typename ConfusionMatrixToMassOfBeliefType::Pointer                       ConfusionMatrixToMassOfBeliefPointerType;
  typedef typename ConfusionMatrixToMassOfBeliefType::MapOfIndicesType              MapOfIndicesType;
  typedef typename ConfusionMatrixToMassOfBeliefType::SingleClassLabelMassMapType   SingleClassLabelMassMapType;
  typedef typename ConfusionMatrixToMassOfBeliefType::MassType                      MassType; //double by default
  typedef typename ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod  MassOfBeliefDefinitionMethod;

  typedef typename std::map<LabelType, unsigned int>                                ClassifierHistogramType;
  typedef typename std::vector<ConfusionMatrixType>                                 VectorOfConfusionMatricesType;
  typedef typename std::vector<MapOfIndicesType>                                    VectorOfMapOfIndicesType;
  typedef typename std::vector<SingleClassLabelMassMapType>                         VectorOfMapOfMassesOfBeliefType;


  /** Set/Get the m_LabelForNoDataPixels */
  itkSetMacro(LabelForNoDataPixels, LabelType);
  itkGetMacro(LabelForNoDataPixels, LabelType);

  /** Set/Get the LabelForUndecidedPixels */
  itkSetMacro(LabelForUndecidedPixels, LabelType);
  itkGetMacro(LabelForUndecidedPixels, LabelType);

  // Possible values for this->m_DefinitionMethod:
  // ConfusionMatrixToMassOfBeliefType::PRECISION, RECALL, ACCURACY and KAPPA
  itkSetMacro(DefinitionMethod, MassOfBeliefDefinitionMethod);

  /**
   * If set, only pixels within the mask will be classified.
   * \param mask The input mask.
   */
  void SetInputMask(const MaskImageType * mask);

  void SetInputMapsOfIndices(const VectorOfMapOfIndicesType vectorOfMapOfIndices);
  void SetInputConfusionMatrices(const VectorOfConfusionMatricesType vectorOfConfusionMatrices);

  /**
   * Get the input mask.
   * \return The mask.
   */
  const MaskImageType * GetInputMask(void);

  const VectorOfMapOfIndicesType GetInputMapsOfIndices(void);
  const VectorOfConfusionMatricesType GetInputConfusionMatrices(void);

  const LabelType OptimizedDSMassCombination(PixelType vectorPixelValue);


protected:
  /** Constructor */
  DSFusionOfClassifiersImageFilter();
  /** Destructor */
  virtual ~DSFusionOfClassifiersImageFilter() {}

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId);
  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  DSFusionOfClassifiersImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  unsigned int                              m_NumberOfClassifiers;
  ClassifierHistogramType                   m_Universe;
  unsigned int                              m_NumberOfClassesInUniverse;

  VectorOfMapOfIndicesType                  m_VectorOfMapOfIndices;
  VectorOfConfusionMatricesType             m_VectorOfConfMatrices;

  ConfusionMatrixToMassOfBeliefPointerType  m_ConfusionMatrixToMassOfBeliefFilter;
  MassOfBeliefDefinitionMethod              m_DefinitionMethod;

  VectorOfMapOfMassesOfBeliefType           m_VectorOfMapMOBs;
  std::vector<MassType>                     m_VectorOfUniverseMOBs;

  /** No Data label for invalid pixels (when using a mask) */
  LabelType                                 m_LabelForNoDataPixels;
  /** Undecided label for pixels with NOT unique DS voting */
  LabelType                                 m_LabelForUndecidedPixels;

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDSFusionOfClassifiersImageFilter.txx"
#endif

#endif
