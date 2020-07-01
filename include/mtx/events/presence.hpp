#pragma once

#include <nlohmann/json.hpp>

#include <optional>
#include <string>
#include <string_view>

namespace mtx {
namespace presence {
enum PresenceState
{
        online,
        offline,
        unavailable,
};

std::string
to_string(PresenceState state);
PresenceState
from_string(std::string_view str);
}

namespace events {
namespace presence {
struct Presence
{
        std::string avatar_url;  //! The current avatar URL for this user, if any.
        std::string displayname; //! The current display name for this user, if any.
        uint64_t
          last_active_ago; //! The last time since this used performed some action, in milliseconds.
        mtx::presence::PresenceState presence; //! Required. The presence state for this user. One
                                               //! of: ["online", "offline", "unavailable"]
        bool currently_active;                 //! Whether the user is currently active
        std::string status_msg; //! An optional description to accompany the presence.
};

void
from_json(const nlohmann::json &obj, Presence &presence);
void
to_json(nlohmann::json &obj, const Presence &presence);
}
}
}
