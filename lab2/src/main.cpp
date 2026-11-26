#include "catalog/Dough.hpp"
#include "catalog/Drink.hpp"
#include "catalog/Ingredient.hpp"
#include "catalog/Menu.hpp"
#include "catalog/MenuCategory.hpp"
#include "catalog/Pizza.hpp"
#include "catalog/PizzaSize.hpp"
#include "catalog/Recipe.hpp"
#include "catalog/Sauce.hpp"
#include "catalog/Topping.hpp"
#include "core/Address.hpp"
#include "core/Money.hpp"
#include "delivery/DeliveryZone.hpp"
#include "finance/CardPayment.hpp"
#include "finance/CashPayment.hpp"
#include "finance/OnlinePayment.hpp"
#include "finance/PromoCode.hpp"
#include "finance/Receipt.hpp"
#include "inventory/StockItem.hpp"
#include "inventory/Supplier.hpp"
#include "inventory/SupplyItem.hpp"
#include "inventory/SupplyOrder.hpp"
#include "ordering/DeliveryOrder.hpp"
#include "ordering/DineInOrder.hpp"
#include "ordering/Order.hpp"
#include "ordering/TakeawayOrder.hpp"
#include "people/Chef.hpp"
#include "people/Courier.hpp"
#include "people/Customer.hpp"
#include "people/Waiter.hpp"
#include "quality/Complaint.hpp"
#include "services/Pizzeria.hpp"
#include "venue/Reservation.hpp"
#include "venue/Table.hpp"

#include <ctime>
#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {
Ingredient makeFlour() {
    return Ingredient("Flour", Ingredient::Unit::Gram, Money::fromRubles(0.08), 3.6, {"Gluten"});
}

Ingredient makeTomato() {
    return Ingredient("Tomato", Ingredient::Unit::Gram, Money::fromRubles(0.15), 0.2);
}

Ingredient makeMozzarella() {
    return Ingredient("Mozzarella", Ingredient::Unit::Gram, Money::fromRubles(0.6), 2.5, {"Milk"});
}

Ingredient makeBasil() {
    return Ingredient("Basil", Ingredient::Unit::Gram, Money::fromRubles(1.0), 0.4);
}

Ingredient makePepperoni() {
    return Ingredient("Pepperoni", Ingredient::Unit::Gram, Money::fromRubles(0.9), 4.0, {}, false);
}

Recipe makeMargheritaRecipe(const Ingredient& flour, const Ingredient& tomato) {
    Recipe recipe("Margherita recipe", 12);
    recipe.addIngredient(flour, 180.0);
    recipe.addIngredient(tomato, 80.0);
    recipe.addStep("Knead the dough and let it rest");
    recipe.addStep("Spread tomato sauce over the base");
    recipe.addStep("Bake until the crust is golden");
    return recipe;
}

Recipe makePepperoniRecipe(const Ingredient& flour, const Ingredient& tomato) {
    Recipe recipe("Pepperoni recipe", 14);
    recipe.addIngredient(flour, 180.0);
    recipe.addIngredient(tomato, 80.0);
    recipe.addStep("Knead the dough and let it rest");
    recipe.addStep("Spread tomato sauce and add toppings");
    recipe.addStep("Bake until the cheese melts");
    return recipe;
}

