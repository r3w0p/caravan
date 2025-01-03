// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TABLE_H
#define CARAVAN_MODEL_TABLE_H

#include <array>
#include <cstdint>
#include <memory>
#include "caravan/model/caravan.h"

class Table {
protected:
    std::unique_ptr<Caravan> caravan_a = std::make_unique<Caravan>(CARAVAN_A);
    std::unique_ptr<Caravan> caravan_b = std::make_unique<Caravan>(CARAVAN_B);
    std::unique_ptr<Caravan> caravan_c = std::make_unique<Caravan>(CARAVAN_C);
    std::unique_ptr<Caravan> caravan_d = std::make_unique<Caravan>(CARAVAN_D);
    std::unique_ptr<Caravan> caravan_e = std::make_unique<Caravan>(CARAVAN_E);
    std::unique_ptr<Caravan> caravan_f = std::make_unique<Caravan>(CARAVAN_F);

    std::array<Caravan *, TABLE_CARAVANS_MAX> caravans = {
        caravan_a.get(),
        caravan_b.get(),
        caravan_c.get(),
        caravan_d.get(),
        caravan_e.get(),
        caravan_f.get()
    };

public:
    explicit Table() = default;

    /**
     * @param cvname The caravan to get.
     * @return Pointer to the caravan.
     * @throws CaravanFatalException Invalid caravan name.
     */
    Caravan *get_caravan(CaravanName cvname);

    /**
     * Remove all cards from the caravan.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     */
    bool clear_caravan(CaravanName cvname, bool check_only = false);

    /**
     * @param cvname A caravan name.
     * @param card A face card.
     * @param pos The position of the numeral card on which to place the face card.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     * @throws CaravanGameException QUEEN not played on latest numeral card in caravan.
     */
    bool play_face_card(CaravanName cvname, Card card, uint8_t pos, bool check_only = false);

    /**
     * @param cvname A caravan name.
     * @param card A numeral card to place in the caravan.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     */
    bool play_numeral_card(CaravanName cvname, Card card, bool check_only = false);
};

#endif //CARAVAN_MODEL_TABLE_H
