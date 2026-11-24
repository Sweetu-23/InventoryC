#include "router.hpp"

#include "util.hpp"

namespace inventory {

void Router::add(std::unique_ptr<Controller> controller) {
    controllers_.push_back(std::move(controller));
}

std::string Router::help() const {
    std::string text = "commands:\n";
    for (const auto& controller : controllers_) {
        text += "  " + controller->usage() + "\n";
    }
    text += "  help | quit\n";
    return text;
}

Result Router::dispatch(const std::string& line) const {
    std::vector<std::string> words = tokenize(line);
    if (words.empty()) {
        return {true, ""};
    }
    if (words[0] == "help") {
        return {true, help()};
    }
    for (const auto& controller : controllers_) {
        if (controller->name() == words[0]) {
            words.erase(words.begin());
            return controller->handle(words);
        }
    }
    return {false, "unknown command '" + words[0] + "'. type help"};
}

}  // namespace inventory