Menu buildDemoMenu() {
    const Ingredient flour = makeFlour();
    const Ingredient tomato = makeTomato();
    const Ingredient mozzarella = makeMozzarella();
    const Ingredient basil = makeBasil();
    const Ingredient pepperoni = makePepperoni();

    const Dough classicDough(Dough::Type::Classic, "Classic", 1.0);
    const Dough thinDough(Dough::Type::Thin, "Thin", 1.0);
    const Sauce tomatoSauce("Tomato", 0, 1.0, true);
    const Sauce spicySauce("Spicy", 3, 1.1, true);
    const PizzaSize mediumSize("Medium", 30.0, 1.0, 8);
    const PizzaSize largeSize("Large", 35.0, 1.25, 10);

    const Topping mozzarellaTopping(mozzarella, 100.0, Money::fromRubles(60.0));
    const Topping basilTopping(basil, 10.0, Money::fromRubles(30.0));
    const Topping pepperoniTopping(pepperoni, 60.0, Money::fromRubles(90.0));

    auto margherita = std::make_unique<Pizza>(
        "Margherita",
        "Tomato sauce, mozzarella and basil",
        Money::fromRubles(350.0),
        mediumSize,
        classicDough,
        tomatoSauce,
        makeMargheritaRecipe(flour, tomato),
        std::vector<Topping>{mozzarellaTopping, basilTopping});

    auto pepperoniPizza = std::make_unique<Pizza>(
        "Pepperoni",
        "Tomato sauce, mozzarella and pepperoni",
        Money::fromRubles(450.0),
        largeSize,
        thinDough,
        spicySauce,
        makePepperoniRecipe(flour, tomato),
        std::vector<Topping>{mozzarellaTopping, pepperoniTopping});

    auto cola = std::make_unique<Drink>(
        "Cola", "Chilled cola", Money::fromRubles(120.0), 500, "ColaCo", false, 42.0);
    auto lemonade = std::make_unique<Drink>(
        "Lemonade", "House lemonade", Money::fromRubles(150.0), 400, "Pizzeria", false, 25.0);

    MenuCategory pizzas("Pizzas", 1);
    pizzas.addItem(std::move(margherita));
    pizzas.addItem(std::move(pepperoniPizza));

    MenuCategory drinks("Drinks", 2);
    drinks.addItem(std::move(cola));
    drinks.addItem(std::move(lemonade));

    Menu menu;
    menu.addCategory(std::move(pizzas));
    menu.addCategory(std::move(drinks));
    menu.publish("1.0");
    return menu;
}

void printSection(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
}

Receipt buildReceipt(const Order& order, const std::string& paymentMethod, const Money& tip) {
    Receipt receipt("ORD-" + std::to_string(order.id()), 0.0);
    for (const auto& item : order.items()) {
        receipt.addLine(item.name(), item.quantity(), item.unitPrice());
    }
    if (order.extraFee().minorUnits() > 0) {
        receipt.addLine(order.orderType() + " fee", 1, order.extraFee());
    }
    if (order.discount().minorUnits() > 0) {
        receipt.setDiscount(order.discount());
    }
    receipt.setTip(tip);
    receipt.setPaymentMethod(paymentMethod);
    return receipt;
}

void printMenu(const Menu& menu) {
    for (const auto& category : menu.categories()) {
        std::cout << '[' << category.title() << "]\n";
        for (const auto& item : category.items()) {
            std::cout << "  - " << item->describe() << '\n';
        }
    }
}
}

