#pragma once

#include "controller.hpp"
#include "inventory_service.hpp"

namespace inventory {

// product add <sku> <name> <quantity> <reorder level> <price in cents>
// product list
// product show <sku>
class ProductController : public Controller {
public:
    explicit ProductController(InventoryService& service) : service_(service) {}

    std::string name() const override { return "product"; }
    std::string usage() const override;
    Result handle(const std::vector<std::string>& args) override;

private:
    InventoryService& service_;
};

}  // namespace inventory
