#pragma once

#include <SFML/Network.hpp>
#include <vector>
#include <cstdint>
#include <iostream>
#include "RecursosT.h"

class sfml_network : public network_api
{

    bool sendUDPData(sf::UdpSocket &socket, const std::vector<uint8_t> &data, const sf::IpAddress &address, unsigned short port)
    {
        sf::Packet packet;
        for (uint8_t byte : data)
        {
            packet << byte;
        }
        return socket.send(packet, address, port) == sf::Socket::Done;
    }

    bool receiveUDPData(sf::UdpSocket &socket, std::vector<uint8_t> &data, sf::IpAddress &sender, unsigned short &port)
    {
        sf::Packet packet;
        if (socket.receive(packet, sender, port) != sf::Socket::Done)
        {
            return false;
        }

        data.clear();
        uint8_t byte;
        while (!packet.endOfPacket())
        {
            packet >> byte;
            data.push_back(byte);
        }

        return true;
    }

    void startServer(unsigned short port)
    {
        sf::UdpSocket serverSocket;
        if (serverSocket.bind(port) != sf::Socket::Done)
        {
            std::cerr << "Failed to bind the server socket to the port." << std::endl;
            return;
        }

        std::vector<uint8_t> receivedData;
        sf::IpAddress sender;
        unsigned short senderPort;
        if (receiveUDPData(serverSocket, receivedData, sender, senderPort))
        {
            std::cout << "Received data: ";
            for (auto byte : receivedData)
            {
                std::cout << static_cast<int>(byte) << " ";
            }
            std::cout << std::endl;
        }
    }

    void startClient(const sf::IpAddress &serverAddress, unsigned short serverPort)
    {
        sf::UdpSocket clientSocket;
        if (clientSocket.bind(sf::Socket::AnyPort) != sf::Socket::Done)
        {
            std::cerr << "Failed to bind the client socket." << std::endl;
            return;
        }

        std::vector<uint8_t> dataToSend = {1, 2, 3, 4, 5};
        if (sendUDPData(clientSocket, dataToSend, serverAddress, serverPort))
        {
            std::cout << "Data sent successfully." << std::endl;
        }
    }
};
