#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace inventory {

struct Product {
    int id = 0;
    std::string sku;
    std::string name;
    int quantity = 0;
    int reorder_level = 0;
    std::int64_t price_cents = 0;
};

enum class OrderStatus { Open, Fulfilled, Cancelled };

struct OrderLine {
    int product_id = 0;
    int quantity = 0;
};

struct Order {
    int id = 0;
    std::vector<OrderLine> lines;
    OrderStatus status = OrderStatus::Open;
};

// What every service call and controller returns: did it work, and a message for the user.
struct Result {
    bool ok = false;
    std::string message;
};

std::string to_string(OrderStatus status);

}  // namespace inventory
