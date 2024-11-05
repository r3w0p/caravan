// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_CARAVAN_H
#define CARAVAN_MODEL_CARAVAN_H

#include <cstdint>
#include <array>
#include "caravan/model/deck.h"
#include "caravan/core/common.h"


/**
 * A caravan that contains all of the information for a given track of numeral
 * cards and any face cards attached to them, including: the total caravan bid,
 * its direction, and its suit.
 */
class Caravan {

protected:
    CaravanName name;
    Track track;
    uint8_t i_track;

    /**
     * @param rank A numeral rank.
     * @return An integer equivalent of the numeral, range: 1-10.
     *
     * @throws CaravanFatalException If a non-numeral rank is provided.
     */
    static uint8_t numeral_rank_to_uint8_t(Rank rank);

    /**
     * @param index The index of the numeral card to remove from the caravan.
     */
    void remove_numeral_card(uint8_t index);

public:
    /**
     * @param cvname Caravan name.
     */
    explicit Caravan(CaravanName cvname) :
        name(cvname), track({}), i_track(0) {};

    /**
     * @return Caravan bid.
     */
    uint16_t get_bid();

    /**
     * @return Current caravan direction.
     */
    Direction get_direction();

    /**
    * @return Caravan name.
    */
    CaravanName get_name();

    /**
     * @return Current number of numeral cards in caravan.
     */
    uint8_t get_size();

    /**
     * @param pos Caravan position.
     * @return Slot at position.
     * @throws CaravanGameException Chosen card position is out of range.
     */
    Slot get_slot(uint8_t pos);

    /**
     * @return Current caravan suit.
     */
    Suit get_suit();

    /**
     * Remove all cards from the caravan.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     * @throws CaravanGameException Caravan track is empty.
     */
    bool clear(bool check_only = false);

    /**
     * @param card Face card to put into caravan.
     * @param pos Position of numeral card on which to put the face card.
     * @param *target Stores a copy of the numeral card on which the face
     *        card was placed into this pointer.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     *
     * @throws CaravanGameException Caravan position not entered.
     * @throws CaravanGameException No numeral card at chosen position.
     * @throws CaravanGameException Chosen card is not a face card.
     * @throws CaravanGameException Numeral card at maximum face card capacity.
     */
    bool put_face_card(Card card, uint8_t pos, Card *target = nullptr, bool check_only = false);

    /**
     * @param card Numeral card to put into caravan.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     * @throws CaravanGameException Card is not a numeral.
     * @throws CaravanGameException Caravan is at maximum numeral card capacity.
     * @throws CaravanGameException Numeral card has same rank as most recent
     *         card in caravan.
     * @throws CaravanGameException Numeral does not follow caravan direction.
     */
    bool put_numeral_card(Card card, bool check_only = false);

    /**
     * Remove all numeral cards of a given rank.
     *
     * @param rank The rank to remove.
     * @param pos_exclude The numeral card at the position will be excluded from
     *                    removal. If 0, no card is excluded.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     * @throws CaravanFatalException Exclude position is out of range.
     */
    bool remove_rank(Rank rank, uint8_t pos_exclude, bool check_only = false);

    /**
     * Remove all numeral cards of a given suit.
     *
     * @param suit The suit to remove.
     * @param pos_exclude The numeral card at the position will be excluded from
     *                    removal. If 0, no card is excluded.
     * @param check_only If true, only check whether operation is valid;
     *        if false, actually perform the operation.
     * @return True if operation or check was successful; false otherwise.
     * @throws CaravanFatalException Exclude position is out of range.
     */
    bool remove_suit(Suit suit, uint8_t pos_exclude, bool check_only = false);
};

#endif //CARAVAN_MODEL_CARAVAN_H
