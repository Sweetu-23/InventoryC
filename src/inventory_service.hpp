#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

#include "models.hpp"
#include "repository.hpp"

namespace inventory {

// All the business rules. Controllers only turn text into calls on this class and results back into text.
class InventoryService {
public:
    InventoryService(ProductRepository& products, OrderRepository& orders)
        : products_(products), orders_(orders) {}

    Result add_product(const std::string& sku, const std::string& name, int quantity, int reorder_level,
                       std::int64_t price_cents);
    Result receive_stock(const std::string& sku, int quantity);
    std::vector<Product> low_stock() const;
    std::int64_t stock_value_cents() const;

    // lines: (sku, quantity). The order is only recorded; stock moves when it is fulfilled.
    Result create_order(const std::vector<std::pair<std::string, int>>& lines);
    Result fulfill_order(int order_id);
    Result cancel_order(int order_id);

    const ProductRepository& products() const { return products_; }
    const OrderRepository& orders() const { return orders_; }

private:
    ProductRepository& products_;
    OrderRepository& orders_;
};

}  // namespace inventory
