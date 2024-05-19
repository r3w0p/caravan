// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_TUI_H
#define CARAVAN_VIEW_TUI_H

#include <string>
#include "caravan/model/game.h"
#include "caravan/user/user.h"
#include "caravan/view/view.h"

class TUI : public View {
public:
    explicit TUI(User *utop, User *ubottom) :
        View(utop, ubottom) {};

    void run() override;
};

#endif //CARAVAN_VIEW_TUI_H
