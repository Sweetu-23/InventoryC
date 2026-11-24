#pragma once

#include <memory>
#include <string>
#include <vector>

#include "controller.hpp"

namespace inventory {

// Finds the controller named by the first word of a command line and hands it the rest.
class Router {
public:
    void add(std::unique_ptr<Controller> controller);
    Result dispatch(const std::string& line) const;
    std::string help() const;

private:
    std::vector<std::unique_ptr<Controller>> controllers_;
};

}  // namespace inventory
