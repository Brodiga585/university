#ifndef ONLINE_PAYMENT_HPP
#define ONLINE_PAYMENT_HPP

#include "finance/Payment.hpp"

#include <string>

class OnlinePayment : public Payment {
public:
    OnlinePayment(Money amount, std::string provider, std::string account, bool confirmed = true);

    const std::string& provider() const;
    const std::string& account() const;
    const std::string& transactionId() const;
    bool isConfirmed() const;
    void setConfirmed(bool confirmed);

    std::string method() const override;
    void pay() override;

private:
    std::string provider_;
    std::string account_;
    std::string transactionId_;
    bool confirmed_;
};

#endif
