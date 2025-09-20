#ifndef CALCULATE_H
#define CALCULATE_H

#include "types.h"
#include "parser.h"
#include <optional>
#include <unordered_map>

namespace calculateExpr {
    number_t eval(NodeExpr* expr, const std::optional<std::unordered_map<std::string, number_t>>& varTable = std::nullopt);
    number_t eval(std::string eq);
}

#endif