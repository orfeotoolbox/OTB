/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbXmlWrapper_h
#define otbXmlWrapper_h

#include "otbMacro.h"
#include "otbStringUtilities.h"
#include "otb_tinyxml.h" // Need to include this one to avoid link errors!
#include <string>
#include <cassert>
#include <istream>

namespace otb
{
namespace xml
{

namespace details
{

/** Helper class used to maintain XPath context in XML documents for reporting
 * errors.
 *
 * Implicitly copyable and movable.
 */
struct error_context
{
  /**
   * Simplified constructor.
   * \param[in] filename_  name of the file associated to the XML document.
   * \throw std::bad_alloc if filename cannot be copied
   * \post `m_xpath` is empty
   */
  explicit error_context(std::string filename_)
  : m_filename(std::move(filename_))
  {}

  /**
   * Simplified constructor.
   * \param[in] filename_  name of the file associated to the XML document.
   * \param[in] xpath_     initial value to pinpoint xpath position
   * \throw std::bad_alloc if filename or xpath cannot be copied
   */
  explicit error_context(std::string filename_, std::string xpath_)
  : m_filename(std::move(filename_))
  , m_xpath(std::move(xpath_))
  {}

  /**
   * Appends extra path to xpath memorized.
   * \param[in] extra_path  path to append
   * \return current object
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion for
   * appending to xpath.
   * \post xpath is old-value "/" extra_path ; or just extra_path if old value
   * was empty.
   */
  error_context& operator+=(otb::string_view extra_path)
  {
    if (!m_xpath.empty())
      m_xpath += '/';
    m_xpath.append(std::begin(extra_path), std::end(extra_path));
    return *this;
  }

  /**
   * Creates a new error_context by appending context to xpath.
   * \param[in] ec context copied and completed
   * \param[in] extra_path  path to append
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion for
   * appending to xpath.
   * \post result xpath is `ec.xpath` "/" extra_path ; or just extra_path if
   * old value was empty.
   * \internal implemented as an hidden friend!
   */
  OTB_NODISCARD
  friend error_context operator+(error_context ec, otb::string_view extra_path)
  {
    ec += extra_path;
    return ec;
  }

  /** Raises an exception from current context.
   * \return NEVER
   * \throw std::runtime_error with current `xpath` and `filename` information
   */
  [[noreturn]]
  void raise() const
  {
    otbRaise("No <" << m_xpath << "> node found in '" << m_filename << "'");
  }

  /** Returns current context information as a string. */
  OTB_NODISCARD
  std::string context() const
  {
    return '<' + m_xpath + "> from '" + m_filename + "'";
  }

  /// filename accessor.
  OTB_NODISCARD
  std::string const& filename() const noexcept
  { return m_filename; }

private:
  std::string m_filename;
  std::string m_xpath;
};


/**
 * Generic read-only wrapper over TinyXML objects.
 * Acts as a view over existing instances of `TiXmlBase`.
 *
 * Unlike `TiXmlHandle` with which this class shares a few similarities,
 * `NodeImpl` tries to be more C++ idiomatic:
 * - iterable with for-range loops
 * - multi-parameters function for finding nodes (instead of chaining multiple
 *   calls)
 * - template converter function of the _text_ element
 * - informative error messages when nodes can't be found, or can't be decoded.
 *
 * \tparam TNode  any child class of `TiXmlBase`
 */
template <typename TNode>
class NodeImpl
{
  static_assert(std::is_base_of<TiXmlBase, TNode>::value, "Expect a TinyXml Base node");

public:

  /**
   * Converting constructor from TinyXML base node pointer.
   * \param[in] node_ node that'll be wrapped. May be null.
   *
   * \throw None (expecting `std::string` to rely on SSO)
   * \post error_context filename is set to "<>"
   */
  // OTB_NODISCARD requires C++20
  NodeImpl(TNode const* node_)
  : NodeImpl(node_, error_context("<>"))
  {}

