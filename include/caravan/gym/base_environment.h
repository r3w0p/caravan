// Copyright (c) 2022-2026 r3w0p
// The following code can be redistributed and/or
// modified under the terms of the GPL-3.0 License.

#ifndef CARAVAN_GYM_BASE_ENVIRONMENT_H
#define CARAVAN_GYM_BASE_ENVIRONMENT_H
#include <tuple>

namespace Caravan::Gym {

    template<typename O, typename A, typename R, typename I>
    class BaseEnvironment {
        public:
            explicit BaseEnvironment() = default;
            virtual ~BaseEnvironment() = default;

            /**
             * Reset the environment.
             * @return Initial observation from reset environment, and any
             *         additional information.
             */
            virtual std::tuple<O, I> reset() = 0;

            /**
             * Sample the action space for the next step.
             * @return Sampled action, and any additional information.
             */
            virtual std::tuple<A, I> sample() = 0;

            /**
             * Take an action on the currently observed environment state.
             *
             * @param action The action to perform.
             * @return A new observation of the environment, the reward for
             *         taking the action, whether the episode is done, and any
             *         additional information.
             */
            virtual std::tuple<O, R, bool, I> step(A &action) = 0;
    };

}

#endif //CARAVAN_GYM_BASE_ENVIRONMENT_H
