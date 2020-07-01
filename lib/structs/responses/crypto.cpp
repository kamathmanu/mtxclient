#include "mtx/responses/crypto.hpp"

#include <nlohmann/json.hpp>

namespace mtx {
namespace responses {

void
from_json(const nlohmann::json &obj, UploadKeys &response)
{
        response.one_time_key_counts =
          obj.at("one_time_key_counts").get<std::map<std::string, uint32_t>>();
}

void
from_json(const nlohmann::json &obj, QueryKeys &response)
{
        response.failures    = obj.at("failures").get<std::map<std::string, nlohmann::json>>();
        response.device_keys = obj.at("device_keys").get<std::map<std::string, DeviceToKeysMap>>();
}

void
from_json(const nlohmann::json &obj, ClaimKeys &response)
{
        response.failures = obj.at("failures").get<std::map<std::string, nlohmann::json>>();
        response.one_time_keys =
          obj.at("one_time_keys")
            .get<std::map<std::string, std::map<std::string, nlohmann::json>>>();
}

void
from_json(const nlohmann::json &obj, KeyChanges &response)
{
        response.changed = obj.at("changed").get<std::vector<std::string>>();
        response.left    = obj.at("left").get<std::vector<std::string>>();
}

namespace backup {
void
from_json(const nlohmann::json &obj, EncryptedSessionData &response)
{
        response.ephemeral  = obj.at("ephemeral");
        response.ciphertext = obj.at("ciphertext");
        response.mac        = obj.at("mac");
}
void
to_json(nlohmann::json &obj, const EncryptedSessionData &response)
{
        obj["ephemeral"]  = response.ephemeral;
        obj["ciphertext"] = response.ciphertext;
        obj["mac"]        = response.mac;
}

void
from_json(const nlohmann::json &obj, SessionBackup &response)
{
        response.first_message_index = obj.at("first_message_index");
        response.forwarded_count     = obj.at("forwarded_count");
        response.is_verified         = obj.at("is_verified");
        response.session_data        = obj.at("session_data");
}
void
to_json(nlohmann::json &obj, const SessionBackup &response)
{
        obj["first_message_index"] = response.first_message_index;
        obj["forwarded_count"]     = response.forwarded_count;
        obj["is_verified"]         = response.is_verified;
        obj["session_data"]        = response.session_data;
}

void
from_json(const nlohmann::json &obj, RoomKeysBackup &response)
{
        response.sessions = obj.at("sessions").get<decltype(response.sessions)>();
}
void
to_json(nlohmann::json &obj, const RoomKeysBackup &response)
{
        obj["sessions"] = response.sessions;
}

void
from_json(const nlohmann::json &obj, KeysBackup &response)
{
        response.rooms = obj.at("rooms").get<decltype(response.rooms)>();
}
void
to_json(nlohmann::json &obj, const KeysBackup &response)
{
        obj["rooms"] = response.rooms;
}

void
from_json(const nlohmann::json &obj, BackupVersion &response)
{
        response.algorithm = obj.at("algorithm");
        response.auth_data = obj.at("auth_data");
        response.count     = obj.at("count");
        response.etag =
          obj.at("etag").dump(); // workaround, since synapse 1.15.1 and older sends this as integer
        response.version = obj.at("version");
}
void
to_json(nlohmann::json &obj, const BackupVersion &response)
{
        obj["algorithm"] = response.algorithm;
        obj["auth_data"] = response.auth_data;
        obj["count"]     = response.count;
        obj["etag"]      = response.etag;
        obj["version"]   = response.version;
}

void
to_json(nlohmann::json &obj, const SessionData &data)
{
        obj["algorithm"]                       = data.algorithm;
        obj["forwarding_curve25519_key_chain"] = data.forwarding_curve25519_key_chain;
        obj["sender_key"]                      = data.sender_key;
        obj["sender_claimed_keys"]             = data.sender_claimed_keys;
        obj["session_key"]                     = data.session_key;
}

void
from_json(const nlohmann::json &obj, SessionData &data)
{
        data.algorithm = obj.at("algorithm");
        data.forwarding_curve25519_key_chain =
          obj.at("forwarding_curve25519_key_chain")
            .get<decltype(data.forwarding_curve25519_key_chain)>();
        data.sender_key = obj.at("sender_key");
        data.sender_claimed_keys =
          obj.at("sender_claimed_keys").get<decltype(data.sender_claimed_keys)>();
        data.session_key = obj.at("session_key");
}
}
}
}
