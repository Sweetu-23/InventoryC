#include <iostream>
#include <memory>
#include <string>

#include "inventory_service.hpp"
#include "order_controller.hpp"
#include "product_controller.hpp"
#include "repository.hpp"
#include "router.hpp"
#include "stock_controller.hpp"

int main() {
    using namespace inventory;

    ProductRepository products;
    OrderRepository orders;
    InventoryService service(products, orders);

    Router router;
    router.add(std::make_unique<ProductController>(service));
    router.add(std::make_unique<StockController>(service));
    router.add(std::make_unique<OrderController>(service));

    std::cout << "inventory manager. type help for the commands, quit to leave.\n";
    std::string line;
    while (std::cout << "> " && std::getline(std::cin, line)) {
        if (line == "quit" || line == "exit") {
            break;
        }
        const Result result = router.dispatch(line);
        if (!result.message.empty()) {
            std::cout << (result.ok ? "" : "error: ") << result.message;
            if (result.message.back() != '\n') {
                std::cout << "\n";
            }
        }
    }
    return 0;
}
