// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <string>
#include "caravan/model/types.h"

namespace Caravan::User {

    std::string caravan_letter(Model::CaravanName cvname) {
        switch (cvname) {
            case Model::CARAVAN_A:
                return "A";
            case Model::CARAVAN_B:
                return "B";
            case Model::CARAVAN_C:
                return "C";
            case Model::CARAVAN_D:
                return "D";
            case Model::CARAVAN_E:
                return "E";
            case Model::CARAVAN_F:
                return "F";
            default:
                return "";
        }
    }

}