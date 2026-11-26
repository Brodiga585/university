#include "services/Pizzeria.hpp"

#include <sstream>
#include <stdexcept>
#include <utility>

Pizzeria::Pizzeria(std::string name, Address address)
    : name_(std::move(name)), address_(std::move(address)), hall_("Main hall", 40) {
    if (name_.empty()) {
        throw std::invalid_argument("Pizzeria name must not be empty");
    }
    hall_.addTable(1, 2, "Window");
    hall_.addTable(2, 4, "Main hall");
    hall_.addTable(3, 4, "Main hall");
    hall_.addTable(4, 6, "Terrace");

    delivery_.addZone(DeliveryZone("City center",
                                   address_,
                                   0.0,
                                   10.0,
                                   Money::fromRubles(99.0),
                                   Money::fromRubles(20.0),
                                   20,
                                   3.0,
                                   Money::fromRubles(300.0)));
}

const std::string& Pizzeria::name() const {
    return name_;
}

const Address& Pizzeria::address() const {
    return address_;
}

Menu& Pizzeria::menu() {
    return menu_;
}

const Menu& Pizzeria::menu() const {
    return menu_;
}

Hall& Pizzeria::hall() {
    return hall_;
}

const Hall& Pizzeria::hall() const {
    return hall_;
}

InventoryService& Pizzeria::inventory() {
    return inventory_;
}

const InventoryService& Pizzeria::inventory() const {
    return inventory_;
}

OrderService& Pizzeria::orders() {
    return orders_;
}

const OrderService& Pizzeria::orders() const {
    return orders_;
}

KitchenService& Pizzeria::kitchen() {
    return kitchen_;
}

const KitchenService& Pizzeria::kitchen() const {
    return kitchen_;
}

DeliveryService& Pizzeria::delivery() {
    return delivery_;
}

const DeliveryService& Pizzeria::delivery() const {
    return delivery_;
}

PaymentService& Pizzeria::payments() {
    return payments_;
}

const PaymentService& Pizzeria::payments() const {
    return payments_;
}

Chef& Pizzeria::hireChef(std::string fullName,
                         std::string phone,
                         std::string email,
                         const Money& monthlySalary,
                         Chef::SkillLevel skillLevel,
                         std::string specialty) {
    auto employee = std::make_unique<Chef>(std::move(fullName), std::move(phone),
                                           std::move(email), monthlySalary,
                                           std::time(nullptr), skillLevel,
                                           std::move(specialty));
    Chef* result = employee.get();
    employees_.push_back(std::move(employee));
    return *result;
}

Waiter& Pizzeria::hireWaiter(std::string fullName,
                             std::string phone,
                             std::string email,
                             const Money& monthlySalary,
                             int maxTables) {
    auto employee = std::make_unique<Waiter>(std::move(fullName), std::move(phone),
                                             std::move(email), monthlySalary,
                                             std::time(nullptr), maxTables);
    Waiter* result = employee.get();
    employees_.push_back(std::move(employee));
    return *result;
}

Cashier& Pizzeria::hireCashier(std::string fullName,
                               std::string phone,
                               std::string email,
                               const Money& monthlySalary) {
    auto employee = std::make_unique<Cashier>(std::move(fullName), std::move(phone),
                                              std::move(email), monthlySalary,
                                              std::time(nullptr));
    Cashier* result = employee.get();
    employees_.push_back(std::move(employee));
    payments_.setCashier(result);
    return *result;
}

Courier& Pizzeria::hireCourier(std::string fullName,
                               std::string phone,
                               std::string email,
                               const Money& monthlySalary,
                               Courier::Vehicle vehicle,
                               std::string transportNumber) {
    auto employee = std::make_unique<Courier>(std::move(fullName), std::move(phone),
                                              std::move(email), monthlySalary,
                                              std::time(nullptr), vehicle,
                                              std::move(transportNumber));
    Courier* result = employee.get();
    employees_.push_back(std::move(employee));
    delivery_.registerCourier(result);
    return *result;
}

