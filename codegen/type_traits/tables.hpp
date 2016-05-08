#pragma once

#include <map>
#include <string>
#include <type_traits>

namespace tt {

extern const std::map<std::string, bool> is_floating_point_table;
extern const std::map<std::string, size_t> type_size_table;

}
