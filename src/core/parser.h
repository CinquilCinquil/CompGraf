#ifndef PARSER_H
#define PARSER_H 1

/*!
 * Implementation of XML processing functions.
 * @file parser.cpp
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <vector>
#include <utility>
#include <optional>

using std::boolalpha;
using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::setw;
using std::string;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::copy;
using std::stringstream;
using std::begin;
using std::endl;
using std::istream_iterator;
using std::vector;
using std::pair;
using std::optional;

#include "../ext/tinyxml2.h"
#include "error.h"
#include "paramset.h"

namespace rt3 {

// === Support functions
template <typename T>
std::optional<std::vector<T>> read_array(tinyxml2::XMLElement*, const string&);
template <typename T>
std::optional<T> read_single_value(tinyxml2::XMLElement* p_element, const string& att_key);

/// Extracts a single COMPOSITE element.
template <typename BASIC, typename COMPOSITE>
bool parse_single_COMPOSITE_attrib(tinyxml2::XMLElement* p_element,
                                   rt3::ParamSet* ps,
                                   string att_key);
/// Extracts an array of COMPOSITE elements.
template <typename BASIC, typename COMPOSITE, int SIZE = 3>
bool parse_array_COMPOSITE_attrib(tinyxml2::XMLElement* p_element,
                                  rt3::ParamSet* ps,
                                  string att_key);

/// Extracts a single BASIC element.
template <typename T>
bool parse_single_BASIC_attrib(tinyxml2::XMLElement* p_element, rt3::ParamSet* ps, string att_key);
/// Extracts an array of BASIC elements.
template <typename T>
bool parse_array_BASIC_attrib(tinyxml2::XMLElement* p_element, rt3::ParamSet* ps, string att_key);

// === Enumerations
/// Type of possible parameter types we may read from the input scene file.
enum class param_type_e : int {
  BOOL = 0,
  INT,          //!< Single integet
  UINT,         //!< Single unsigned int
  REAL,         //!< Single real number
  VEC3F,        //!< Single Vector3f
  VEC3I,        //!< Single Vector3i
  NORMAL3F,     //!< Single Normal3f
  POINT3F,      //!< Single Point3f
  POINT2I,      //!< Single Point2i
  COLOR,        //!< Single Color
  SPECTRUM,     //!< Single Spectrum
  STRING,       //!< Single string
  ARR_INT,      //!< An array of integers
  ARR_REAL,     //!< An array of real numbers
  ARR_VEC3F,    //!< An array of Vector3f
  ARR_VEC3I,    //!< An array of Vector3i
  ARR_POINT3F,  //!< An array of Point3f
  ARR_COLOR,    //!< An array of Color
  ARR_NORMAL3F  //!< An array of Normal3f
};
// === parsing functions.
void parse(const char*);
void parse_tags(tinyxml2::XMLElement*, int);
void parse_parameters(tinyxml2::XMLElement* p_element,
                      const vector<std::pair<param_type_e, string>>& param_list,
                      ParamSet* ps_out);

//-------------------------------------------------------------------------------
}  // namespace rt3

#endif  // PARSER_H