Employee* Pizzeria::findEmployee(const std::string& fullName) {
    for (auto& employee : employees_) {
        if (employee->fullName() == fullName) {
            return employee.get();
        }
    }
    return nullptr;
}

const Employee* Pizzeria::findEmployee(const std::string& fullName) const {
    for (const auto& employee : employees_) {
        if (employee->fullName() == fullName) {
            return employee.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Employee>>& Pizzeria::employees() const {
    return employees_;
}

Customer& Pizzeria::registerCustomer(std::string fullName,
                                     std::string phone,
                                     std::string email,
                                     std::optional<Address> address) {
    if (findCustomer(fullName) != nullptr) {
        throw std::invalid_argument("Customer already registered: " + fullName);
    }
    customers_.push_back(std::make_unique<Customer>(
        std::move(fullName), std::move(phone), std::move(email), std::move(address)));
    return *customers_.back();
}

Customer* Pizzeria::findCustomer(const std::string& fullName) {
    for (auto& customer : customers_) {
        if (customer->fullName() == fullName) {
            return customer.get();
        }
    }
    return nullptr;
}

const Customer* Pizzeria::findCustomer(const std::string& fullName) const {
    for (const auto& customer : customers_) {
        if (customer->fullName() == fullName) {
            return customer.get();
        }
    }
    return nullptr;
}

const std::vector<std::unique_ptr<Customer>>& Pizzeria::customers() const {
    return customers_;
}

Reservation& Pizzeria::bookTable(int tableNumber,
                                 Customer& customer,
                                 int guests,
                                 std::time_t startTime,
                                 int durationMinutes) {
    return hall_.book(tableNumber, customer.fullName(), customer.phone(), guests,
                      startTime, durationMinutes);
}

Review& Pizzeria::addReview(Customer& customer,
                            int rating,
                            std::string comment,
                            Entity::Id orderId) {
    reviews_.emplace_back(customer.fullName(), rating, std::move(comment), orderId);
    return reviews_.back();
}

Complaint& Pizzeria::fileComplaint(Customer& customer,
                                   Complaint::Category category,
                                   std::string description,
                                   Entity::Id orderId) {
    complaints_.emplace_back(customer.fullName(), category, std::move(description), orderId);
    return complaints_.back();
}

const std::vector<Review>& Pizzeria::reviews() const {
    return reviews_;
}

const std::vector<Complaint>& Pizzeria::complaints() const {
    return complaints_;
}

double Pizzeria::averageRating() const {
    if (reviews_.empty()) {
        return 0.0;
    }
    int total = 0;
    for (const auto& review : reviews_) {
        total += review.rating();
    }
    return static_cast<double>(total) / static_cast<double>(reviews_.size());
}

std::size_t Pizzeria::openComplaints() const {
    std::size_t count = 0;
    for (const auto& complaint : complaints_) {
        if (complaint.isOpen()) {
            ++count;
        }
    }
    return count;
}

std::string Pizzeria::dashboard() const {
    std::ostringstream output;
    output << name_ << " at " << address_.format() << '\n'
           << "Menu: " << menu_.itemCount() << " items, version " << menu_.version() << '\n'
           << "Hall: " << hall_.tableCount() << " tables, " << hall_.totalSeats()
           << " seats, " << hall_.activeReservations().size() << " active reservations\n"
           << "Staff: " << employees_.size() << ", customers: " << customers_.size() << '\n'
           << "Orders: " << orders_.count() << " (" << orders_.activeOrders().size()
           << " active), kitchen queue: " << kitchen_.queuedCount()
           << ", deliveries: " << delivery_.activeAssignments() << " active\n"
           << "Stock value: " << inventory_.stockValue().toString()
           << ", low-stock items: " << inventory_.lowStock().size() << '\n'
           << "Revenue: " << payments_.revenue().toString() << " from "
           << payments_.paymentsCount() << " payments\n"
           << "Rating: " << averageRating() << "/5, open complaints: "
           << openComplaints();
    return output.str();
}
