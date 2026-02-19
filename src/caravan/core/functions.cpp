// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include <random>

#include <chrono>

#include "caravan/core/exceptions.h"
#include "caravan/model/types.h"

namespace Caravan {
    std::string caravan_name_to_str(
        const Model::CaravanName caravan_name,
        const bool letter_only
    ) {
        switch (caravan_name) {
            case Model::CARAVAN_A:
                return letter_only ? "A" : "Caravan A";
            case Model::CARAVAN_B:
                return letter_only ? "B" : "Caravan B";
            case Model::CARAVAN_C:
                return letter_only ? "C" : "Caravan C";
            case Model::CARAVAN_D:
                return letter_only ? "D" : "Caravan D";
            case Model::CARAVAN_E:
                return letter_only ? "E" : "Caravan E";
            case Model::CARAVAN_F:
                return letter_only ? "F" : "Caravan F";
            default:
                throw CaravanFatalException("Invalid caravan name.");
        }
    }

    std::wstring caravan_name_to_wstr(
        const Model::CaravanName caravan_name,
        const bool letter_only
    ) {
        switch (caravan_name) {
            case Model::CARAVAN_A:
                return letter_only ? L"A" : L"Caravan A";
            case Model::CARAVAN_B:
                return letter_only ? L"B" : L"Caravan B";
            case Model::CARAVAN_C:
                return letter_only ? L"C" : L"Caravan C";
            case Model::CARAVAN_D:
                return letter_only ? L"D" : L"Caravan D";
            case Model::CARAVAN_E:
                return letter_only ? L"E" : L"Caravan E";
            case Model::CARAVAN_F:
                return letter_only ? L"F" : L"Caravan F";
            default:
                throw CaravanFatalException("Invalid caravan name.");
        }
    }

    std::wstring direction_to_wstr(const Model::Direction direction) {
        switch (direction) {
            case Model::ANY:
                return L"ANY";
            case Model::ASCENDING:
                return L"ASC";
            case Model::DESCENDING:
                return L"DES";
            default:
                throw CaravanFatalException("Invalid direction.");
        }
    }

    std::mt19937 generate_seed() {
        std::random_device rd;
        std::seed_seq seq{rd(), rd(), rd(), rd(), rd()};

        return std::mt19937(seq);
    }

    /**
     * @param rank A numeral rank.
     * @return An integer equivalent of the numeral, range: 1-10.
     *
     * @throws CaravanFatalException If a non-numeral rank is provided.
     */
    uint8_t numeral_rank_to_uint8_t(const Model::Rank rank) {
        switch (rank) {
            case Model::ACE:
                return 1;
            case Model::TWO:
                return 2;
            case Model::THREE:
                return 3;
            case Model::FOUR:
                return 4;
            case Model::FIVE:
                return 5;
            case Model::SIX:
                return 6;
            case Model::SEVEN:
                return 7;
            case Model::EIGHT:
                return 8;
            case Model::NINE:
                return 9;
            case Model::TEN:
                return 10;
            default:
                throw CaravanFatalException("Invalid rank.");
        }
    }

    std::wstring rank_to_wstr(const Model::Rank rank, const bool lead) {
        switch (rank) {
            case Model::ACE:
                return lead ? L" A" : L"A";
            case Model::TWO:
                return lead ? L" 2" : L"2";
            case Model::THREE:
                return lead ? L" 3" : L"3";
            case Model::FOUR:
                return lead ? L" 4" : L"4";
            case Model::FIVE:
                return lead ? L" 5" : L"5";
            case Model::SIX:
                return lead ? L" 6" : L"6";
            case Model::SEVEN:
                return lead ? L" 7" : L"7";
            case Model::EIGHT:
                return lead ? L" 8" : L"8";
            case Model::NINE:
                return lead ? L" 9" : L"9";
            case Model::TEN:
                return L"10";
            case Model::JACK:
                return lead ? L" J" : L"J";
            case Model::QUEEN:
                return lead ? L" Q" : L"Q";
            case Model::KING:
                return lead ? L" K" : L"K";
            case Model::JOKER:
                return L"JO";
            default:
                return L"";
        }
    }

    std::wstring suit_to_wstr(const Model::Suit suit) {
        switch (suit) {
            case Model::NO_SUIT:
                return L" ";
            case Model::SPADES:
                return L"♠";
            case Model::CLUBS:
                return L"♣";
            case Model::HEARTS:
                return L"♥";
            case Model::DIAMONDS:
                return L"♦";
            default:
                throw CaravanFatalException("Invalid suit.");
        }
    }

    uint64_t time_milliseconds() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }
}
