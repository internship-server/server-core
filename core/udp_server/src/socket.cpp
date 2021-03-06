//
// socket.cpp
//

#include "socket.hpp"

namespace core::udp {

core::ObjectPool<Socket::read_io_data, 614> Socket::r_io_data_pool_;
core::ObjectPool<Socket::write_io_data, 78> Socket::w_io_data_pool_;

Socket::Socket(Endpoint &endpoint)
    : endpoint_(endpoint)
    , iocp_(nullptr)
    , is_with_iocp_(false)
{
    socket_ = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ::bind(socket_, (const sockaddr*)&endpoint_.Addr(),
        sizeof(endpoint_.Addr()));
}

Socket::Socket(Endpoint &endpoint, HANDLE iocp)
    : endpoint_(endpoint)
    , iocp_(iocp)
{
    socket_ = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ::bind(socket_, (const sockaddr*)&endpoint_.Addr(),
        sizeof(endpoint_.Addr()));
    if (iocp_ != nullptr &&
        CreateIoCompletionPort((HANDLE)socket_, iocp_, (ULONG_PTR)this, 0)) {
        is_with_iocp_ = true;
    }
}

Socket::Socket()
    : iocp_(nullptr)
    , is_with_iocp_(false)
{
    socket_ = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
}

Socket::Socket(HANDLE iocp)
{
    socket_ = ::socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (iocp_ != nullptr &&
        CreateIoCompletionPort((HANDLE)socket_, iocp_, (ULONG_PTR)this, 0)) {
        is_with_iocp_ = true;
    }
}

void Socket::Recv()
{
    /* TODO */
    if (!is_with_iocp_) return;

    unsigned long flags = 0;

    read_io_data* _read_io_data = new(r_io_data_pool_.Malloc()) read_io_data();

    WSARecvFrom(socket_, &(_read_io_data->wsa_buf), 1, &received_bytes, &flags,
        (sockaddr*)&(_read_io_data->remote_endpoint.Addr()),
        &(_read_io_data->from_len), &(_read_io_data->overlapped), NULL);
}

void Socket::Send(Packet &packet, Endpoint &remote_endpoint, bool immediately)
{
    /* TODO */
    if (!is_with_iocp_) return;

    unsigned long flags = 0;

    write_io_data* _write_io_data = new(w_io_data_pool_.Malloc())
        write_io_data(packet);

    WSASendTo(socket_, &(_write_io_data->wsa_buf), 1, nullptr, flags,
        (sockaddr*)&remote_endpoint.Addr(), sizeof(remote_endpoint.Addr()),
        &(_write_io_data->overlapped), nullptr);
}
}