  /**
   * Internal constructor when full xpath context is known.
   * \param[in] node_ node that'll be wrapped. May be null.
   * \param[in] context  current full context pinpointing node position
   *
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
   * constructing short strings
   *
   * \internal
   * This constructor s not expected to be used directly by end-user code.
   * The `error_context` will be initialized in `Document` object, then
   * propagated and updated automatically
   */
  // OTB_NODISCARD requires C++20
  NodeImpl(TNode const* node_, error_context context)
  : node(node_)
  , xpath_context(std::move(context))
  {}

  /**
   * Tells whether the node held is not null.
   * \return whether `node != nullptr`
   * \throw None
   */
  explicit operator bool() const noexcept { return node != nullptr; }

  /**
   * Deferencing operator.
   * \return the pointer to the internal TinyXML node pointer.
   * \throw None
   * \pre `node != nullptr`
   */
  OTB_NODISCARD
  TNode const* operator*() const noexcept
  {
    assert(node);
    return *node;
  }

  /**
   * Pointer operator.
   * \return the pointer to the internal TinyXML node pointer.
   * \throw None
   * \pre `node != nullptr`
   */
  OTB_NODISCARD
  TNode const* operator->() const noexcept
  {
    assert(node);
    return node;
  }

  /**
   * Finds nodes from an xapth(-like) list of parameters.
   * Recursivelly descend into the tree structure following the requested
   * xpath-like sequence to return the associated node (as a wrapper around
   * `TiXmlElement`)
   * \tparam C  variadic list of `char const*`
   * \param[in] path  head of the xpath-like sequence
   * \param[in] tail  tail... of the xpath-like sequence
   *
   * \return the node found, wrapped.
   * \throw std::runtime_error if the request path cannot be found from the
   * current node. The full path of the requested node will be used in
   * exception message.
   * \pre current node is not null
   * \pre `path != nullptr`
   * \pre `tail... != nullptr`
   * \post result node won't be null
   */
  template <typename... C>
  OTB_NODISCARD
  NodeImpl<TiXmlElement> find(char const* path, C const*... tail) const
  {
    assert(path);
    assert(this->node);

    auto  context = xpath_context + path;
    auto* sub     = node->FirstChildElement(path);
    if (!sub)
    {
      context.raise();
    }
    return NodeImpl<TiXmlElement>(sub, std::move(context)).find(tail...);
  }

  /**
   * Extracts current node text.
   * \return current node text.
   * \pre current node is not null
   */
  OTB_NODISCARD
  otb::string_view text() const
  {
    assert(node);
    auto txt = node->GetText();
    assert(txt);
    return txt;
  }

  /**
   * Converts current node text into the resqued type.
   * \tparam T  destination type for converting current node text.
   * \return current node text, converted.
   * \throw std::runtime_error if the current node text cannot be fully
   * converted to the requested type. Precise information will be returned
   * through the exception message.
   * \pre current node is not null
   */
  template <typename T>
  OTB_NODISCARD
  T as() const
  {
    return otb::to<T>(this->text(), "extracting " + xpath_context.context());
  }

  /**
   * Finds subnode text.
   * Syntaxic sugar around `find(path...).text()`.
   * \tparam C  variadic list of `char const*`
   * \param[in] path  xpath-like sequence
   * \return text of the node found.
   *
   * \throw std::runtime_error if the request path cannot be found from the
   * current node. The full path of the requested node will be used in
   * exception message.
   *
   * \pre current node is not null
   * \pre `path... != nullptr`
   */
  template <typename... C>
  OTB_NODISCARD
  otb::string_view get(C const*... path) const
  {
    assert(node);
    return find(path...).text();
  }

