#ifndef PIZZERIA_HPP
#define PIZZERIA_HPP

#include "catalog/Menu.hpp"
#include "core/Address.hpp"
#include "people/Cashier.hpp"
#include "people/Chef.hpp"
#include "people/Courier.hpp"
#include "people/Customer.hpp"
#include "people/Waiter.hpp"
#include "quality/Complaint.hpp"
#include "quality/Review.hpp"
#include "services/DeliveryService.hpp"
#include "services/InventoryService.hpp"
#include "services/KitchenService.hpp"
#include "services/OrderService.hpp"
#include "services/PaymentService.hpp"
#include "venue/Hall.hpp"

#include <cstddef>
#include <ctime>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Pizzeria {
public:
    Pizzeria(std::string name, Address address);

    const std::string& name() const;
    const Address& address() const;

    Menu& menu();
    const Menu& menu() const;
    Hall& hall();
    const Hall& hall() const;
    InventoryService& inventory();
    const InventoryService& inventory() const;
    OrderService& orders();
    const OrderService& orders() const;
    KitchenService& kitchen();
    const KitchenService& kitchen() const;
    DeliveryService& delivery();
    const DeliveryService& delivery() const;
    PaymentService& payments();
    const PaymentService& payments() const;

    Chef& hireChef(std::string fullName,
                   std::string phone,
                   std::string email,
                   const Money& monthlySalary,
                   Chef::SkillLevel skillLevel,
                   std::string specialty);
    Waiter& hireWaiter(std::string fullName,
                       std::string phone,
                       std::string email,
                       const Money& monthlySalary,
                       int maxTables = 4);
    Cashier& hireCashier(std::string fullName,
                         std::string phone,
                         std::string email,
                         const Money& monthlySalary);
    Courier& hireCourier(std::string fullName,
                         std::string phone,
                         std::string email,
                         const Money& monthlySalary,
                         Courier::Vehicle vehicle,
                         std::string transportNumber = {});

    Employee* findEmployee(const std::string& fullName);
    const Employee* findEmployee(const std::string& fullName) const;
    const std::vector<std::unique_ptr<Employee>>& employees() const;

    Customer& registerCustomer(std::string fullName,
                               std::string phone,
                               std::string email,
                               std::optional<Address> address = std::nullopt);
    Customer* findCustomer(const std::string& fullName);
    const Customer* findCustomer(const std::string& fullName) const;
    const std::vector<std::unique_ptr<Customer>>& customers() const;

    Reservation& bookTable(int tableNumber,
                           Customer& customer,
                           int guests,
                           std::time_t startTime,
                           int durationMinutes);
    Review& addReview(Customer& customer,
                      int rating,
                      std::string comment,
                      Entity::Id orderId = 0);
    Complaint& fileComplaint(Customer& customer,
                             Complaint::Category category,
                             std::string description,
                             Entity::Id orderId = 0);

    const std::vector<Review>& reviews() const;
    const std::vector<Complaint>& complaints() const;
    double averageRating() const;
    std::size_t openComplaints() const;

    std::string dashboard() const;

private:
    std::string name_;
    Address address_;
    Menu menu_;
    Hall hall_;
    InventoryService inventory_;
    OrderService orders_;
    KitchenService kitchen_;
    DeliveryService delivery_;
    PaymentService payments_;
    std::vector<std::unique_ptr<Employee>> employees_;
    std::vector<std::unique_ptr<Customer>> customers_;
    std::vector<Review> reviews_;
    std::vector<Complaint> complaints_;
};

#endif
