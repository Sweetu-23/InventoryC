#include "models.hpp"

namespace inventory {

std::string to_string(OrderStatus status) {
    switch (status) {
        case OrderStatus::Open:
            return "open";
        case OrderStatus::Fulfilled:
            return "fulfilled";
        case OrderStatus::Cancelled:
            return "cancelled";
    }
    return "unknown";
}

}  // namespace inventory
