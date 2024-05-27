// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_VIEW_TUI_H
#define CARAVAN_VIEW_TUI_H

#include "caravan/view/view.h"
#include "caravan/core/common.h"

class ViewTUI : public View {
protected:
    GameCommand parse_user_input(std::string input, bool confirmed);

public:
    explicit ViewTUI(ViewConfig *vc, Game *game) : View(vc, game) {};

    void run() override;

    void close() override;
};

#endif //CARAVAN_VIEW_TUI_H
