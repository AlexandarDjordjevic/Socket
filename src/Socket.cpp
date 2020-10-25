#include <Network/Socket.hpp>
namespace Network
{
    const std::string Socket::ANY_ADDRES = {"0.0.0.0"};

    struct Socket::impl
    {
        int fileDescriptor;
        uint32_t address;
        in_port_t port;
        Socket::Domain domain;
        Socket::Type type;
    };

    Socket::Socket()
        : pimpl(new Socket::impl())
    {
    }

    Socket::Socket(int descriptor)
        : pimpl(new Socket::impl())
    {
        pimpl->fileDescriptor = descriptor;
    }

    Socket::Socket(Domain _domain, Type _type)
        : pimpl(new Socket::impl)
    {
        pimpl->domain = _domain;
        pimpl->type = _type;
        pimpl->fileDescriptor = -1; //!<Invalid socket
    }

    Socket::~Socket()
    {
        shutdown();
        //TODO Add read before closing socket ... test if it is necessary read!
        close();
    }

    bool Socket::create()
    {
        pimpl->fileDescriptor = socket(int(pimpl->domain), int(pimpl->type), 0);
        if (pimpl->fileDescriptor < 0)
            return false;
        return true;
    }

    bool Socket::create(Domain domain, Type type)
    {
        pimpl->domain = domain;
        pimpl->type = type;
        return create();
    }

    void Socket::setPort(uint16_t port)
    {
        pimpl->port = port;
    }

    uint16_t Socket::getPort()
    {
        return pimpl->port;
    }

    bool Socket::connect(const std::string &address, uint16_t port)
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        struct sockaddr_in serv_addr;
        inet_pton(int(pimpl->domain), address.c_str(), &serv_addr.sin_addr);
        return connect(ntohl(serv_addr.sin_addr.s_addr), port);
    }

    bool Socket::connect(uint32_t address, uint16_t port)
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        struct sockaddr_in serv_addr;
        pimpl->address = address;
        pimpl->port = port;
        serv_addr.sin_family = sa_family_t(pimpl->domain);
        serv_addr.sin_addr.s_addr = htonl(pimpl->address);
        serv_addr.sin_port = htons(pimpl->port);
        if (::connect(pimpl->fileDescriptor, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        {
            return false;
        }
        return true;
    }

    bool Socket::bind(const std::string &address, uint16_t port)
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        struct sockaddr_in serv_addr;
        inet_pton(int(pimpl->domain), address.c_str(), &(pimpl->address));
        pimpl->port = port;

        serv_addr.sin_family = sa_family_t(pimpl->domain);
        serv_addr.sin_addr.s_addr = pimpl->address;
        serv_addr.sin_port = htons(pimpl->port);

        auto result = ::bind(pimpl->fileDescriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (result < 0)
            return false;
        return true;
    }

    bool Socket::bind(uint16_t port)
    {
        return bind(ANY_ADDRES, port);
    }

    bool Socket::listen()
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        auto result = ::listen(pimpl->fileDescriptor, DEFAULT_CONNECTION_QUEUE_SIZE);
        if (result < 0)
            return false;
        return true;
    }

    bool Socket::listen(size_t backlog)
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        auto result = ::listen(pimpl->fileDescriptor, backlog);
        if (result < 0)
            return false;
        return true;
    }

    bool Socket::setBlockingMode(bool blocking)
    {
        if (pimpl->fileDescriptor == -1)
            return false;
        int flags = fcntl(pimpl->fileDescriptor, F_GETFL, 0);
        if (flags == -1)
            return false;
        if (blocking)
            flags &= ~O_NONBLOCK;
        else
            flags |= O_NONBLOCK;
        return (fcntl(pimpl->fileDescriptor, F_SETFL, flags) == 0) ? true : false;
    }

    int Socket::accept()
    {
        return ::accept(pimpl->fileDescriptor, NULL, NULL);
    }

    bool Socket::accept(Socket *client)
    {
        struct sockaddr_in cli;
        socklen_t len = 0;
        client->pimpl->fileDescriptor = ::accept(pimpl->fileDescriptor, (struct sockaddr *)&cli, &len);
        if (-1 == client->pimpl->fileDescriptor) return false;
        client->pimpl->address = ntohl(cli.sin_addr.s_addr);
        client->pimpl->port = ntohs(cli.sin_port);
        return true;
    }

    bool Socket::write(const uint8_t *data, size_t length)
    {
        if (pimpl->fileDescriptor < 0)
            return false;
        auto result = ::write(pimpl->fileDescriptor, data, length);
        if (result == -1)
            return false;
        return true;
    }

    bool Socket::write(const std::string &data)
    {
        return write((const uint8_t *)(data.c_str()), data.length());
    }

    size_t Socket::read(uint8_t *buffer, const size_t length)
    {
        return ::read(pimpl->fileDescriptor, buffer, length);
    }

    size_t Socket::receiveFrom(uint8_t *buffer, const size_t length, Socket *client)
    {
        struct sockaddr_in cli;
        socklen_t len = 0;
        auto received_len = ::recvfrom(pimpl->fileDescriptor, buffer, length, 0, (struct sockaddr *)&cli, &len);
        if (client != nullptr)
        {
            client->pimpl->address = cli.sin_addr.s_addr;
            client->pimpl->port = cli.sin_port;
        }
        return received_len;
    }

    int Socket::getDescriptor() const
    {
        return pimpl->fileDescriptor;
    }

    std::string Socket::getIp()
    {
        char str[INET_ADDRSTRLEN] = {0};
        try
        {
            in_addr clientAdd;
            clientAdd.s_addr = pimpl->address;
            inet_ntop(AF_INET, &(clientAdd), str, INET_ADDRSTRLEN);
            return std::string(str);
        }
        catch (...)
        {
            return nullptr;
        }
    }

    uint32_t Socket::getIpNumeric()
    {
        return pimpl->address;
    }

    void Socket::shutdown()
    {
        ::shutdown(pimpl->fileDescriptor, SHUT_RDWR);
    }

    void Socket::close()
    {
        ::close(pimpl->fileDescriptor);
    }
} // namespace Network
