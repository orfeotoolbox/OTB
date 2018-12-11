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
#ifndef otb_VariadicNamedInputsImageFilter_h
#define otb_VariadicNamedInputsImageFilter_h

#include "otbVariadicInputsImageFilter.h"

namespace otb {

/// TODO: Documentation

namespace internal
{
template<typename Arg, typename Tuple> struct tuple_index;
template<typename Arg, typename...Args> struct tuple_index<Arg, std::tuple<Arg,Args...> >
{
  static constexpr std::size_t value = 0;
};

template<typename Arg, typename NotMatching, typename...Args> struct tuple_index<Arg, std::tuple<NotMatching,Args...>>
{
  static_assert(sizeof...(Args)>0,"Could not find requested type in tuple");
  static constexpr std::size_t value = 1 + tuple_index<Arg,std::tuple<Args...>>::value;
};
}

template<class TOuptut, class TInputNameMap, class ... TInputs> class VariadicNamedInputsImageFilter : public VariadicInputsImageFilter<TOuptut,TInputs...>
{
public:
  using Self         = VariadicNamedInputsImageFilter<TOuptut,TInputNameMap, TInputs...>;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using Superclass   = VariadicInputsImageFilter<TOuptut, TInputs...>;;

  using InputTypesTupleType                = typename Superclass::InputTypesTupleType;
  template <size_t I> using InputImageType = typename Superclass::template InputImageType<I>;
  static constexpr size_t NumberOfInputs   = Superclass::NumberOfInputs;

  static_assert(std::tuple_size<TInputNameMap>::value == NumberOfInputs,"Tuple for input name does not match the size of ... TInputs");
  
  itkNewMacro(Self);
  
  template <typename Tag> void SetVariadicNamedInput(const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value> * inputPtr)
  {
    constexpr size_t idx = internal::tuple_index<Tag, TInputNameMap>::value;
    this->SetNthInput(idx,const_cast<InputImageType<idx> *>(inputPtr));
  }

  template <typename Tag> void SetVariadicNamedInput(Tag,const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value> * inputPtr)
  {
    SetVariadicNamedInput<Tag>(inputPtr);
  }
  
  template <typename Tag> const InputImageType<internal::tuple_index<Tag,TInputNameMap>::value> * GetVariadicNamedInput()
  {
    constexpr size_t idx = internal::tuple_index<Tag, TInputNameMap>::value;
    return dynamic_cast<const InputImageType<idx> *>(this->GetInput(idx));
  }

  template <typename Tag> const InputImageType<internal::tuple_index<Tag,TInputNameMap>::value> * GetVariadicNamedInput(Tag)
  {
    return GetVariadicNamedInput<Tag>();
  }

protected:
  VariadicNamedInputsImageFilter() = default;
  ~VariadicNamedInputsImageFilter() = default;
  
private:
  VariadicNamedInputsImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#endif
