#pragma once

#include <nlohmann/json.hpp>

#include "mtx/events/messages/image.hpp"
#include "mtx/identifiers.hpp"

using json = nlohmann::json;

namespace mtx {
namespace events {

enum class EventType
{
        /// m.key.verification.cancel
        KeyVerificationCancel,
        /// m.key.verification.request
        KeyVerificationRequest,
        /// m.key.verification.start
        KeyVerificationStart,
        /// m.key.verification.accept
        KeyVerificationAccept,
        /// m.key.verification.key
        KeyVerificationKey,
        /// m.key.verification.mac
        KeyVerificationMac,
        /// m.room_key
        RoomKey,
        /// m.room_key_request
        RoomKeyRequest,
        /// m.room.aliases
        RoomAliases,
        /// m.room.avatar
        RoomAvatar,
        /// m.room.canonical_alias
        RoomCanonicalAlias,
        /// m.room.create
        RoomCreate,
        /// m.room.encrypted.
        RoomEncrypted,
        /// m.room.encryption.
        RoomEncryption,
        /// m.room.guest_access
        RoomGuestAccess,
        /// m.room.history_visibility
        RoomHistoryVisibility,
        /// m.room.join_rules
        RoomJoinRules,
        /// m.room.member
        RoomMember,
        /// m.room.message
        RoomMessage,
        /// m.room.name
        RoomName,
        /// m.room.power_levels
        RoomPowerLevels,
        /// m.room.topic
        RoomTopic,
        /// m.room.redaction
        RoomRedaction,
        /// m.room.pinned_events
        RoomPinnedEvents,
        /// m.room.tombstone
        RoomTombstone,
        // m.sticker
        Sticker,
        // m.tag
        Tag,
        // m.push_rules
        PushRules,
        // Unsupported event
        Unsupported,
};

std::string
to_string(EventType type);

EventType
getEventType(const std::string &type);

EventType
getEventType(const json &obj);

//! The basic set of fields all events must have.
template<class Content>
struct Event
{
        //! The fields in this object will vary depending on the type of event.
        //! When interacting with the REST API, this is the HTTP body.
        Content content;
        //! The type of event.
        //! This *should* be namespaced similar to Java package
        //! naming conventions e.g. 'com.example.subdomain.event.type'
        EventType type;
};

template<class Content>
void
to_json(json &obj, const Event<Content> &event)
{
        obj["content"] = event.content;

        switch (event.type) {
        case EventType::KeyVerificationStart:
                obj["type"] = "m.key.verification.start";
                break;
        case EventType::KeyVerificationAccept:
                obj["type"] = "m.key.verification.accept";
                break;
        case EventType::KeyVerificationMac:
                obj["type"] = "m.key.verification.mac";
                break;
        case EventType::KeyVerificationKey:
                obj["type"] = "m.key.verification.key";
                break;
        case EventType::KeyVerificationCancel:
                obj["type"] = "m.key.verification.cancel";
                break;
        case EventType::KeyVerificationRequest:
                obj["type"] = "m.key.verification.request";
                break;
        case EventType::RoomKey:
                obj["type"] = "m.room_key";
                break;
        case EventType::RoomKeyRequest:
                obj["type"] = "m.room_key_request";
                break;
        case EventType::RoomAliases:
                obj["type"] = "m.room.aliases";
                break;
        case EventType::RoomAvatar:
                obj["type"] = "m.room.avatar";
                break;
        case EventType::RoomCanonicalAlias:
                obj["type"] = "m.room.canonical_alias";
                break;
        case EventType::RoomCreate:
                obj["type"] = "m.room.create";
                break;
        case EventType::RoomEncrypted:
                obj["type"] = "m.room.encrypted";
                break;
        case EventType::RoomEncryption:
                obj["type"] = "m.room.encryption";
                break;
        case EventType::RoomGuestAccess:
                obj["type"] = "m.room.guest_access";
                break;
        case EventType::RoomHistoryVisibility:
                obj["type"] = "m.room.history_visibility";
                break;
        case EventType::RoomJoinRules:
                obj["type"] = "m.room.join_rules";
                break;
        case EventType::RoomMember:
                obj["type"] = "m.room.member";
                break;
        case EventType::RoomMessage:
                obj["type"] = "m.room.message";
                break;
        case EventType::RoomName:
                obj["type"] = "m.room.name";
                break;
        case EventType::RoomPowerLevels:
                obj["type"] = "m.room.power_levels";
                break;
        case EventType::RoomTopic:
                obj["type"] = "m.room.topic";
                break;
        case EventType::RoomRedaction:
                obj["type"] = "m.room.redaction";
                break;
        case EventType::RoomPinnedEvents:
                obj["type"] = "m.room.pinned_events";
                break;
        case EventType::RoomTombstone:
                obj["type"] = "m.room.tombstone";
                break;
        case EventType::Sticker:
                obj["type"] = "m.sticker";
                break;
        case EventType::Tag:
                obj["type"] = "m.tag";
                break;
        case EventType::PushRules:
                obj["type"] = "m.push_rules";
                break;
        case EventType::Unsupported:
        default:
                break;
        }
}

template<class Content>
void
from_json(const json &obj, Event<Content> &event)
{
        event.content = obj.at("content").get<Content>();
        event.type    = getEventType(obj.at("type").get<std::string>());
}

//! Extension of the Event type for device events.
template<class Content>
struct DeviceEvent : public Event<Content>
{
        std::string sender;
};

template<class Content>
void
from_json(const json &obj, DeviceEvent<Content> &event)
{
        event.content = obj.at("content").get<Content>();
        event.type    = getEventType(obj.at("type").get<std::string>());
        event.sender  = obj.at("sender");
}

template<class Content>
void
to_json(json &obj, const DeviceEvent<Content> &event)
{
        Event<Content> base_event = event;
        to_json(obj, base_event);

        obj["sender"] = event.sender;
}

struct UnsignedData
{
        //! The time in milliseconds that has elapsed since the event was sent.
        //! This field is generated by the local homeserver,
        //! and may be incorrect if the local time on at least one
        //! of the two servers is out of sync, which can cause the age to
        //! either be negative or greater than it actually is.
        uint64_t age = 0;
        //! The client-supplied transaction ID, if the client
        //! being given the event is the same one which sent it.
        std::string transaction_id;
        std::string prev_sender;
        std::string replaces_state;
        //! The event ID that redacted this event.
        std::string redacted_by;
};

inline void
from_json(const json &obj, UnsignedData &data)
{
        if (obj.find("age") != obj.end())
                data.age = obj.at("age").get<uint64_t>();

        if (obj.find("transaction_id") != obj.end())
                data.transaction_id = obj.at("transaction_id").get<std::string>();

        if (obj.find("prev_sender") != obj.end())
                data.prev_sender = obj.at("prev_sender").get<std::string>();

        if (obj.find("replaces_state") != obj.end())
                data.replaces_state = obj.at("replaces_state").get<std::string>();

        if (obj.find("redacted_by") != obj.end())
                data.redacted_by = obj.at("redacted_by").get<std::string>();
}

inline void
to_json(json &obj, const UnsignedData &event)
{
        if (!event.prev_sender.empty())
                obj["prev_sender"] = event.prev_sender;

        if (!event.transaction_id.empty())
                obj["transaction_id"] = event.transaction_id;

        if (!event.replaces_state.empty())
                obj["replaces_state"] = event.replaces_state;

        if (event.age != 0)
                obj["age"] = event.age;
}

template<class Content>
struct StrippedEvent : public Event<Content>
{
        std::string sender;
        std::string state_key;
};

template<class Content>
void
from_json(const json &obj, StrippedEvent<Content> &event)
{
        event.content   = obj.at("content").get<Content>();
        event.type      = getEventType(obj.at("type").get<std::string>());
        event.sender    = obj.at("sender");
        event.state_key = obj.at("state_key");
}

template<class Content>
void
to_json(json &obj, const StrippedEvent<Content> &event)
{
        Event<Content> base_event = event;
        to_json(obj, base_event);

        obj["sender"]    = event.sender;
        obj["state_key"] = event.state_key;
}

//! RoomEvent.
template<class Content>
struct RoomEvent : public Event<Content>
{
        //! The globally unique event identifier.
        std::string event_id;
        //! The ID of the room associated with this event.
        std::string room_id;
        //! Contains the fully-qualified ID of the user who sent this event.
        std::string sender;
        //! Timestamp in milliseconds on originating homeserver
        //! when this event was sent.
        uint64_t origin_server_ts;
        // SPEC_BUG: The contents of unsigned_data are also present as top level keys.
        //! Contains optional extra information about the event.
        UnsignedData unsigned_data;
};

template<class Content>
void
from_json(const json &obj, RoomEvent<Content> &event)
{
        event.content = obj.at("content").get<Content>();
        event.type    = getEventType(obj.at("type").get<std::string>());

        event.event_id         = obj.at("event_id");
        event.sender           = obj.at("sender");
        event.origin_server_ts = obj.at("origin_server_ts");

        // SPEC_BUG: Not present in the state array returned by /sync.
        if (obj.find("room_id") != obj.end())
                event.room_id = obj.at("room_id");

        if (obj.find("unsigned") != obj.end())
                event.unsigned_data = obj.at("unsigned");
}

template<class Content>
void
to_json(json &obj, const RoomEvent<Content> &event)
{
        Event<Content> base_event = event;
        to_json(obj, base_event);

        if (!event.room_id.empty())
                obj["room_id"] = event.room_id;

        obj["event_id"]         = event.event_id;
        obj["sender"]           = event.sender;
        obj["unsigned"]         = event.unsigned_data;
        obj["origin_server_ts"] = event.origin_server_ts;
}

//! Extension of the RoomEvent.
template<class Content>
struct StateEvent : public RoomEvent<Content>
{
        //! A unique key which defines the overwriting semantics
        //! for this piece of room state.
        std::string state_key;
};

template<class Content>
void
to_json(json &obj, const StateEvent<Content> &event)
{
        RoomEvent<Content> base_event = event;
        to_json(obj, base_event);

        obj["state_key"] = event.state_key;
}

template<class Content>
void
from_json(const json &obj, StateEvent<Content> &event)
{
        event.content          = obj.at("content").get<Content>();
        event.event_id         = obj.at("event_id");
        event.origin_server_ts = obj.at("origin_server_ts");
        event.sender           = obj.at("sender");
        event.type             = getEventType(obj.at("type").get<std::string>());

        if (obj.find("room_id") != obj.end())
                event.room_id = obj.at("room_id");

        if (obj.find("unsigned") != obj.end())
                event.unsigned_data = obj.at("unsigned");

        event.state_key = obj.at("state_key").get<std::string>();
}

//! Extension of the RoomEvent.
template<class Content>
struct RedactionEvent : public RoomEvent<Content>
{
        //! The event id of the event that was redacted.
        std::string redacts;
};

template<class Content>
void
to_json(json &obj, const RedactionEvent<Content> &event)
{
        RoomEvent<Content> base_event = event;
        to_json(obj, base_event);

        obj["redacts"] = event.redacts;
}

template<class Content>
void
from_json(const json &obj, RedactionEvent<Content> &event)
{
        event.content          = obj.at("content").get<Content>();
        event.event_id         = obj.at("event_id");
        event.origin_server_ts = obj.at("origin_server_ts");
        event.sender           = obj.at("sender");
        event.type             = getEventType(obj.at("type").get<std::string>());

        if (obj.find("unsigned") != obj.end())
                event.unsigned_data = obj.at("unsigned");

        if (obj.find("room_id") != obj.end())
                event.room_id = obj.at("room_id");

        event.redacts = obj.at("redacts").get<std::string>();
}

//! Extension of the RoomEvent.
template<class Content>
struct EncryptedEvent : public RoomEvent<Content>
{};

template<class Content>
void
to_json(json &obj, const EncryptedEvent<Content> &event)
{
        RoomEvent<Content> base_event = event;
        to_json(obj, base_event);
}

template<class Content>
void
from_json(const json &obj, EncryptedEvent<Content> &event)
{
        event.content          = obj.at("content").get<Content>();
        event.event_id         = obj.at("event_id");
        event.origin_server_ts = obj.at("origin_server_ts");
        event.sender           = obj.at("sender");
        event.type             = getEventType(obj.at("type").get<std::string>());

        if (obj.find("unsigned") != obj.end())
                event.unsigned_data = obj.at("unsigned");

        if (obj.find("room_id") != obj.end())
                event.room_id = obj.at("room_id");
}

enum class MessageType
{
        // m.audio
        Audio,
        // m.emote
        Emote,
        // m.file
        File,
        // m.image
        Image,
        // m.location
        Location,
        // m.notice
        Notice,
        // m.text
        Text,
        // m.video
        Video,
        // Unrecognized message type
        Unknown,
};

MessageType
getMessageType(const std::string &type);

MessageType
getMessageType(const json &obj);

struct Sticker : public RoomEvent<mtx::events::msg::StickerImage>
{};

} // namespace events
} // namespace mtx
