// Copyright (c) 2022 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_H
#define CARAVAN_VIEW_H

#include "model.h"

class View {};

class ViewCLI : public View {
public:
    void refresh_view(Table t, std::string msg);
    std::string prompt_next_move(std::string msg);
};

#endif //CARAVAN_VIEW_H
