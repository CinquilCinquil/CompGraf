#ifndef PARAMSET_H
#define PARAMSET_H

#include <iostream>
#include <map>
#include <memory>
#include <string>

/// Pure virtual basic type. The map stores a pointer to the base class
class ValueBase {
public:
  ValueBase() = default;
  virtual ~ValueBase() = default;
};

/// We must convert the base class object to the proper derived class object.
template <typename T> class Value : public ValueBase {
private:
  T m_value;  // The stored data.

public:
  /// Ctr. always requires an argument.
  Value(T value) : m_value{ value } {}
  ~Value() override = default;

  /// Copy ctr.
  Value(const Value& src) : m_value{ src.m_value } {}

  /// Assignemt operator
  Value& operator=(const Value& src) {
    if (this != &src) {
      m_value = src.m_value;
    }
    return *this;
  }

  /// Retrieve value.
  T value() { return m_value; }
};

namespace rt3 {
// The ParamSet is just a heterogeneous hash table. All keys are strings.
using ParamSet = std::map<std::string, std::shared_ptr<ValueBase>>;

/*!
 * This is an auxiliary function to avoid the *verbose* access associated
 * with the std::map<>.
 *
 * Tries to retrieve the value associated with `key` from the ParamSet `ps`.
 * In case there is no such key/value pair stored in `ps`, the function returns
 * the default value `default_value` provided by the client him/herself.
 * @param ps The ParamSet we want to extract the value from.
 * @param key The key stored in the ParamSet.
 * @param default_value The default value returned, in case the key is not in
 * the ParamSet.
 * @return The value associated with `key`, if such key is stored in the `ps`,
 * or the provided default value otherwise.
 */
template <typename T>
T retrieve(const ParamSet& ps, const std::string& key, const T& default_value = T{}) {
  // Try to retrieve key/data item from the map.
  auto result = ps.find(key);
  if (result != ps.end()) {
    const auto& [key, sptr] = *result;
    // Alternative ways of converting generic pointer into the desired one.
    auto rval = dynamic_cast<Value<T>&>(*sptr).value();
    /* auto rval2 = dynamic_cast<Value<T>&>(*sptr);
    auto rval3 = dynamic_cast<Value<T>*>(sptr.get());
    Value<T>* rval4 = (Value<T>*)(sptr.get()); */

    std::cout << "--> ParamSet: Found [\"" << key << "\"]\n";
    // std::cout << rval<< ".\n";
    /* std::cout << rval2.value()  << ".\n";
    std::cout << rval3->value() << ".\n";
    std::cout << rval4->value() << ".\n"; */

    // Returns the stored value.
    return rval;
  }
  std::cout << "--> ParamSet: Key [\"" << key << "\"] not present.\n";
  // Assign a default value in case type is not in the ParamSet object.
  return default_value;
}
}  // namespace rt3

#endif
