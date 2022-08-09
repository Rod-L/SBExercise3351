#include <iostream>
#include <map>
#include "PurchaseExceptions.h"

std::map<std::string,int> database;

int input_product_amount() {
    int amount;
    std::cin >> amount;

    if (!std::cin.good() || amount < 0) {
        std::cin.clear();
        std::cin.ignore(INT32_MAX, '\n');
        throw InvalidAmountException();
    }
    return amount;
}

void fill_assortment() {
    std::string descrStr = "Enter code of the product and amount of it in the shop (enter 'exit' to finish assortment):";
    std::cout << descrStr << std::endl;
    while (true) {
        std::string code;
        std::cin >> code;

        if (code == "exit") return;

        int amount;
        try {
            amount = input_product_amount();
        } catch (InvalidAmountException& exc) {
            std::cout << exc.what() << std::endl;
            std::cout << descrStr << std::endl;
            continue;
        }
        database[code] = amount;
        std::cout << amount << " of product '" << code << "' have been registered in the database." << std::endl;
    }
}

void complete_purchase(std::map<std::string,int>& items) {
    for (auto& it : items) {
        database[it.first] -= it.second;
    }
}

void display_products(std::map<std::string,int>& products) {
    for (auto& it : products) {
        std::cout << it.first << " - " << it.second << std::endl;
    }
}

void fill_basket() {
    std::map<std::string,int> basket;
    std::cout << "Enter code of the product and amount of it to add to the basket.\n"
                 "Enter 'purchase' to finish this transaction and return to main menu.\n"
                 "Enter 'exit' to discard this order and return to main menu.\n";

    while (true) {
        std::string code;
        std::cin >> code;

        if (code == "exit") {
            return;
        }
        if (code == "purchase") {
            std::cout << "Processing order:" << std::endl;
            display_products(basket);
            complete_purchase(basket);
            std::cout << "Success." << std::endl;
            return;
        }

        auto available = database.find(code);
        if (available == database.end()) {
            throw NoSuchProductException(code);
        }

        int amount = input_product_amount();
        int total = amount;
        auto it = basket.find(code);
        if (it == basket.end()) {
            basket[code] = amount;
        } else {
            total += it->second;
            basket[code] = total;
        }

        if (total > available->second) {
            throw NotEnoughProductException(code, total, available->second);
        }

        std::cout << amount << " of product '" << code << "' have been added to order." << std::endl;
    }
}

int main() {
    fill_assortment();

    while (true) {
        std::cout << "Available commands:\n"
                     "1 - make new order\n"
                     "2 - show available products\n"
                     "3 - exit program\n";

        std::string command;
        std::cin >> command;

        switch (command[0]) {
            case '1':
                try {
                    fill_basket();
                } catch (NoSuchProductException& exc) {
                    std::cout << exc.what() << std::endl;
                } catch (NotEnoughProductException& exc) {
                    std::cout << exc.what() << std::endl;
                } catch (...) {
                    std::cout << "Unknown error." << std::endl;
                }
                break;

            case '2':
                std::cout << "Products in shop:" << std::endl;
                display_products(database);
                std::cout << std::endl;
                break;

            case '3':
                return 0;

            default:
                std::cout << "Unknown command." << std::endl;
        }
    }
}
