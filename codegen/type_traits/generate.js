const fs = require('fs');
const path = require('path');

const types = require('../generators/builtin_types').types;

function genTables() {
    return `#include "tables.hpp"

namespace tt {

  const std::map<std::string, bool> is_floating_point_table{
    ${types.map(t => `{"${t[1]}", std::is_floating_point<${t[1]}>::value}`).join(',\n    ')}
  };
  
  const std::map<std::string, size_t> type_size_table{
    ${types.map(t => `{"${t[1]}", sizeof(${t[1]})}`).join(',\n    ')}
  };

}`;
}

fs.writeFileSync(path.join(__dirname, 'tables.cpp'), genTables());


