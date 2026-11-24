#pragma once

#include "controller.hpp"
#include "inventory_service.hpp"

namespace inventory {

// stock receive <sku> <quantity>
// stock low
// stock value
class StockController : public Controller {
public:
    explicit StockController(InventoryService& service) : service_(service) {}

    std::string name() const override { return "stock"; }
    std::string usage() const override;
    Result handle(const std::vector<std::string>& args) override;

private:
    InventoryService& service_;
};

}  // namespace inventory
