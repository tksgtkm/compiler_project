#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

std::vector<std::string_view> split(std::string_view str, std::string_view delim) {
  std::vector<std::string_view> out;

  std::string_view::size_type start = 0;
  std::string_view::size_type end   = str.find(delim);

  while (end != std::string_view::npos) {
    out.push_back(str.substr(start, end - start));

    start = end + delim.length();
    end = str.find(delim, start);
  }

  out.push_back(str.substr(start, end - start));

  return out;
}

std::string_view trim(std::string_view str) {
  auto isspace = [] (auto c) {
    return std::isspace(c);
  };

  auto start = std::find_if_not(str.begin(), str.end(), isspace);
  auto end = std::find_if_not(str.rbegin(), str.rend(), isspace).base();

  return {start, std::string_view::size_type(end - start)};
}

std::string toLowerCase(std::string_view str) {
  std::string out;

  for (char c : str) {
    out.push_back(std::tolower(c));
  }

  return out;
}

std::string fix_pointer(std::string_view field) {
  std::ostringstream out;
  std::string_view type = split(field, " ")[0];
  std::string_view name = split(field, " ")[1];
  bool close_bracket = false;

  if (type.substr(0, 12) == "std::vector<") {
    out << "std::vector<";
    type = type.substr(12, type.length() - 13);
    close_bracket = true;
  }

  if (type.back() == '*') {
    type.remove_suffix(1);
    out << "std::shared_ptr<" << type << ">";
  } else {
    out << type;
  }

  if (close_bracket)
    out << ">";

  out << " " << name;

  return out.str();
}

void defineVisitor(std::ofstream& writer, std::string_view baseName, const std::vector<std::string_view>& types) {
  writer << "strict " << baseName << "Visitor {\n";

  for (std::string_view type : types) {
    std::string_view typeName = trim(split(type, ":")[0]);
    writer << " virtual std::any visit" << typeName << baseName << 
    "(std::shared_ptr<" << typeName << "> " << toLowerCase(baseName) << ") = 0;\n";
  }

  writer << " virtual ~" << baseName << "Visitor() = default;\n";
  writer << "};\n";
}