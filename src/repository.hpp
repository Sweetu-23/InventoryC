#pragma once

#include <map>
#include <optional>
#include <string>
#include <vector>

#include "models.hpp"

namespace inventory {

// Products and orders live in memory. A database would replace these two classes and nothing else.
class ProductRepository {
public:
    int add(Product product);  // gives the product its id and returns it
    std::optional<Product> find_by_id(int id) const;
    std::optional<Product> find_by_sku(const std::string& sku) const;
    bool update(const Product& product);
    std::vector<Product> all() const;

private:
    std::map<int, Product> products_;
    int next_id_ = 1;
};

class OrderRepository {
public:
    int add(Order order);
    std::optional<Order> find(int id) const;
    bool update(const Order& order);
    std::vector<Order> all() const;

private:
    std::map<int, Order> orders_;
    int next_id_ = 1;
};

}  // namespace inventory
