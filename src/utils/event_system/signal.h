#pragma once


#include <vector>
#include <functional>
#include <cassert>

#include "signal_reaction.h"

#include <cstring>

template <typename SIGNAL_T>
class SignalDispatcher {
    const char *id;

    bool is_active = true;

    std::vector<SignalReaction<SIGNAL_T>> observers;

    typedef std::function<SIGNAL_T(const SIGNAL_T &)> SignalAffector;
    SignalAffector signal_affector;

public:

    SignalDispatcher(const char *id = "noname_signal") : id(id), signal_affector(nullptr) {}

    ~SignalDispatcher() {
    }

    SignalDispatcher &operator=(const SignalDispatcher &other) = delete;

    void push_observer(const SignalReaction<SIGNAL_T> &observer) {
        observers.push_back(observer);
    }

    void emit(const SIGNAL_T &signal) {
        if (!is_active) return;

        SIGNAL_T affected_signal = signal_affector ? signal_affector(signal) : signal;
        dispatch(affected_signal);
    }

    void dispatch(const SIGNAL_T &signal) {
        if (!is_active) return;

        for (auto &observer : observers) {
            observer(signal);
        }
    }

    const char *get_id() { return id; }

    void set_signal_affector(SignalAffector affector) { signal_affector = affector; }

    inline void pop_observer() {
        if (!observers.size()) return;
        observers.pop_back();
    }

    void clear_observers() {
        while (observers.size()) {
            pop_observer();
        }
    }

    void set_active(bool flag) { is_active = flag; }
};
