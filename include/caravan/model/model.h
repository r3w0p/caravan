// Copyright (c) 2022-2024 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_MODEL_H
#define CARAVAN_MODEL_H

#include <string>
#include "caravan/core/common.h"

class ModelSubscriber {
public:
    virtual void on_model_change_me() = 0;
};

class Model : public Publisher<ModelSubscriber> {
protected:
    bool closed;
public:
    virtual ~Model() = default;
    explicit Model() : closed(false) {};

    virtual void run() = 0;
    void subscribe(ModelSubscriber *sub) override;
    void close() { closed = true; };
};



#endif //CARAVAN_MODEL_H
