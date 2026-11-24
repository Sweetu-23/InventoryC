#include "repository.hpp"

namespace inventory {

int ProductRepository::add(Product product) {
    product.id = next_id_++;
    products_[product.id] = product;
    return product.id;
}

std::optional<Product> ProductRepository::find_by_id(int id) const {
    auto it = products_.find(id);
    if (it == products_.end()) {
        return std::nullopt;
    }
    return it->second;
}

std::optional<Product> ProductRepository::find_by_sku(const std::string& sku) const {
    for (const auto& entry : products_) {
        if (entry.second.sku == sku) {
            return entry.second;
        }
    }
    return std::nullopt;
}

bool ProductRepository::update(const Product& product) {
    auto it = products_.find(product.id);
    if (it == products_.end()) {
        return false;
    }
    it->second = product;
    return true;
}

std::vector<Product> ProductRepository::all() const {
    std::vector<Product> result;
    result.reserve(products_.size());
    for (const auto& entry : products_) {
        result.push_back(entry.second);
    }
    return result;
}

int OrderRepository::add(Order order) {
    order.id = next_id_++;
    orders_[order.id] = order;
    return order.id;
}

std::optional<Order> OrderRepository::find(int id) const {
    auto it = orders_.find(id);
    if (it == orders_.end()) {
        return std::nullopt;
    }
    return it->second;
}

bool OrderRepository::update(const Order& order) {
    auto it = orders_.find(order.id);
    if (it == orders_.end()) {
        return false;
    }
    it->second = order;
    return true;
}

std::vector<Order> OrderRepository::all() const {
    std::vector<Order> result;
    result.reserve(orders_.size());
    for (const auto& entry : orders_) {
        result.push_back(entry.second);
    }
    return result;
}

}  // namespace inventory
