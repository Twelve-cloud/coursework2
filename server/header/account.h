#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

enum class Role
{
    BROKER,
    CONSULTANT,
    USER
};

class Account
{
private:
    std::string m_login;
    std::string m_password;
    std::string m_mobilePhone;
    Role m_role;
public:
    Account() {}
    Account(const std::string& login, const std::string& password, const std::string& mobilePhone, Role role = Role::USER) : m_login(login), m_password(password), m_mobilePhone(mobilePhone), m_role(role) {}

    std::string getLogin() const noexcept { return m_login; }
    std::string getPassword() const noexcept { return m_password; }
    std::string getMobilePhone() const noexcept { return m_mobilePhone; }
    std::string getRoleString() const noexcept
    {
        switch(m_role)
        {
            case Role::BROKER: return "Broker";
            case Role::CONSULTANT: return "Consultant";
            case Role::USER: return "User";
        }
    }
    Role getRole() const noexcept { return m_role; }

    void setLogin(const std::string& login) { m_login = login; }
    void setPassword(const std::string& password) { m_password = password; }
    void setMobilePhone(const std::string mobilePhone) { m_mobilePhone = mobilePhone; }
    void setRole(const Role& role) { m_role = role; }
};

#endif // ACCOUNT_H
