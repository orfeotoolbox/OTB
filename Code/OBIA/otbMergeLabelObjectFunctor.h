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
#ifndef __otbMergeLabelObjectFunctor_h
#define __otbMergeLabelObjectFunctor_h


namespace otb
{

namespace Functor
{

/** \class MergeLabelObjectFunctor
 *  \brief Merge two LabelObjects
 *
 * This functor merges the two inputs label objects into a third label
 * object where the Run have been merged. The resulting label object
 * holds data from first input label object copied via the
 * CopyDataFrom() method.
 *
 * Please note that if the two input label objects are disjoint, the
 * resulting label object will have more than one connected component.
 *
 * This functor is thread safe.
 */
template <class TLabelObject >
class MergeLabelObjectFunctor
{
public:
  /** Template parameters typedefs */
  typedef TLabelObject                                LabelObjectType;
  typedef typename LabelObjectType::Pointer           LabelObjectPointerType;
  typedef typename LabelObjectType::LineContainerType LineContainerType;
  typedef typename LabelObjectType::LineType          LineType;
  typedef typename LineType::IndexType                IndexType;


  /**
   * \param l1 First label object to merge (data are copied from this
   * one),
   * \param l2 Second label object to merge
   * \return The merged label object
   */
  inline LabelObjectPointerType operator()(const LabelObjectType * l1, const LabelObjectType * l2) const
  {
    // Retrieve the two input line containers
    LineContainerType lines1 = l1->GetLineContainer();
    LineContainerType lines2 = l2->GetLineContainer();

    // Ensure they are sorted according to our criterion
    stable_sort(lines1.begin(), lines1.end(), &LexicographicalLineCompare);
    stable_sort(lines2.begin(), lines2.end(), &LexicographicalLineCompare);

    // Merge the two containers
    LineContainerType linesOut1(lines1.size()+lines2.size());
    merge(lines1.begin(), lines1.end(), lines2.begin(), lines2.end(), linesOut1.begin(), &LexicographicalLineCompare);

    // Merge consecutive and overlapping lines
    LineContainerType linesOut2;

    // Initialize output final container
    typename LineContainerType::const_iterator lit = linesOut1.begin();
    linesOut2.push_back(*lit);
    ++lit;

    // Walk the merged line container
    while(lit!=linesOut1.end())
      {
      // Test if next line overlaps with current
      if(LinesOverlap(linesOut2.back(), *lit))
   {
   // Merge lines
   LineType mline = MergesLines(linesOut2.back(), *lit);

   // Replace the last line by the merged line
   linesOut2.pop_back();
   linesOut2.push_back(mline);
  }
      else
   {
        // Push back the new line
  linesOut2.push_back(*lit);
   }

      // fetch next line
      ++lit;
      }

    // Create the output label object
    LabelObjectPointerType resp = LabelObjectType::New();
    resp->CopyAllFrom(l1);
    resp->GetLineContainer() = linesOut2;

    // Return the output label object
    return resp;
  }

private:
  /// Compare two line in the lexicographical order with respect to
  /// their start index.
  static bool LexicographicalLineCompare(const LineType &  l1, const LineType & l2)
  {
    bool resp = l2.GetIndex()[1]> l1.GetIndex()[1];
    resp = resp
      || (l2.GetIndex()[1] == l1.GetIndex()[1] && (l1.GetIndex()[0] < l2.GetIndex()[0]));
    return resp;
  }

  /// Check if lines overlap (same row and one of the run end inside the
  /// other run)
  static bool LinesOverlap(const LineType & l1, const LineType & l2)
  {
    // Test if we are in same row
    bool sameRow = l2.GetIndex()[1] == l1.GetIndex()[1];
    bool leftEndInside  = (l2.GetIndex()[0]>= l1.GetIndex()[0] && l2.GetIndex()[0]<=l1.GetIndex()[0]+static_cast<long>(l1.GetLength()));
    bool rightEndInside = (l1.GetIndex()[0]>= l2.GetIndex()[0] && l1.GetIndex()[0]<=l2.GetIndex()[0]+static_cast<long>(l2.GetLength()));

    return (sameRow && (leftEndInside || rightEndInside));
  }

  /// Merge two lines. l1 and l2 are supposed to be in the same row
  /// and to overlap. l1 is supposed to appear before l2
  static const LineType MergesLines(const LineType & l1, const LineType & l2)
  {
    LineType resp;
    resp.SetIndex(l1.GetIndex());
    unsigned long length = std::max(l1.GetLength(), l2.GetIndex()[0]+l2.GetLength()-l1.GetIndex()[0]);
    resp.SetLength(length);
    return resp;
  }

}; // end class MergeLabelObjectFunctor

} // end namespace Functor

} // end namespace otb

#endif


