// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_TABLE_H
#define CARAVAN_MODEL_TABLE_H

#include <array>
#include <cstdint>
#include "caravan/model/caravan.h"

class Table {
protected:
    Caravan *a = new Caravan(CARAVAN_A);
    Caravan *b = new Caravan(CARAVAN_B);
    Caravan *c = new Caravan(CARAVAN_C);
    Caravan *d = new Caravan(CARAVAN_D);
    Caravan *e = new Caravan(CARAVAN_E);
    Caravan *f = new Caravan(CARAVAN_F);

    std::array<Caravan *, TABLE_CARAVANS_MAX> caravans = {a, b, c, d, e, f};

public:
    explicit Table() = default;

    ~Table();

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
