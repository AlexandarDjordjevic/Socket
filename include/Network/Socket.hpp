#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory>

#define DEFAULT_CONNECTION_QUEUE_SIZE 10
namespace Network
{
    class Socket
    {
    public:
        /**
         * @brief This is the socket domain types
         * 
         */
        enum struct Domain
        {
            D_UNSPEC,
            D_LOCAL = 1,
            D_UNIX = 1,
            D_FILE = 1,
            D_INET = 2,
            D_AX25,
            D_IPX,
            D_APPLETALK,
            D_NETROM,
            D_BRIDGE,
            D_ATMPVC,
            D_X25,
            D_INET6,
            D_ROSE,
            D_DECnet,
            D_NETBEUI,
            D_SECURITY,
            D_KEY,
            D_NETLINK,
            D_ROUTE,
            D_PACKET,
            D_ASH,
            D_ECONET,
            D_ATMSVC,
            D_RDS,
            D_SNA,
            D_IRDA,
            D_PPPOX,
            D_WANPIPE,
            D_LLC,
            D_IB,
            D_MPLS,
            D_CAN,
            D_TIPC,
            D_BLUETOOTH,
            D_IUCV,
            D_RXRPC,
            D_ISDN,
            D_PHONET,
            D_IEEE802154,
            D_CAIF,
            D_ALG,
            D_NFC,
            D_VSOCK,
            D_KCM,
            D_QIPCRTR,
            D_SMC,
            D_MAX,
        };

        /**
         * @brief This is the socket type definition
         * 
         */
        enum struct Type
        {
            SOCK_STREAM = 1,
            SOCK_DGRAM = 2,
            SOCK_RAW = 3
        };

        const static std::string ANY_ADDRES;

        /**
         * @brief Construct a new Socket object
         * 
         */
        Socket();

        /**
         * @brief Construct a new Socket object
         * 
         * @param descriptor Socket file descriptor
         */
        Socket(int descriptor);

        /**
         * @brief Construct a new Socket object
         * 
         * @param[in] Domain This is the domain used for new Socket object
         * @param[in] Type This is the object socket type
         */
        Socket(Domain, Type);

        /**
         * @brief Destroy the Socket object
         * 
         */
        ~Socket();

        Socket(const Socket &) = delete;
        Socket &operator=(const Socket &) = delete;
        Socket(Socket &&) = delete;
        Socket &operator=(const Socket &&) = delete;

        /**
         * @brief This is the function for creating the socket
         * 
         * @return true if Operation was successful
         * @return false if Operation was unsuccessful
         */
        bool create();

        /**
         * @brief This is the function for creating a socket
         * 
         * @param domain Socket domain
         * @param type Socket type (Stream / Datagram)
         * @return true peration was successful
         * @return false Operation was unsuccessful
         */
        bool create(Domain domain, Type type);

        /**
         * @brief Set the port
         * 
         * @param port Port number
         */
        void setPort(uint16_t port);

        /**
         * @brief This function return socket port number
         * 
         * @return uint16_t port number
         */
        uint16_t getPort();

        /**
         * @brief This function is used to establish connection between two network nodes
         * 
         * @param address Remote node ip address
         * @param port Remote node port
         * @return true connected successfuly
         * @return false fail to connect
         */
        bool connect(const std::string &address, uint16_t port);

        /**
         * @brief This function is used to establish connection between two network nodes
         * 
         * @param address Remote node ip address (numeric representation)
         * @param port Remote node port
         * @return true connected successfuly
         * @return false fail to connect
         */
        bool connect(uint32_t address, uint16_t port);

        /**
         * @brief This is the function for binding the socket
         * 
         * @param port Bind port number
         * @param address Bind address (numeric representation)
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool bind(const std::string &address, uint16_t port);

        /**
         * @brief This is the function for binding the socket to the address 0.0.0.0(INADDR_ANY)
         * 
         * @param port Bind port number
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool bind(uint16_t port);

        /**
         * @brief Set socket into listen state and ready to accept incoming connections.
         *        Maximum queue for incoming connection is default(10)
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool listen();

        /**
         * @brief Set socket into listen state and ready to accept incoming connections.
         *
         * @param backlog maximum length to which the queue of pending connections may grow.
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool listen(size_t backlog);

        /**
         * @brief Set the non blocking mode for the socket
         * 
         * @param blocking Blocking flag
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool setBlockingMode(bool blocking);

        /**
         * @brief This is the function for accepting incoming connections
         * 
         * @return int incoming connection socket descriptor
         */
        int accept();

        /**
         * @brief This is the function for accepting incoming connections
         * 
         * @param client Object to capture socket data
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool accept(Socket *client);

        /**
         * @brief This is the function for writing data(binary) on socket
         * 
         * @param data Data to be written
         * @param length Length of the data
         * 
         * @return  true if operation was successfly, otherwise return false
         */
        bool write(const uint8_t *data, size_t length);

        /**
         * @brief This is the function for writing string on socket
         * 
         * @param data String to be written
         * @return true Operation data was successful
         * @return false Operation data was unsuccessful 
         */
        bool write(const std::string &data);

        /**
         * @brief This is the function for reading data from socket
         * 
         * @param buffer Data storing buffer
         * @param length Data buffer size
         * 
         * @return size_t Number of stored data into data buffer
         */
        size_t read(uint8_t *buffer, const size_t length);

        /**
         * @brief This function shall receive message from remote node.
         * 
         * @param buffer This is the buffer for storing message
         * @param length This is the buffer length
         * 
         * @return size_t Number of received bytes
         */
        size_t receiveFrom(uint8_t *buffer, const size_t length, Socket *socket);

        /**
         * @brief This function return socket file descriptor
         * 
         * @return int socket file descriptor
         */
        int getDescriptor() const;

        /**
         * @brief This function return socket ip represented as string
         * 
         * @return std::string address represented as string
         */
        std::string getIp();

        /**
         * @brief This function return socket ip address as numeric representation
         * 
         * @return uint32_t address value
         */
        uint32_t getIpNumeric();

        /**
         * @brief This function will cause all or part of a full-duplex connection on the
         *        socket associated with the file descriptor socket to be shut down.
         * 
         */
        void shutdown();

        /**
         * @brief This function will dealocate socket file
         * 
         */
        void close();

    private:
        struct impl;
        std::unique_ptr<impl> pimpl;
    };

} // namespace Network
