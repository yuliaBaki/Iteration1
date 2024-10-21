#pragma once

#include <unordered_map>
#include <memory>
#include "../state/State.h"
#include "../state/transition/TransitionState.h"


namespace RessourceManager {
    class StateManager
    {
    public:
        StateManager() = delete;

        /*
            create a state with given parameter
            if state already exist, return the existing state and ignore the param.

            note: I don't know why you would get an existing state in the current use.

            FAQ to myself: Why does this class exist? It follow the same exact shape as other manager
            and those manager tends to be singleton for on disk data. State isn't. Yet it need memory
            management of some sort. Keeping the same structure as other my help not forgetting how to use
            it.

            to consider: adding some debut print for get with message: "already exist, returning the existing instance of state"
                         only for debug mode.
        */

        template <typename T, typename... Args>
        static std::shared_ptr<T> create(Args... args) {
            std::shared_ptr<T> p_T = std::make_shared<T>(args...);
            p_states.insert({ p_T->getId(), p_T });
            return p_T;
        }

        template <typename T, typename... Args>
        static std::shared_ptr<TransitionState<T> > create_transition(Game* parent, std::shared_ptr<State> current, std::unique_ptr<Transition>& transition_ptr, Args... args) {

            std::shared_ptr<TransitionState<T>> transition = std::make_shared<TransitionState<T> >(parent, current, transition_ptr, args...);
            p_states.insert({ transition->getId(), transition });
            return transition;
        }

        static std::shared_ptr<TransitionState<State> > create_transition(Game* parent, std::shared_ptr<State> current, std::shared_ptr<State> next, std::unique_ptr<Transition>& transition_ptr) {

            std::shared_ptr<TransitionState<State>> transition = std::make_shared<TransitionState<State> >(parent, current, next, transition_ptr);
            p_states.insert({ transition->getId(), transition });
            return transition;
        }

        template <typename T>
        static std::shared_ptr<T> get(const std::string id) {
            const auto i = p_states.find(id);
            if (i != p_states.end())
                return std::reinterpret_pointer_cast<T>(i->second);
            return nullptr;
        }

        static bool there_exists(const std::string& id) {
            const auto i = p_states.find(id);
            bool exist = false;
            if (i != p_states.end())
                exist = true;

            return exist;
        }

        static void add(std::shared_ptr<State> state) {
            p_states[state->getId()] = state;
        }

        static void removeUnused();

    private:
        static std::unordered_map<std::string, std::shared_ptr<State>> p_states;
    };

}
