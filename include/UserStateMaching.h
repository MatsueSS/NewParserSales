#ifndef _USER_STATE_MACHING_H_
#define _USER_STATE_MACHING_H_

#include <unordered_map>
#include <string>

class UserStateMaching{
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

    UserStateMaching() = default;

    UserStateMaching(const UserStateMaching&) = default;
    UserStateMaching& operator=(const UserStateMaching&) = default;

    UserStateMaching(UserStateMaching&&) noexcept = default;
    UserStateMaching& operator=(UserStateMaching&&) noexcept = default;

private:
    std::unordered_map<std::string, UserAction> waiting_for_input;

};

template<typename Data>
void UserStateMaching::set_waiting(Data&& id, UserAction action) noexcept
{
    waiting_for_input[std::forward<Data>(id)] = action;
}

template<typename Data>
UserStateMaching::UserAction UserStateMaching::get_waiting(Data&& id) const noexcept
{
    auto it = waiting_for_input.find(std::forward<Data>(id));
    return it == waiting_for_input.end() ? NONE : it->second;
}

template<typename Data>
void UserStateMaching::clear(Data&& id) noexcept
{
    waiting_for_input.erase(std::forward<Data>(id));
}

template<typename Data>
bool UserStateMaching::is_waiting(Data&& id) const noexcept
{
    return waiting_for_input.count(std::forward<Data>(id));
}

#endif