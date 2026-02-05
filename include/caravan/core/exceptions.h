// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_EXCEPTIONS_H
#define CARAVAN_CORE_EXCEPTIONS_H

#include <string>
#include <utility>

namespace Caravan {
    /*
     * BASE
     */

    class CaravanException : public std::exception {
        std::string message;

        public:
            explicit
            CaravanException(std::string msg) : message(std::move(msg)) {}

            std::string what();
    };

    /*
     * GENERAL
     */

    class CaravanFatalException : public CaravanException {
        public:
            explicit CaravanFatalException(std::string msg) : CaravanException(
                std::move(msg)
            ) {}
    };

    class CaravanIllegalException : public CaravanException {
        public:
            explicit
            CaravanIllegalException(std::string msg) : CaravanException(
                std::move(msg)
            ) {}
    };

    /*
     * MODEL
     */

    class CaravanFatalModelException : public CaravanFatalException {
        public:
            explicit CaravanFatalModelException(
                const std::string &msg
            ) : CaravanFatalException(msg) {}
    };

    class CaravanIllegalModelException : public CaravanIllegalException {
        public:
            explicit CaravanIllegalModelException(
                const std::string &msg
            ) : CaravanIllegalException(msg) {}
    };

    /*
     * VIEW
     */

    class CaravanFatalViewException : public CaravanFatalException {
        public:
            explicit CaravanFatalViewException(
                const std::string &msg
            ) : CaravanFatalException(msg) {}
    };

    class CaravanIllegalViewException : public CaravanIllegalException {
        public:
            explicit CaravanIllegalViewException(
                const std::string &msg
            ) : CaravanIllegalException(msg) {}
    };

    /*
     * CONTROLLER
     */

    class CaravanFatalControllerException : public CaravanFatalException {
        public:
            explicit CaravanFatalControllerException(
                const std::string &msg
            ) : CaravanFatalException(msg) {}
    };

    class CaravanIllegalControllerException : public CaravanIllegalException {
        public:
            explicit CaravanIllegalControllerException(
                const std::string &msg
            ) : CaravanIllegalException(msg) {}
    };
}

#endif //CARAVAN_CORE_EXCEPTIONS_H