  /**
   * Finds and converts subnode text.
   * Syntaxic sugar around `find(path...).as<T>()`.
   * \tparam T  destination type for converting node text.
   * \tparam C  variadic list of `char const*`
   * \param[in] path  xpath-like sequence
   * \return converted text of the node found.
   *
   * \throw std::runtime_error if the request path cannot be found from the
   * current node. The full path of the requested node will be used in
   * exception message.
   * \throw std::runtime_error if the node text cannot be fully converted to
   * the requested type. Precise information will be returned through the
   * exception message.
   *
   * \pre current node is not null
   * \pre `path... != nullptr`
   */
  template <typename T, typename... C>
  OTB_NODISCARD
  T get(C const*... path) const
  {
    assert(node);
    return find(path...).template as<T>();
  }

  /** `NodeImpl` (const) iterator type.
   */
  class const_iterator
  {
  public:
    /** Defaut construtor to point to `end()`.
     * \throw None
     * \post always returns true when compared to `node().end()`
     */
    const_iterator() noexcept = default;

    /**
     * Pre-increments the iterator.
     * \return current iterator
     * \throw None
     * \pre the iterator shall point to a node!
     * \post the iterator references the next sibling.
     */
    const_iterator& operator++()
    {
      assert(current_node);
      current_node = current_node->NextSiblingElement();
      return *this;
    }

    /**
     * Post-increments the iterator.
     * \return an iterator to the next sibling
     * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
     * copying iterator node context.
     * \pre the iterator shall point to a node!
     * \post the returned iterator references the next sibling.
     */
    const_iterator operator++(int)
    {
      auto copy = *this;
      ++(*this);
      return copy;
    }

    /**
     * Dereferencing operator.
     * \return an `otb::xml::Element` over the curret node.
     * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
     * copying iterator node context.
     */
    OTB_NODISCARD
    NodeImpl<TiXmlElement> operator*() const
    {
      return {current_node, xpath_context};
    }

    /**
     * Equality comparison operator.
     * \return whether the iterators reference the same node.
     * \throw None
     * \internal implemented as an hidden friend!
     */
    OTB_NODISCARD
    friend bool operator==(const_iterator lhs, const_iterator rhs) noexcept
    {
      return lhs.current_node == rhs.current_node;
    }

    /**
     * Difference comparison operator.
     * \return whether the iterators reference different nodes.
     * \throw None
     * \internal implemented as an hidden friend!
     */
    OTB_NODISCARD
    friend bool operator!=(const_iterator lhs, const_iterator rhs) noexcept
    {
      return !(lhs == rhs);
    }

  private:
    // Only NodeImpl::begin() function is authorized to build new iterators
    // from node children.
    friend class NodeImpl;

    /**
     * Iterator init constructor over sequence start.
     * \param[in] start  first node of a siblings sequence; may be null
     * \param[in] context  context of the parent node
     *
     * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
     * copying iterator node context.
     */
    const_iterator(TiXmlElement const* start, error_context context)
    : current_node(start)
    , xpath_context(std::move(context))
    {}

    TiXmlElement const* current_node = nullptr;
    error_context       xpath_context;
  };

  /** Returns iterator to first child -- if any.
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
   * copying iterator node context.
   * \pre current node is not null
   */
  OTB_NODISCARD
  const_iterator begin() const
  {
    assert(node);
    return const_iterator(
        node->FirstChildElement(),
        xpath_context + "*"
    );
  }

  /** Returns iterator to end sequence.
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
   * copying iterator node context.
   * \pre current node is not null
   */
  OTB_NODISCARD
  const_iterator end() const
  {
    assert(node);
    return const_iterator(
        nullptr,
        xpath_context + "*"
    );
  }

  /// Constant overload of `begin()`.
  OTB_NODISCARD const_iterator cbegin() const noexcept { return begin(); }
  /// Constant overload of `end()`.
  OTB_NODISCARD const_iterator cend()   const noexcept { return end();   }

  /// Context mutator.
  void reset_context(error_context ec) { xpath_context = std::move(ec); }

  /// Context accessor.
  OTB_NODISCARD
  auto const& context() const noexcept
  {return xpath_context; }

private:
  // Only other `NodeImpl` instances are allowed to call
  // `NodeImpl<TiXmlElement>::find()`
  template <typename U>
  friend class NodeImpl;

