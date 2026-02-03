// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CONTROLLER_CONTROLLER_STR_TO_MOVE_H
#define CARAVAN_CONTROLLER_CONTROLLER_STR_TO_MOVE_H

#include <string>
#include "caravan/controller/controller.h"
#include "caravan/model/types.h"

namespace Caravan::Controller {

    class ControllerStrToMove : public Controller<std::string, Caravan::Model::GameMove> {
    public:
        Caravan::Model::GameMove convert(const std::string &input, bool confirmed) override;
    };

}
#endif //CARAVAN_CONTROLLER_CONTROLLER_STR_TO_MOVE_H
