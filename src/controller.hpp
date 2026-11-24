#pragma once

#include <string>
#include <vector>

#include "models.hpp"

namespace inventory {

// A controller owns one group of commands. `args` is what comes after the group name, for example for
// "product add SKU-1 Widget 5 2 1999" the product controller gets: add SKU-1 Widget 5 2 1999.
class Controller {
public:
    virtual ~Controller() = default;
    virtual std::string name() const = 0;
    virtual std::string usage() const = 0;
    virtual Result handle(const std::vector<std::string>& args) = 0;
};

}  // namespace inventory
