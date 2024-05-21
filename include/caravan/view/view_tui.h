// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_TUI_H
#define CARAVAN_VIEW_TUI_H

#include "caravan/view/view.h"
#include "caravan/core/common.h"

class ViewTUI : public View {
public:
    explicit ViewTUI(User *utop, User *ubottom) : View(utop, ubottom) {};

    void run() override;
};

#endif //CARAVAN_VIEW_TUI_H
