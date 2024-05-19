// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#include "caravan/core/common.h"


bool is_numeral_card(Card c) {
    return (c.rank >= ACE and c.rank <= TEN);
}

bool is_face_card(Card c) {
    return (c.rank >= JACK and c.rank <= JOKER);
}

template<typename T>
void Publisher<T>::subscribe(T *sub) {
    subscribers.push_back(sub);
}
