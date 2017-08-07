//
// packet.hpp
//

#pragma once

// 508 + 4 == 512
#define MAX_PACKET_SIZE 508

#include <memory>

namespace core::udp {
class Packet {
private:
    struct data {
        short size;
        short packet_type;
        char body[MAX_PACKET_SIZE];
    };

public:
    explicit Packet(Packet &&packet);
    explicit Packet(short size, short packet_id, const char* data);
    explicit Packet(const char* data);

    short Size() { return data_->size; }
    short PacketType() { return data_->packet_type; }
    const char* Data() { return data_->body; }

private:
    std::shared_ptr<data> data_;
};
}
