// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include "model.h"

class View {
public:
    virtual void display(Game* g, std::string msg) = 0;
};

class ViewCLI : public View {
public:
    void display(Game* g, std::string msg);
};

#endif //CARAVAN_VIEW_H
