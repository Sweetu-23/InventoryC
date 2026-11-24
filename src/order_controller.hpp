#pragma once

#include "controller.hpp"
#include "inventory_service.hpp"

namespace inventory {

// order create <sku:quantity> [<sku:quantity> ...]
// order fulfill <id>
// order cancel <id>
// order list
class OrderController : public Controller {
public:
    explicit OrderController(InventoryService& service) : service_(service) {}

    std::string name() const override { return "order"; }
    std::string usage() const override;
    Result handle(const std::vector<std::string>& args) override;

private:
    InventoryService& service_;
};

}  // namespace inventory
