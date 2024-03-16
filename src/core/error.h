#ifndef ERROR_H
# define ERROR_H

# include <iomanip>  // setw()
# include <iostream>
# include <string>

# define SC               SourceContext(__FILE__, __LINE__)

# define RT3_ERROR(msg)   Error(msg, SC)

# define RT3_WARNING(msg) Warning(msg, SC)

# define RT3_MESSAGE(msg) Message(msg)

namespace rt3 {
// Holds context information for a warning or error while pre-processing scene
// file or executing the renderer. Use macro SC to create one
struct SourceContext {
  const char* file;
  int line;
  SourceContext(const char* file = "n/a", int line = 0) : file(file), line(line) {}
  SourceContext& operator=(const SourceContext&) = default;
  SourceContext(const SourceContext& clone) = default;
  ~SourceContext() = default;
};

/// Prints out the error message and exits the program.
void Error(const std::string&, const SourceContext&);
/// Prints out the warning, but the program keeps running.
void Warning(const std::string&, const SourceContext&);
/// Prints out a simple message, program keeps running.
void Message(const std::string&);
}  // namespace rt3

#endif
//-------------------------------------------------------------------------------
