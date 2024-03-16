/*!
 * Implementation of XML processing functions.
 * @file parser.h
 */

#include "parser.h"

#include <cstdint>
#include "api.h"
#include "paramset.h"
#include "rt3.h"

// === Function Implementation

namespace rt3 {

using rt3::Point3f;
using rt3::Vector3f;
using rt3::Vector3i;

/// This is the entry function for the parsing process.
void parse(const char* scene_file_name) {
  tinyxml2::XMLDocument xml_doc;

  // Load file.
  if (xml_doc.LoadFile(scene_file_name) != tinyxml2::XML_SUCCESS) {
    RT3_ERROR(std::string{ "The file \"" } + scene_file_name
              + std::string{ "\" either is not available OR contains an invalid "
                             "RT3 scene provided!" });
  }

  // ===============================================
  // Get a pointer to the document's root node.
  tinyxml2::XMLNode* p_root = xml_doc.FirstChild();
  if (p_root == nullptr) {
    RT3_ERROR("Error while trying to find \"RT3\" tag in the scene file.");
  }
  // ===============================================

  // Get the first-level tag inside the root node.
  auto* p_child{ p_root->FirstChildElement() };
  if (p_child == nullptr) {
    RT3_ERROR("No \"children\" tags found inside the \"RT3\" tag. Empty scene file?");
  }

  parse_tags(p_child, /* initial level */ 0);
}

/// Main loop that handles each possible tag we may find in a RT3 scene file.
void parse_tags(tinyxml2::XMLElement* p_element, int level) {
  /// Lambda expression that returns a lowercase version of the input string.
  constexpr auto csrt_lowercase = [](const char* t) -> std::string {
    std::string str{ t };
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
  };
  // All messages sent to std::clog is for DEBUG purposes.
  std::clog << "[parse_tags()]: level is " << level << '\n';

  // Traverse all items on the children's level.
  while (p_element != nullptr) {
    // Convert the attribute name to lowecase before testing it.
    auto tag_name = csrt_lowercase(p_element->Value());
    clog << "\n"
         << setw(level * 3) << "" << "***** Tag id is `" << tag_name << "`, at level " << level
         << '\n';

    // Big switch for each possible RT3 tag type.
    if (tag_name == "camera") {
      ParamSet ps;
      // List of parameters that might be defined inside the camera's tag.
      vector<std::pair<param_type_e, string>> param_list{
        { param_type_e::STRING, "type" },
        // Legacy parameters BEGINS.
        // In this legacy version, we specify the cameras location
        // manually, instead of defining a transformation matrix.
        { param_type_e::POINT3F, "look_from" },
        { param_type_e::POINT3F, "look_at" },
        { param_type_e::VEC3F, "up" },
        { param_type_e::ARR_REAL, "vpdim" },  // The viewport dimensions defined by the scene
        // Legacy parameters ENDS.
        { param_type_e::REAL, "fovy" },
        { param_type_e::REAL, "focal_distance" },
        { param_type_e::REAL, "frame_aspectratio" },  // By default it's calulated from the x/y
                                                      // resolution, but it might be overridden.
        { param_type_e::ARR_REAL, "screen_window" }   // Bounds of the film in screen space. [-1,1]
                                                      // along the shorter image axis, and its set
                                                      // proportionally along the longer axis.
      };

      parse_parameters(p_element, param_list, /* out */ &ps);

      // API::camera(ps);
    } else if (tag_name == "background") {
      ParamSet ps;
      vector<std::pair<param_type_e, string>> param_list{
        { param_type_e::STRING, "type" },
        { param_type_e::STRING, "filename" },  // Texture file name.
        { param_type_e::STRING, "mapping" },   // Type of mapping required.
        { param_type_e::COLOR, "color" },      // Single color for the entire background.
        { param_type_e::COLOR, "tl" },         // Top-left corner
        { param_type_e::COLOR, "tr" },         // Top-right corner
        { param_type_e::COLOR, "bl" },         // Bottom-left corner
        { param_type_e::COLOR, "br" }          // Bottom-right corner
      };
      parse_parameters(p_element, param_list, /* out */ &ps);

      API::background(ps);
    } else if (tag_name == "film") {
      ParamSet ps;
      vector<std::pair<param_type_e, string>> param_list{
        { param_type_e::STRING, "type" },
        { param_type_e::STRING, "filename" },
        { param_type_e::STRING, "img_type" },
        { param_type_e::INT, "x_res" },
        { param_type_e::INT, "y_res" },
        { param_type_e::ARR_REAL, "crop_window" },
        { param_type_e::STRING, "gamma_corrected" }  // bool
      };
      parse_parameters(p_element, param_list, /* out */ &ps);

      API::film(ps);
    } else if (tag_name == "lookat") {
      ParamSet ps;
      vector<std::pair<param_type_e, string>> param_list{ { param_type_e::POINT3F, "look_from" },
                                                          { param_type_e::POINT3F, "look_at" },
                                                          { param_type_e::VEC3F, "up" } };

      parse_parameters(p_element, param_list, /* out */ &ps);
      // API::look_at(ps);
    } else if (tag_name == "world_begin") {
      // std::clog << ">>> Entering WorldBegin, at level " << level+1 <<
      // std::endl;
      //  We should get only one `world` tag per scene file.
      API::world_begin();
    } else if (tag_name == "world_end") {
      API::world_end();
      // std::clog << ">>> Leaving WorldBegin, at level " << level+1 <<
      // std::endl;
    }
    // else RT3_WARNING( "Undefined tag `" + tag_name + "` found!" );

    // Get next (to the right) sibling on this tree level.
    p_element = p_element->NextSiblingElement();
  }
}

/// Universal parameters parser.
/*!
 * This function receives a list of pairs <param_type, name>, traverse all the
 * attributes found in `p_element` and extract the attribute values into the
 * `ps_out` `ParamSet` object. Only named attributes found are read into the
 * `ps_out`.
 *
 * @param p_element XML element we are extracting information from.
 * @param param_list List of pairs <param_type, name> we need to extract from
 * the XML element.
 * @param ps_out The `ParamSet` object we need to fill in with parameter
 * information extracted from the XML element.
 */
void parse_parameters(tinyxml2::XMLElement* p_element,
                      const vector<std::pair<param_type_e, string>>& param_list,
                      ParamSet* ps_out) {
  // std::clog << "parse_parameters(): p_element = " << p_element << endl;

  // Traverse the list of paramters pairs: type + name.
  for (const auto& e : param_list) {
    const auto& [type, name] = e;  // structured binding, requires C++ 17
    std::clog << "---Parsing att \"" << name << "\", type = " << (int)type << "\n";
    // This is just a dispatcher to the proper extraction functions.
    switch (type) {
    // ATTENTION: We do not parse bool from the XML file because TinyXML2 cannot
    // parse one. Bools are treated as strings. case param_type_e::BOOL:
    // parse_single_BASIC_attrib<bool>( p_element, ps_out, name );
    // break;
    case param_type_e::UINT:
      parse_single_BASIC_attrib<unsigned int>(p_element, ps_out, name);
      break;
    case param_type_e::INT:
      parse_single_BASIC_attrib<int>(p_element, ps_out, name);
      break;
    case param_type_e::REAL:
      parse_single_BASIC_attrib<real_type>(p_element, ps_out, name);
      break;
    case param_type_e::STRING:
      parse_single_BASIC_attrib<std::string>(p_element, ps_out, name);
      break;
    case param_type_e::VEC3F:
      parse_single_COMPOSITE_attrib<real_type, Vector3f>(p_element, ps_out, name);
      break;
    case param_type_e::VEC3I:
      parse_single_COMPOSITE_attrib<int, Vector3i>(p_element, ps_out, name);
      break;
    case param_type_e::NORMAL3F:
      parse_single_COMPOSITE_attrib<real_type, Normal3f>(p_element, ps_out, name);
      break;
    case param_type_e::POINT3F:
      parse_single_COMPOSITE_attrib<real_type, Point3f>(p_element, ps_out, name);
      break;
    // case param_type_e::POINT2I:
    // parse_single_COMPOSITE_attrib<int, Point2i, int(2)>( p_element, ps_out,
    // name ); break;
    case param_type_e::COLOR:
      parse_single_COMPOSITE_attrib<uint8_t, Color24>(p_element, ps_out, name);
      break;
    case param_type_e::SPECTRUM:
      parse_single_COMPOSITE_attrib<real_type, Spectrum>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_REAL:
      parse_array_BASIC_attrib<real_type>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_INT:
      parse_array_BASIC_attrib<int>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_VEC3F:
      parse_array_COMPOSITE_attrib<real_type, Vector3f>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_VEC3I:
      parse_array_COMPOSITE_attrib<int, Vector3i>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_NORMAL3F:
      parse_array_COMPOSITE_attrib<real_type, Normal3f>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_POINT3F:
      parse_array_COMPOSITE_attrib<real_type, Point3f>(p_element, ps_out, name);
      break;
    case param_type_e::ARR_COLOR:
      parse_array_COMPOSITE_attrib<uint8_t, Color24>(p_element, ps_out, name);
      break;
    default:
      RT3_WARNING(string{ "parse_params(): unkonwn param type received!" });
      break;
    }
    clog << "---Done!\n";
  }
}

//-------------------------------------------------------------------------------

/*!
 * This function parses a set of 2 or 3 basic values to form a composite
 * element. For instance, if we have in the scene file points="1 2 3 " and they
 * represent a Point3f, this function will store in the ParamSet a
 * Point3f{1,2,3}.
 */
template <typename BASIC, typename COMPOSITE>
bool parse_single_COMPOSITE_attrib(tinyxml2::XMLElement* p_element,
                                   rt3::ParamSet* ps,
                                   string att_key) {
  // Attribute() returns the value of the attribute as a const char *, or
  // nullptr if such attribute does not exist.
  const char* att_value_cstr = p_element->Attribute(att_key.c_str());
  // Test whether the att_key exists.
  if (att_value_cstr) {
    // Create a temporary array to store all the BASIC data. (e.g. BASIC =
    // float) This read all the BASIC values into a single array.
    auto result = read_array<BASIC>(p_element, att_key);
    // Error check
    if (not result.has_value()) {
      RT3_ERROR(string{ "parse_single_COMPOSITE_attrib(): could not read values "
                        "for attribute \""
                        + att_key + "\"!" });
    }

    // Values ok, get the value inside optional.
    vector<BASIC> values{ result.value() };
    // Get array length
    auto n_basic{ values.size() };  // How many?
    // Create the COMPOSITE value.
    COMPOSITE comp;
    if (n_basic == 2) {
      comp = COMPOSITE{ values[0], values[1] };
    } else if (n_basic == 3) {
      comp = COMPOSITE{ values[0], values[1], values[2] };
    } else {
      return false;  // Invalid number of basic components.
    }

    // Store the vector of composites in the ParamSet object.
    // Recall that `ps` is a dictionary, that receives a pair { key, value }.
    (*ps)[att_key] = std::make_shared<Value<COMPOSITE>>(comp);
    // --------------------------------------------------------------------------
    // Show message (DEBUG only, remove it or comment it out if code is
    // working).
    // --------------------------------------------------------------------------
    clog << "\tAdded attribute (" << att_key << ": \"";
    for (const auto& e : comp) {
      clog << e << " ";
    }
    clog << "\")\n";
    // --------------------------------------------------------------------------

    return true;
  }
  return false;
}

/*!
 * \brief Retrieves from the XML tree an array of basics elements into a
 * composite type.
 *
 * Retrieves from the XML element an array of BASIC type elements (usually
 * floats), convert it into a COMPOSITE type with 3 fields (typically Point3f or
 * Vector3f), and store it in the ParamSet object.
 *
 * For instance, if we have in the scene file points="1 2 3 4 2 4 0 0 1" and
 * they represent a list of Point3f, this function will store in the ParamSet
 * this vector: { {Point3f{1,2,3}, Point3f{4,2,4}, Point3f{0,0,1} }
 *
 * \t_param T_BASIC The basic type of the array elements we want to convert from.
 *
 * \t_param T_COMPOSITE The composite type of the single element we want to convert to.
 * \t_param T_COMPOSITE_SIZE Number of dimensions of the composite type. Default is 3.
 *
 * \param p_element The pointer to the XML Element node.
 * \param ps The output `ParamSet` object we need to fill in.
 * \param att_key The id of the attribute we want to read from.
 *
 * \return `true` if the parsing goes smoothly, `false` otherwise.
 */
template <typename BASIC, typename COMPOSITE, int COMPOSITE_SIZE>
bool parse_array_COMPOSITE_attrib(tinyxml2::XMLElement* p_element,
                                  rt3::ParamSet* ps,
                                  string att_key) {
  // Attribute() returns the value of the attribute as a const char *, or
  // nullptr if such attribute does not exist.
  const char* att_value_cstr = p_element->Attribute(att_key.c_str());
  // Test whether the att_key exists.
  if (att_value_cstr) {
    // [1]
    // Create a temporary array to store all the BASIC data. (e.g. BASIC =
    // float) This read all the BASIC values into a single array. The next step
    // is to break it into COMPOSITE units. (e.g. COMPOSITE = Point3f)
    auto result = read_array<BASIC>(p_element, att_key);
    // Error check
    if (not result.has_value()) {
      RT3_ERROR(string{ "parse_array_COMPOSITE_attrib(): could not read values "
                        "for attribute \""
                        + att_key + "\"!" });
    }

    // Values ok, get the value inside optional.
    vector<BASIC> values{ result.value() };
    // Get array length
    auto n_basic{ values.size() };  // How many?
    // Create the vector that will hold the COMPOSITE values.
    vector<COMPOSITE> composit_list;

    // [2]
    // Fill in `composit_list` with `COMPOSITE`: every COMPOSITE_SIZE values we
    // have a 2D or 3D coordinate. For example, if we have values = { 1, 1, 1,
    // 2, 2, 2, 4, 4, 4, 8, 8, 8} and COMPOSITE = Vector3f, we must extract 4
    // Vector3f: Vector3f{1,1,1}, {2,2,2}, ..., {8,8,8}.
    for (auto i{ 0U }; i < n_basic / COMPOSITE_SIZE; i++) {
      std::cout << "COMPOSITE_SIZE = " << COMPOSITE_SIZE << '\n';
      // Call the proper constructor, as in Vector3f{x,y,z} or Vector2f{x,y}.
      // If, say, COMPOSITE = Vector3f, this will call the constructor
      // Vector3f{x,y,z}.
      if (COMPOSITE_SIZE == 3) {
        composit_list.push_back(
          COMPOSITE{ values[3 * i + 0], values[3 * i + 1], values[3 * i + 2] });
      } else {  // COMPOSITE_SIZE == 2
        composit_list.push_back(COMPOSITE{ values[2 * i + 0], values[2 * i + 1] });
      }
    }

    // [3]
    // Store the vector of composites in the ParamSet object.
    // Recall that `ps` is a dictionary, that receives a pair { key, value }.
    (*ps)[att_key] = std::make_shared<Value<vector<COMPOSITE>>>(composit_list);
    // --------------------------------------------------------------------------
    // Show message (DEBUG only, remove it or comment it out if code is
    // working).
    // --------------------------------------------------------------------------
    clog << "\tAdded attribute (" << att_key << ": \"";
    for (const auto& e : composit_list) {
      for (const auto& x : e) {
        clog << x << " ";
      }
    }
    clog << "\")\n";
    // --------------------------------------------------------------------------

    return true;
  }
  return false;
}

template <typename T>
bool parse_array_BASIC_attrib(tinyxml2::XMLElement* p_element, rt3::ParamSet* ps, string att_key) {
  // Attribute() returns the value of the attribute as a const char *, or
  // nullptr if such attribute does not exist.
  const char* att_value_cstr = p_element->Attribute(att_key.c_str());
  // Test whether the att_key exists.
  if (att_value_cstr) {
    // Create a temporary array to store all the T data. (e.g. T = float)
    // This read all the T values into a single array.
    auto result = read_array<T>(p_element, att_key);
    // Error check
    if (not result.has_value()) {
      RT3_ERROR(string{ "parse_array_BASIC_attrib(): could not read values for attribute \""
                        + att_key + "\"!" });
    }
    // Values ok, get the value inside optional.
    vector<T> values{ result.value() };
    // Store the vector of T in the ParamSet object.
    // Recall that `ps` is a dictionary, that receives a pair { key, value }.
    (*ps)[att_key] = std::make_shared<Value<vector<T>>>(values);
    // --------------------------------------------------------------------------
    // Show message (DEBUG only, remove it or comment it out if code is
    // working).
    // --------------------------------------------------------------------------
    clog << "\tAdded attribute (" << att_key << ": \"";
    for (const auto& e : values) {
      clog << e << " ";
    }
    clog << "\")\n";
    // --------------------------------------------------------------------------

    return true;
  }
  return false;
}

/// Parse the XML element `p_element` looking for an attribute `att_key` and
/// extract one or more values into the `ParamSet` `ps`.
template <typename T>
bool parse_single_BASIC_attrib(tinyxml2::XMLElement* p_element, rt3::ParamSet* ps, string att_key) {
  // Test whether the att_key exists. Attribute() returns the value of the
  // attribute, as a const char *, or nullptr if such attribute does not exist.
  if (p_element->Attribute(att_key.c_str())) {
    std::cout << "\tAttribute \"" << att_key << "\" present, let us extract it!\n";
    auto result = read_single_value<T>(p_element, att_key);
    if (result.has_value()) {
      // Store the BASIC value in the ParamSet object.
      // Recall that `ps` is a dictionary, that receives a pair { key, value }.
      // ps->operator[]( att_key ) = values;
      (*ps)[att_key] = std::make_shared<Value<T>>(result.value());
      // const auto [ it, success ] = ps->insert({att_key, std::make_shared<
      // Value<T> >( values )}
      // ); Show message
      std::cout << "\tInsertion of \"" << att_key << "\" succeeded\n";
      clog << "\tAdded attribute (" << att_key << ": \"" << result.value() << "\" )\n";
      return true;
    }
  }
  return false;
}

template <typename T>
std::optional<std::vector<T>> read_array(tinyxml2::XMLElement* p_element, const string& att_key) {
  // outgoing list of values retrieved from the XML doc.
  vector<T> vec;
  // C-style string that will store the attributes read from the XML doc.
  const char* value_cstr{ nullptr };
  // Retrieve the string value into the `value_str` C-style string. Ex. "1 2 3"
  p_element->QueryStringAttribute(att_key.c_str(), &value_cstr);

  // If query fails, return nothing.
  if (value_cstr == nullptr) {
    return std::nullopt;
  }

  // ==========[ I might make this a function ]======================
  // auto tokens = get_tokens(value_cstr);
  // ----------------------------------------------------------------
  // Separate individual BASIC elements as tokens.
  string str(value_cstr);
  std::stringstream tokenizer(str);
  std::vector<std::string> tokens;
  tokens.insert(tokens.begin(),
                std::istream_iterator<std::string>(tokenizer),
                std::istream_iterator<std::string>());
  //=====================================================================

  // Check whether we got at least one element.
  if (!tokens.empty()) {
    // Convert them to T and add them to the output vector.
    for_each(tokens.begin(),
             tokens.end(),
             // Lambda expression to convert a string "value" to T (BASIC)
             // value and then store it into a vector.
             [&vec](const string& token) {
               T value;
               // =======================================================================
               // RT3_WARNING: THIS DOES NOT WORK FOR BOOL VALUES!!!
               // That's why we use string instead of bool in the XML file.
               // =======================================================================
               if (stringstream(token) >> value) {
                 vec.push_back(value);
               }
             });
  }

  return vec;
}

template <typename T>
std::optional<T> read_single_value(tinyxml2::XMLElement* p_element, const string& att_key) {
  // C-style string that will store the attributes read from the XML doc.
  const char* value_cstr;
  // Retrieve the string value into the `value_str` C-style string.
  p_element->QueryStringAttribute(att_key.c_str(), &value_cstr);

  // Separate individual BASIC elements as tokens.
  string str{ value_cstr };
  // outgoing value retrieved from the XML doc.
  T value;
  // Convert string to the BASIC type and add it to the outgoing vector.
  // =======================================================================
  // RT3_WARNING: THIS DOES NOT WORK FOR BOOL VALUES!!!
  // That's why we use string instead of bool in the XML file.
  // =======================================================================
  if (stringstream(str) >> value) {
    return value;
  }  // return std::nullopt;  // Null optional.
  return {};  // Null optional.
}

}  // namespace rt3
