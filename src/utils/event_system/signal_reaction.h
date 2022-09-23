#pragma once


#include <functional>


template <typename SIGNAL_T>
using SignalReaction = std::function<void (const SIGNAL_T&)>;


template <typename ACCEPTOR_T, typename SIGNAL_T>
class SignalAcceptor : public SignalReaction<SIGNAL_T> {
protected:
    ACCEPTOR_T *acceptor;
public:
    virtual ~SignalAcceptor() = default;
    SignalAcceptor(ACCEPTOR_T *acceptor) : acceptor(acceptor) {}
};
