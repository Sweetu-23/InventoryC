#include "inventory_service.hpp"

namespace inventory {

Result InventoryService::add_product(const std::string& sku, const std::string& name, int quantity,
                                     int reorder_level, std::int64_t price_cents) {
    if (sku.empty() || name.empty()) {
        return {false, "sku and name are required"};
    }
    if (quantity < 0 || reorder_level < 0 || price_cents < 0) {
        return {false, "quantity, reorder level and price cannot be negative"};
    }
    if (products_.find_by_sku(sku)) {
        return {false, "a product with sku " + sku + " already exists"};
    }
    Product product;
    product.sku = sku;
    product.name = name;
    product.quantity = quantity;
    product.reorder_level = reorder_level;
    product.price_cents = price_cents;
    const int id = products_.add(product);
    return {true, "added product " + sku + " (id " + std::to_string(id) + ")"};
}

Result InventoryService::receive_stock(const std::string& sku, int quantity) {
    if (quantity <= 0) {
        return {false, "quantity must be above zero"};
    }
    auto product = products_.find_by_sku(sku);
    if (!product) {
        return {false, "unknown sku " + sku};
    }
    product->quantity += quantity;
    products_.update(*product);
    return {true, "stock of " + sku + " is now " + std::to_string(product->quantity)};
}

std::vector<Product> InventoryService::low_stock() const {
    std::vector<Product> low;
    for (const Product& product : products_.all()) {
        if (product.quantity <= product.reorder_level) {
            low.push_back(product);
        }
    }
    return low;
}

std::int64_t InventoryService::stock_value_cents() const {
    std::int64_t total = 0;
    for (const Product& product : products_.all()) {
        total += static_cast<std::int64_t>(product.quantity) * product.price_cents;
    }
    return total;
}

Result InventoryService::create_order(const std::vector<std::pair<std::string, int>>& lines) {
    if (lines.empty()) {
        return {false, "an order needs at least one line"};
    }
    Order order;
    for (const auto& line : lines) {
        if (line.second <= 0) {
            return {false, "line quantities must be above zero"};
        }
        auto product = products_.find_by_sku(line.first);
        if (!product) {
            return {false, "unknown sku " + line.first};
        }
        order.lines.push_back({product->id, line.second});
    }
    const int id = orders_.add(order);
    return {true, "created order " + std::to_string(id)};
}

Result InventoryService::fulfill_order(int order_id) {
    auto order = orders_.find(order_id);
    if (!order) {
        return {false, "unknown order " + std::to_string(order_id)};
    }
    if (order->status != OrderStatus::Open) {
        return {false, "order " + std::to_string(order_id) + " is " + to_string(order->status)};
    }
    // Check everything first, so a short line never leaves a half-fulfilled order.
    for (const OrderLine& line : order->lines) {
        auto product = products_.find_by_id(line.product_id);
        if (!product || product->quantity < line.quantity) {
            const std::string sku = product ? product->sku : std::to_string(line.product_id);
            return {false, "not enough stock of " + sku};
        }
    }
    for (const OrderLine& line : order->lines) {
        auto product = products_.find_by_id(line.product_id);
        product->quantity -= line.quantity;
        products_.update(*product);
    }
    order->status = OrderStatus::Fulfilled;
    orders_.update(*order);
    return {true, "fulfilled order " + std::to_string(order_id)};
}

Result InventoryService::cancel_order(int order_id) {
    auto order = orders_.find(order_id);
    if (!order) {
        return {false, "unknown order " + std::to_string(order_id)};
    }
    if (order->status != OrderStatus::Open) {
        return {false, "only open orders can be cancelled; order " + std::to_string(order_id) + " is " +
                           to_string(order->status)};
    }
    order->status = OrderStatus::Cancelled;
    orders_.update(*order);
    return {true, "cancelled order " + std::to_string(order_id)};
}

}  // namespace inventory
