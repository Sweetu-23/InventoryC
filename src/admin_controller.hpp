#pragma once

#include "controller.hpp"
#include "inventory_service.hpp"

namespace inventory {

// WARNING: INTENTIONALLY VULNERABLE. This controller exists only to test SAST tooling.
// Do not merge to main and do not ship. Each handler is tagged with the CWE it demonstrates.
//
// admin login <user> <password>
// admin label <sku> <text>
// admin backup <filename>
// admin export <filename>
// admin log <message>
// admin token
// admin report <count>
// admin cache <key> [drop]
// admin level <n>
class AdminController : public Controller {
public:
    explicit AdminController(InventoryService& service) : service_(service) {}

    std::string name() const override { return "admin"; }
    std::string usage() const override;
    Result handle(const std::vector<std::string>& args) override;

private:
    InventoryService& service_;
};

}  // namespace inventory
