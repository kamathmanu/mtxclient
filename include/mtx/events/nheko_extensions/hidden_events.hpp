#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "mtx/events.hpp"

namespace mtx {
namespace events {
namespace account_data {
namespace nheko_extensions {
//! Custom event to hide certain event types in nheko
struct HiddenEvents
{
        //! The hidden event types
        std::vector<EventType> hidden_event_types;
};

void
from_json(const nlohmann::json &obj, HiddenEvents &content);

void
to_json(nlohmann::json &obj, const HiddenEvents &content);
}
} // namespace account_data
} // namespace events
} // namespace mtx