int main() {
    try {
        const Address hub("Moscow", "Tverskaya", "1", "", "101000", 55.7558, 37.6173);
        const Address home("Moscow", "Arbat", "12", "5", "119002", 55.7520, 37.5950);
        const std::time_t now = std::time(nullptr);

        Pizzeria pizzeria("Pizza Roma", hub);
        pizzeria.menu() = buildDemoMenu();

        Chef& chef = pizzeria.hireChef("Marco Rossi", "+7-900-100-10-10",
                                       "marco@pizzaroma.ru", Money::fromRubles(120000.0),
                                       Chef::SkillLevel::HeadChef, "Neapolitan pizza");
        Waiter& waiter = pizzeria.hireWaiter("Anna Petrova", "+7-900-200-20-20",
                                             "anna@pizzaroma.ru", Money::fromRubles(70000.0));
        pizzeria.hireCashier("Olga Smirnova", "+7-900-300-30-30",
                             "olga@pizzaroma.ru", Money::fromRubles(65000.0));
        Courier& courier = pizzeria.hireCourier("Igor Volkov", "+7-900-400-40-40",
                                                "igor@pizzaroma.ru", Money::fromRubles(60000.0),
                                                Courier::Vehicle::Scooter, "S-101");

        Customer& alice = pizzeria.registerCustomer("Alice Ivanova", "+7-900-500-50-50",
                                                    "alice@example.com", home);
        Customer& bob = pizzeria.registerCustomer("Bob Petrov", "+7-900-600-60-60",
                                                  "bob@example.com");
        Customer& cara = pizzeria.registerCustomer("Cara Orlova", "+7-900-700-70-70",
                                                   "cara@example.com");

        const Ingredient flour = makeFlour();
        const Ingredient tomato = makeTomato();
        const Ingredient mozzarella = makeMozzarella();
        const Ingredient basil = makeBasil();
        const Ingredient pepperoni = makePepperoni();

        pizzeria.inventory().addStock(StockItem(flour, 5000.0, 0.0, 1000.0, 0));
        pizzeria.inventory().addStock(
            StockItem(tomato, 3000.0, 0.0, 800.0, now + 5 * 86400));
        pizzeria.inventory().addStock(
            StockItem(mozzarella, 2500.0, 0.0, 700.0, now + 2 * 86400));
        pizzeria.inventory().addStock(StockItem(basil, 500.0, 0.0, 100.0, now + 86400));
        pizzeria.inventory().addStock(
            StockItem(pepperoni, 1800.0, 0.0, 500.0, now + 10 * 86400));

        pizzeria.inventory().addSupplier(Supplier(
            "Farm Co", "Ivan Sokolov", "+7-900-800-80-80", hub,
            {Supplier::Weekday::Monday, Supplier::Weekday::Thursday}, 4.7,
            Money::fromRubles(1000.0)));
        SupplyOrder& supply = pizzeria.inventory().createSupplyOrder(
            "Farm Co", now + 86400);
        supply.addItem(SupplyItem(flour, 20000.0, Money::fromRubles(0.07)));
        supply.addItem(SupplyItem(mozzarella, 5000.0, Money::fromRubles(0.55)));
        supply.send();
        pizzeria.inventory().receiveSupply(supply);

        printSection("Menu");
        printMenu(pizzeria.menu());

        printSection("Dine-in order");
        Table* table = pizzeria.hall().findTable(2);
        Reservation& reservation = pizzeria.bookTable(2, bob, 4, now + 3600, 90);
        reservation.seat();
        table->seat(4);
        waiter.assignTable();

        DineInOrder& dineIn = pizzeria.orders().createDineIn(bob.fullName(), table, 4, 0.1);
        dineIn.addItem(*pizzeria.menu().findItem("Margherita"), 2);
        dineIn.addItem(*pizzeria.menu().findItem("Cola"), 4);
        std::cout << dineIn.describe() << '\n';

        pizzeria.payments().checkout(
            dineIn, std::make_unique<CashPayment>(dineIn.total(), Money::fromRubles(2000.0)));
        pizzeria.kitchen().acceptOrder(dineIn, &chef, 3, 20);
        pizzeria.kitchen().startNext();
        pizzeria.kitchen().markNextReady();
        dineIn.complete();

        waiter.addTip(Money::fromRubles(100.0));
        waiter.releaseTable();
        reservation.complete();
        table->release();

        const Receipt dineReceipt =
            buildReceipt(dineIn, "Cash", Money::fromRubles(100.0));
        std::cout << dineReceipt.format();
        pizzeria.addReview(bob, 5, "Excellent Margherita and fast service", dineIn.id());

        printSection("Delivery order");
        DeliveryOrder& delivery = pizzeria.orders().createDelivery(
            alice.fullName(), home, nullptr, now + 3600);
        delivery.addItem(*pizzeria.menu().findItem("Pepperoni"), 1);
        delivery.addItem(*pizzeria.menu().findItem("Lemonade"), 2);
        const DeliveryZone* deliveryZone = pizzeria.delivery().findZoneFor(home);
        if (deliveryZone == nullptr) {
            throw std::runtime_error("No delivery zone covers the customer address");
        }
        delivery.setZone(deliveryZone);
        PromoCode promo("WELCOME10", 10.0, Money::fromRubles(300.0),
                        now - 3600, now + 7 * 86400, 10);
        delivery.applyPromoCode(promo, now);
        std::cout << delivery.describe() << '\n';

        pizzeria.payments().checkout(
            delivery,
            std::make_unique<CardPayment>(delivery.total(), "4111111111111111",
                                          "ALICE IVANOVA", 12, 2030));
        pizzeria.kitchen().acceptOrder(delivery, &chef, 5, 25);
        pizzeria.kitchen().startNext();
        pizzeria.kitchen().markNextReady();
        pizzeria.delivery().assign(delivery, courier);
        pizzeria.delivery().dispatch(delivery.id());
        pizzeria.delivery().complete(delivery.id());

        std::cout << buildReceipt(delivery, "Card", Money()).format();
        pizzeria.addReview(alice, 4, "Tasty, but the lemonade was warm", delivery.id());

        printSection("Takeaway order");
        TakeawayOrder& takeaway = pizzeria.orders().createTakeaway(
            cara.fullName(), {}, now + 1800);
        takeaway.addItem(*pizzeria.menu().findItem("Margherita"), 1);
        takeaway.addItem(*pizzeria.menu().findItem("Lemonade"), 1);
        pizzeria.payments().checkout(
            takeaway,
            std::make_unique<OnlinePayment>(takeaway.total(), "PayFast",
                                            "cara@example.com"));
        pizzeria.kitchen().acceptOrder(takeaway, &chef, 2, 15);
        pizzeria.kitchen().startNext();
        pizzeria.kitchen().markNextReady();
        while (pizzeria.kitchen().serveNextReady()) {
        }
        takeaway.complete();
        std::cout << buildReceipt(takeaway, "Online", Money()).format();

        Complaint& complaint = pizzeria.fileComplaint(
            alice, Complaint::Category::Delivery,
            "Lemonade was not chilled on arrival", delivery.id());
        complaint.markInReview();
        complaint.resolve("A complimentary drink was added to the next order");
        std::cout << '\n' << complaint.describe() << '\n';

        printSection("Staff");
        for (const auto& employee : pizzeria.employees()) {
            std::cout << "- " << employee->describe() << '\n';
        }
        std::cout << "- courier: " << courier.describe() << '\n';
        std::cout << "- waiter tips: " << waiter.tips().toString() << '\n';

        printSection("Inventory");
        std::cout << pizzeria.inventory().warehouse().audit();
        std::cout << supply.describe() << '\n';
        std::cout << "Low stock items: " << pizzeria.inventory().lowStock().size() << '\n';

        printSection("Quality");
        for (const auto& review : pizzeria.reviews()) {
            std::cout << review.describe() << '\n';
        }

        printSection("Dashboard");
        std::cout << pizzeria.dashboard() << '\n';
        std::cout << "Completed order revenue: "
                  << pizzeria.orders().completedRevenue().toString() << '\n';
        std::cout << "Payment revenue: " << pizzeria.payments().revenue().toString()
                  << ", refunds: " << pizzeria.payments().refundsCount() << '\n';

        printSection("Cancellation and refund");
        TakeawayOrder& cancelled = pizzeria.orders().createTakeaway(
            bob.fullName(), "CANCEL-1", now + 1200);
        cancelled.addItem(*pizzeria.menu().findItem("Cola"), 1);
        pizzeria.payments().checkout(
            cancelled,
            std::make_unique<OnlinePayment>(cancelled.total(), "PayFast",
                                            "bob@example.com"));
        pizzeria.payments().refund(cancelled);
        cancelled.cancel();
        std::cout << cancelled.describe() << '\n';
        std::cout << "Payment revenue after refund: "
                  << pizzeria.payments().revenue().toString()
                  << ", refunds: " << pizzeria.payments().refundsCount() << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Fatal error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
