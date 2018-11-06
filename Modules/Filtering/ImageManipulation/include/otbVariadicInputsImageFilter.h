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
#ifndef otb_VariadicInputsImageFilter_h
#define otb_VariadicInputsImageFilter_h

#include "itkImageSource.h"

namespace otb {

template<class TOuptut, class ... TInputs> class VariadicInputsImageFilter : public itk::ImageSource<TOuptut>
{
public:
  using Self = VariadicInputsImageFilter<TOuptut, TInputs...>;
  using Pointer =  itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using Superclass = itk::ImageSource<TOuptut>;

  using InputTypesTupleType = std::tuple<TInputs...>;

  itkNewMacro(Self);
  
  template <std::size_t I> void SetVInput(typename std::tuple_element<I,InputTypesTupleType>::type * inputPtr)
  {
    this->SetNthInput(I,inputPtr);
  }

#define DefineLegacySetInputMacro(n)                                                                               \
  template<typename Tuple = InputTypesTupleType, typename std::enable_if<n<=std::tuple_size<Tuple>::value >::type> \
  void SetInput ## n(typename std::tuple_element<n-1,Tuple>::type * img)                                           \
  {                                                                                                                \
    this->SetVInput<n-1>(img);                                                                                     \
  }

  // The following defines legacy setters SetInput1()
  // ... SetInput10(), only if the number of input type is sufficient
  DefineLegacySetInputMacro(1);
  DefineLegacySetInputMacro(2);
  DefineLegacySetInputMacro(3);
  DefineLegacySetInputMacro(4);
  DefineLegacySetInputMacro(5);
  DefineLegacySetInputMacro(6);
  DefineLegacySetInputMacro(7);
  DefineLegacySetInputMacro(8);
  DefineLegacySetInputMacro(9);
  DefineLegacySetInputMacro(10);

#undef DefineLegacySetInputMacro
  
  template <std::size_t I> const typename std::tuple_element<I,InputTypesTupleType>::type * GetVInput()
  {
    using ImageType = typename std::tuple_element<I,InputTypesTupleType>::type;
    return dynamic_cast<const ImageType *>(this->GetInput(I));
  }

  void SetVInputs(TInputs*... inputs)
  {
    auto inTuple = std::make_tuple(inputs...);
    SetInputsImpl(inTuple,std::make_index_sequence<sizeof...(inputs)>{});
  }

  auto GetVInputs()
  {
    return GetInputsImpl(std::make_index_sequence<sizeof...(TInputs)>{});
  }

protected:
  VariadicInputsImageFilter()
  {
    this->SetNumberOfRequiredInputs(sizeof...(TInputs));
  };
  
  ~VariadicInputsImageFilter() = default;
  
private:
  template<class Tuple, size_t...Is> auto SetInputsImpl(Tuple& t, std::index_sequence<Is...>)
  {
    return std::initializer_list<int>{(this->SetVInput<Is>(std::get<Is>(t)),0)...};
  }

  template <size_t...Is> auto GetInputsImpl(std::index_sequence<Is...>)
  {
    return std::make_tuple(this->GetVInput<Is>()...);
  }
  
  VariadicInputsImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#endif
