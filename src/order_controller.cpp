#include "order_controller.hpp"

#include <utility>

#include "util.hpp"

namespace inventory {

std::string OrderController::usage() const {
    return "order create <sku:quantity>... | order fulfill <id> | order cancel <id> | order list";
}

Result OrderController::handle(const std::vector<std::string>& args) {
    if (args.empty()) {
        return {false, "usage: " + usage()};
    }
    const std::string& action = args[0];

    if (action == "create") {
        std::vector<std::pair<std::string, int>> lines;
        for (std::size_t i = 1; i < args.size(); i++) {
            const std::size_t colon = args[i].rfind(':');
            int quantity = 0;
            if (colon == std::string::npos || colon == 0 || !parse_int(args[i].substr(colon + 1), quantity)) {
                return {false, "each line must look like <sku>:<quantity>, got '" + args[i] + "'"};
            }
            lines.emplace_back(args[i].substr(0, colon), quantity);
        }
        return service_.create_order(lines);
    }

    if (action == "fulfill" || action == "cancel") {
        int id = 0;
        if (args.size() != 2 || !parse_int(args[1], id)) {
            return {false, "usage: order " + action + " <id>"};
        }
        return action == "fulfill" ? service_.fulfill_order(id) : service_.cancel_order(id);
    }

    if (action == "list") {
        const std::vector<Order> orders = service_.orders().all();
        if (orders.empty()) {
            return {true, "no orders yet"};
        }
        std::string text;
        for (const Order& order : orders) {
            text += "order " + std::to_string(order.id) + "  " + to_string(order.status) + "  " +
                    std::to_string(order.lines.size()) + " line(s)\n";
        }
        return {true, text};
    }

    return {false, "unknown order command '" + action + "'. usage: " + usage()};
}

}  // namespace inventory
