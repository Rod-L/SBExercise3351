#pragma once

#include <exception>

class InvalidAmountException: public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid amount value, must be non negative number.";
    }
};

class NoSuchProductException: public std::exception {
public:
    std::string required_name;

    explicit NoSuchProductException(std::string& _name): required_name(_name) {};

    const char* what() const noexcept override {
        std::string err;
        err.append(required_name);
        err.append(": no such product in database.");
        return err.c_str();
    }
};

class NotEnoughProductException: public std::exception {
public:
    std::string required_name;
    int required_amount;
    int existing_amount;

    explicit NotEnoughProductException(std::string& _name, int _req, int _exc):
        required_name(_name), required_amount(_req), existing_amount(_exc) {};

    const char* what() const noexcept override {
        std::string err;
        err.append(required_name);
        err.append(": ");
        err.append(std::to_string(required_amount));
        err.append(" requested, ");
        err.append(std::to_string(existing_amount));
        err.append(" existing.");
        return err.c_str();
    }
};