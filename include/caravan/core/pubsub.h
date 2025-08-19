// Copyright (c) 2022-2025 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_CORE_PUBSUB_H
#define CARAVAN_CORE_PUBSUB_H

#include <type_traits>
#include <vector>

class CaravanSubscriber {
protected:
    explicit CaravanSubscriber() = default;
};

template <class T>  // TODO T is CaravanSubscriber derivative only
class CaravanPublisher {
protected:
    explicit CaravanPublisher() = default;

public:
    virtual void subscribe(T &subscriber) = 0;
    virtual void unsubscribe(T &subscriber) = 0;
};

#endif //CARAVAN_CORE_PUBSUB_H
