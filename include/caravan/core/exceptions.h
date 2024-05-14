// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_EXCEPTIONS_H
#define CARAVAN_CORE_EXCEPTIONS_H

#include <string>

class CaravanException : public std::exception {
private:
    std::string message;
public:
    explicit CaravanException(std::string msg) : message(msg) {}

    std::string what();
};

class CaravanFatalException : public CaravanException {
public:
    explicit CaravanFatalException(std::string msg) : CaravanException(msg) {}
};

class CaravanGameException : public CaravanException {
public:
    explicit CaravanGameException(std::string msg) : CaravanException(msg) {}
};

#endif //CARAVAN_CORE_EXCEPTIONS_H
