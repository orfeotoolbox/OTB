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

/**
 * \brief Base class for image filter with variadic inputs.
 *
 * This filter act as a base class for all filters that will take
 * several input images with different types and produce an output
 * image.
 * 
 * Type for each input is taken from the variadic template parameter
 * TInputs.
 * 
 * Inputs get be set/get with SetVariadicInput<N>() and
 * GetInput<N>(), when N is the index (first input is 0) of
 * the input. This is resolved at compile time: you can not call 
 * SetInput<N>() with an argument not matching the Nth input
 * type (it will lead to type mismatch compilation error).
 * 
 * Alternatively, you can call SetInputs() with all the input
 * image in the same order as in the template parameters.
 *
 * Last, there is a macro that generates SetInput1() ... SetInput10()
 * (iff the number of varidic input types is large enough) for
 * backward compatibility.
 * 
 */
template<class TOuptut, class ... TInputs> class VariadicInputsImageFilter : public itk::ImageSource<TOuptut>
{
public:
  using Self = VariadicInputsImageFilter<TOuptut, TInputs...>;
  using Pointer =  itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  using Superclass = itk::ImageSource<TOuptut>;
  
  using InputTypesTupleType = std::tuple<TInputs...>;
  
  template <size_t I> using InputImageType = typename std::tuple_element<I,InputTypesTupleType>::type;
  static constexpr size_t NumberOfInputs = std::tuple_size<InputTypesTupleType>::value; 

  // Good old new macro
  itkNewMacro(Self);

  /**
   * \param Set the Ith input
   */
  template <std::size_t I = 0> void SetInput(const InputImageType<I> * inputPtr)
  {
    static_assert(NumberOfInputs>I,"Template value I is out of range.");
    this->SetNthInput(I,const_cast<InputImageType<I> *>(inputPtr));
  }
  
#define DefineLegacySetInputMacro(n)                                                                               \
  template<typename Tuple = InputTypesTupleType, typename Check = typename std::enable_if<n<=std::tuple_size<Tuple>::value >::type> \
  void SetInput ## n(const typename std::tuple_element<n-1,Tuple>::type * img)                                           \
  {                                                                                                                \
    this->template SetInput<n-1>(img);                                                                             \
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

  /**
   * Set the first input (for backward compatibility with ImageToImageFilter)
   * \param input The image to use for first input
   */ 
  using Superclass::SetInput;
  // void SetInput(const InputImageType<0> * input)
  // {
  //   SetInput<0>(input);
  // };

  /**
   * Get the first input (for backward compatibility with
   * ImageToImageFilter)
   * \return The first input
   */
  using Superclass::GetInput;
  // InputImageType<0> * GetInput()
  // {
  //   return GetInput<0>();
  // }
 
  /**
   * \return the Ith variadic input
   */
  template <std::size_t I = 0> const InputImageType<I> * GetInput()
  {
    static_assert(NumberOfInputs>I,"Template value I is out of range.");
    return dynamic_cast<const InputImageType<I> *>(this->GetInput(I));
  }

  /**
   * \param inputs A vararg list of inputs 
   */
  void SetInputs(TInputs*... inputs)
  {
    auto inTuple = std::make_tuple(inputs...);
    SetInputsImpl(inTuple,std::make_index_sequence<sizeof...(inputs)>{});
  }

  /**
   * \return A tuple with all inputs
   */
  auto GetInputs()
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
    return std::initializer_list<int>{(this->SetInput<Is>(std::get<Is>(t)),0)...};
  }

  template <size_t...Is> auto GetInputsImpl(std::index_sequence<Is...>)
  {
    return std::make_tuple(this->GetInput<Is>()...);
  }
  
  VariadicInputsImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
};

}

#endif
