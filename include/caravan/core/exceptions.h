// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_EXCEPTIONS_H
#define CARAVAN_CORE_EXCEPTIONS_H

#include <string>
#include <utility>

/*
 * BASE
 */

class CaravanException : public std::exception {
private:
    std::string message;
public:
    explicit CaravanException(std::string msg) :
        message(std::move(msg)) {}

    std::string what();
};

// TODO CORE
// TODO CONTROLLER
// TODO MODEL
// TODO VIEW

// TODO Replace CaravanGameException with GameResponse class?

/*
 * GENERAL
 */

class CaravanFatalException : public CaravanException {
public:
    explicit CaravanFatalException(std::string msg) :
        CaravanException(std::move(msg)) {}
};

class CaravanGameException : public CaravanException {
public:
    explicit CaravanGameException(std::string msg) :
        CaravanException(std::move(msg)) {}
};

/*
 * CONTROLLER
 */

class CaravanInputException : public CaravanException {
public:
    explicit CaravanInputException(const std::string &msg) :
        CaravanException(msg) {}
};

#endif //CARAVAN_CORE_EXCEPTIONS_H
