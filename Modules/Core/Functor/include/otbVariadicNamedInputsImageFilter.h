/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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
/**
 * \struct tuple_index
 * \brief retrieve index of a type in tuple if exists
 *
 * This struct allows to retrieve the index of the first occurence of type
 * Arg in tuple Tuple. If type Arg can not be found, compilation will
 * end with a static_assert failing.
 *
 * ::value holds the index of type Arg in tuple Tuple.
 */
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

/**
 * \class VariadicNamedInputsImageFilter
 * \brief Adds tagged versions for Get/SetInput to otb::VariadicInputsImageFilter
 *
 * This class extends otb::VariadicInputsImageFilter with SetInput<>()
 * and GetInput<>() with tags.
 *
 * Template parameter TInputNameMap should be a tuple of same size as
 * variadic template parameter TInputs. Each type in the tuple will be
 * used as a tag to set/get the corresponding input and thus should be
 * unique in tuple.
 *
 * This allows to add semantic to inputs and remove the need for the
 * user to know input orders and set them by their index.
 *
 * Example of use:
 * \code
 * struct Foo {};
 * struct Bar {};
 * using Names = std::tuple<<Foo,Bar>;
 *
 * using Filter = VariadicNamedInputsImageFilter<OutputType, Names,
 * InputType1, InputType2>;
 *
 * // Set the input corresponding to Foo tag:
 * filter->SetInput<Foo>(in);
 *
 * // Get the input corresponding to Bar tag:
 * auto in = filter->GetInput<Bar>();
 *
 * \endcode
 *
 * \sa otb::VariadicNamedInputsImageFilter
 * \sa otb::FunctorImageFilter
 *
 * \ingroup OTBFunctor
 */
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

  // This checks that TInputNameMap has the correct size
  static_assert(std::tuple_size<TInputNameMap>::value == NumberOfInputs,"Tuple for input name does not match the size of ... TInputs");

  // Good old new macro
  itkNewMacro(Self);

  // Import definitions for SetInput and GetInput to avoid shadowing
  // by SetInput<> and GetIntput<> defined in this method
  using Superclass::GetInput;
  using Superclass::SetInput;

  /**
   * Set the input corresponding to tag Tag
   * \tparam Tag tag of the input
   * \param inputPtr the pointer to the input image
   */
  template <typename Tag>
  void SetInput(const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value>* inputPtr)
  {
    constexpr size_t idx = internal::tuple_index<Tag, TInputNameMap>::value;
    this->SetNthInput(idx,const_cast<InputImageType<idx> *>(inputPtr));
  }

  /**
   * Set the input corresponding to tag Tag
   * \param tag tag of the input
   * \param inputPtr the pointer to the input image
   */
  template <typename Tag>
  void SetInput(Tag, const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value>* inputPtr)
  {
    SetInput<Tag>(inputPtr);
  }

  /**
   * Get the input corresponding to tag Tag
   * \tparam Tag tag of the input
   * \return the pointer to the input image
   */
  template <typename Tag>
  const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value>* GetInput()
  {
    constexpr size_t idx = internal::tuple_index<Tag, TInputNameMap>::value;
    return dynamic_cast<const InputImageType<idx>*>(this->Superclass::GetInput(idx));
  }

  /**
   * Get the input corresponding to tag Tag
   * \param tag tag of the input
   * \return the pointer to the input image
   */
  template <typename Tag>
  const InputImageType<internal::tuple_index<Tag, TInputNameMap>::value>* GetInput(Tag)
  {
    return GetInput<Tag>();
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
