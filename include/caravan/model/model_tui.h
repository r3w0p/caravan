// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TUI_H
#define CARAVAN_MODEL_TUI_H

#include "caravan/model/model.h"
#include "caravan/core/common.h"

class ModelTUI : public Model {
public:
    explicit ModelTUI() : Model() {};

    void run() override;
};

#endif //CARAVAN_MODEL_TUI_H
