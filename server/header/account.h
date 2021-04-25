#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account
{
public:
    Account(const std::size_t& id, const std::string& login, const std::string& password, const std::string& mobileNumber, const std::string& email, const std::string& role = "USER")
        : m_id(id), m_login(login), m_password(password), m_mobileNumber(mobileNumber), m_email(email), m_role(role) {}

private:
    std::size_t m_id;
    std::string m_login;
    std::string m_password;
    std::string m_mobileNumber;
    std::string m_email;
    std::string m_role;
};

#endif // ACCOUNT_H
