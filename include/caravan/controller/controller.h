// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_CONTROLLER_H
#define CARAVAN_CONTROLLER_CONTROLLER_H

template <typename T, typename U>
class Controller {
public:
    virtual U convert(const T &input, bool confirmed) = 0;
};

#endif //CARAVAN_CONTROLLER_CONTROLLER_H
