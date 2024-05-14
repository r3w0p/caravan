// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include "caravan/core/exceptions.h"

std::string CaravanException::what() {
    return message;
}
