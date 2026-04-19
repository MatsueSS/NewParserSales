#ifndef USER_STATE_MACHINE_H
#define USER_STATE_MACHINE_H

#include <unordered_map>
#include <string>

class UserStateMachine{
public:
    enum UserAction{
        ADD_CARD,
        DEL_CARD,
        FORECAST,
        HAS_DISCOUNT,
        NONE
    };

    template<typename Data>
    void set_waiting(Data&& id, UserAction action) noexcept;

    template<typename Data>
    UserAction get_waiting(Data&& id) const noexcept;

    template<typename Data>
    void clear(Data&& id) noexcept;

    template<typename Data>
    bool is_waiting(Data&& id) const noexcept;

    UserStateMachine() = default;

    UserStateMachine(const UserStateMachine&) = default;
    UserStateMachine& operator=(const UserStateMachine&) = default;

    UserStateMachine(UserStateMachine&&) noexcept = default;
    UserStateMachine& operator=(UserStateMachine&&) noexcept = default;

private:
    std::unordered_map<std::string, UserAction> waiting_for_input;

};

template<typename Data>
void UserStateMachine::set_waiting(Data&& id, UserAction action) noexcept
{
    waiting_for_input[std::forward<Data>(id)] = action;
}

template<typename Data>
UserStateMachine::UserAction UserStateMachine::get_waiting(Data&& id) const noexcept
{
    auto it = waiting_for_input.find(std::forward<Data>(id));
    return it == waiting_for_input.end() ? NONE : it->second;
}

template<typename Data>
void UserStateMachine::clear(Data&& id) noexcept
{
    waiting_for_input.erase(std::forward<Data>(id));
}

template<typename Data>
bool UserStateMachine::is_waiting(Data&& id) const noexcept
{
    return waiting_for_input.count(std::forward<Data>(id));
}

#endif // USER_STATE_MACHINE_H