  /** Final overload recursion over xpath parameter */
  NodeImpl<TiXmlElement> find() const { return *this; }

  TNode const*  node;
  error_context xpath_context;
};

/** Internal wrapper class over actual `TiXmlDocument instance.
 * This class is not a view. It holds the document, and all its children.
 *
 * Copyable
 */
struct WrapDocument
{
  /// Wraps an empty document.
  WrapDocument() = default;
  /** Wraps a document.
   * \post the document is not loaded automatically at this point. It'll be
   * done in `otb::xml::Document`.
   */
  WrapDocument(char const* filename)
  : m_doc(filename)
  {}

  /// Document accessors.
  TiXmlDocument const* doc() const { return &m_doc; }
  TiXmlDocument      * doc()       { return &m_doc; }

#ifdef TIXML_USE_STL
  /**
   * Read document from stream.
   * \param[in,out] is  input stream
   * \return the stream in its updated state.
   */
  std::istream& read(std::istream& is) { return is >> m_doc; }
#endif

private:
  TiXmlDocument m_doc;
};

} // namespace details

/// Typdef over a wrapped `TiXmlNode`
using Node     = details::NodeImpl<TiXmlNode>;

/// Typdef over a wrapped `TiXmlElement`
using Element  = details::NodeImpl<TiXmlElement>;

/** Main xml document wrapper.
 * This class is likelly the netry point for every manipulation.
 *
 * From there, node views can be found and interrogated for their content.
 *
 * Non copyable, but movable -- as OTB is still in C++14. C++17 garantied
 * copy/move elision on return is exactly what we need.
 *
 * \internal The actual document is held through `details::WrapDocument` first
 * parent. And it can be manipulated through `details::NodeImpl` second parent.
 * The inheritance order is important as the second parent depends on the
 * actual `TiXmlDocument` instance held in the first parent.
 */
struct Document
: private details::WrapDocument // 1st to be able to introduce dependency for construction
, public  details::NodeImpl<TiXmlDocument>
{
  /**
   * Default constructor.
   * Just initialise the document. Eventually, it will be required to update it
   * through `operator<<` typically.
   *
   * This constructor is mainly expected to be used when initialising an XML
   * document from a string, typically in unit tests.
   * \post content is empty
   */
  Document()
  : NodeImpl(this->doc()) // yes we depend on the first parent already constructed
  {}

  /**
   * Constructor that loads XML document from a file.
   * Unlike `TiXmlDocument`, this constructor will load the XML file.
   * \param[in] xml_filename  name of the file loaded.
   * \throw std::bad_alloc
   * \throw std::runtime_error if the file is not a valid XML file.
   * \post content is initialized from `xml_filename` content
   */
  explicit Document(std::string xml_filename)
  : WrapDocument(xml_filename.c_str())
  , NodeImpl(this->doc(), details::error_context(std::move(xml_filename)))
  {
    if (!doc()->LoadFile())
    {
      otbRaise("Cannot open XML file '" << context().filename() << "'");
    }
  }

#ifdef TIXML_USE_STL
  /**
   * Stream extractor operator for XML documents.
   * \param[in,out] is  input stream
   * \param[out] d  XML document that will be updated
   * \return the stream in its updated state.
   * \throw std::bad_alloc in the unlikelly case of memory exhaustion while
   * reading the file.
   */
  friend std::istream& operator>>(std::istream& is, Document& d)
  {
    if (d.read(is))
      d.reset_context(details::error_context("<stream>"));
    return is;
  }
#endif

  Document(Document const&)            = delete;
  Document& operator=(Document const&) = delete;
  Document& operator=(Document&&)      = delete;

  // Overridden to point to the correct document.
  Document(Document&& rhs) noexcept
  : WrapDocument(std::move(rhs))
  , NodeImpl(this->doc(), rhs.context())
  {
  }
};

} // xml namespace
} // otb namespace

#endif // otbXmlWrapper_h
