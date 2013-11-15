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
#ifndef __otbPolygonListToRCC8GraphFilter_h
#define __otbPolygonListToRCC8GraphFilter_h

#include "otbRCC8GraphSource.h"
#include "otbPolygonToPolygonRCC8Calculator.h"
#include "otbRCC8VertexIterator.h"
#include "otbRCC8InEdgeIterator.h"
#include "otbRCC8OutEdgeIterator.h"

namespace otb
{
/**
 * \class PolygonListToRCC8GraphFilter
 * \brief This class computes an RCC8 graph from a set of polygons.
 * \sa ImageMultiSegmentationToRCC8GraphFilter
 */
template <class TPolygonList, class TOutputGraph>
class ITK_EXPORT PolygonListToRCC8GraphFilter
  : public RCC8GraphSource<TOutputGraph>
{
public:
  /** Standard class typedefs. */
  typedef PolygonListToRCC8GraphFilter  Self;
  typedef RCC8GraphSource<TOutputGraph> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  /** Run-time type information (and related methods). */
  itkTypeMacro(PolygonListToRCC8GraphFilter, RCC8GraphSource);

  /** Input related typedef */
  typedef TPolygonList                           PolygonListType;
  typedef typename PolygonListType::ConstPointer PolygonListConstPointerType;
  typedef typename PolygonListType::ObjectType   PolygonType;
  typedef typename PolygonType::Pointer          PolygonPointerType;

  /** Output related typedefs */
  typedef TOutputGraph                                   OutputGraphType;
  typedef typename OutputGraphType::Pointer              OutputGraphPointerType;
  typedef typename OutputGraphType::VertexType           VertexType;
  typedef typename VertexType::Pointer                   VertexPointerType;
  typedef typename VertexType::PathType                  PathType;
  typedef typename OutputGraphType::VertexDescriptorType VertexDescriptorType;

  /** Knowledge enum typedef */
  typedef typename OutputGraphType::RCC8ValueType      RCC8ValueType;
  typedef enum {NO_INFO, LEVEL_1, LEVEL_3, FULL}       KnowledgeValueType;
  typedef std::pair<KnowledgeValueType, RCC8ValueType> KnowledgeStateType;

  /** RCC8 calculator typedef */
  typedef PolygonToPolygonRCC8Calculator<PolygonType> RCC8CalculatorType;

  /** Graph iterators typedefs */
  typedef RCC8VertexIterator<OutputGraphType>  VertexIteratorType;
  typedef RCC8InEdgeIterator<OutputGraphType>  InEdgeIteratorType;
  typedef RCC8OutEdgeIterator<OutputGraphType> OutEdgeIteratorType;

  typedef std::pair<unsigned int, unsigned int> EdgePairType;
  typedef std::map<EdgePairType, RCC8ValueType> EdgeMapType;
  typedef std::vector<EdgeMapType>              EdgeMapVectorType;

  typedef std::vector<unsigned int> SegmentationRangesType;

  /** Toogle optimisation flag */
  itkBooleanMacro(Optimisation);
  itkSetMacro(Optimisation, bool);
  itkBooleanMacro(UseInverted);
  itkSetMacro(UseInverted, bool);

  void SetSegmentationRanges(SegmentationRangesType ranges)
  {
    m_SegmentationRanges = ranges;
  }

  const SegmentationRangesType& GetSegmentationRanges() const
  {
    return m_SegmentationRanges;
  }

  /**
   * Get the number of occurences of the given value
   * \return The value.
   */
  unsigned int GetRelationsCount(RCC8ValueType val);

  /**
   * Get number of relations
   * \return The number of relations.
   */
  unsigned int GetNumberOfRelations(void);

  /** Set the input of the filter */
  virtual void SetInput(const PolygonListType *input);
  /** Get the input of the filter */
  const PolygonListType * GetInput(void);

protected:
  /** Constructor */
  PolygonListToRCC8GraphFilter();
  /** Destructor */
  virtual ~PolygonListToRCC8GraphFilter();
  /** Main computation method */
  virtual void GenerateData();

  /** Multi-threading implementation */

  virtual void BeforeThreadedGenerateData();

  virtual void AfterThreadedGenerateData();

  /** startIndex and stopIndex represent the indeces of the vertex to
  examine in thread threadId */
  virtual void ThreadedGenerateData(unsigned int startIndex, unsigned int stopIndex, itk::ThreadIdType threadId);

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
  static ITK_THREAD_RETURN_TYPE ThreaderCallback(void *arg);

  /** Internal structure used for passing image data into the threading library */
  struct ThreadStruct
  {
    Pointer Filter;
  };

  /** End Multi-threading implementation */

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /**
   * Get the composition knowledge.
   * \param r1 First RCC8 relation value,
   * \param r2 Second RCC8 relation value,
   * \return The knowledge associated with the composition.
   */
  KnowledgeStateType GetKnowledge(RCC8ValueType r1, RCC8ValueType r2);

private:
  /** Optimisation flag */
  bool              m_Optimisation;
  unsigned int      m_Accumulator[8];
  EdgeMapVectorType m_EdgesPerThread;

  /** This array stores the indices corresponding to each segmentation */
  SegmentationRangesType m_SegmentationRanges;

  /** If set to true, the filter will also add the invert relationship
    * between v2 and v1 */
  bool m_UseInverted;
};
} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPolygonListToRCC8GraphFilter.txx"
#endif

#endif
