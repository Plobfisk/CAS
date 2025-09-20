#ifndef CAS_H
#define CAS_H

#include "types.h"
#include "parser.h"

#include <unordered_map>
#include <string>
#include <optional>
#include <tuple>

class CAS {
public:
    CAS() = default;

    void setVariable(std::string key, number_t value);
    number_t getVariable(std::string key);

    std::tuple<std::string, number_t> calc(std::string eq);
private:
    std::optional<std::string> isVariable(NodeExpr* expr);
private:
    std::unordered_map<std::string, number_t> m_varTable;
};

#endif