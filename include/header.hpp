// Copyright 2018 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <iostream>
#include <boost/asio.hpp>
#include <boost/date_time.hpp>
#include <boost/thread.hpp>
#include <chrono>
#include <thread>
#include <unistd.h>

#define BOOST_ASIO_SEPARATE_COMPILATION


class Client {
public:
    Client() : ep(boost::asio::ip::address::
                  from_string("127.0.0.1"), 1024), sock(service) {
        sock.connect(ep);
        getLogin();


        mainLoop();
    }

private:
    void mainLoop() {
        std::thread pingThread(pinging, &sock, &ep);
        while (true) {
            std::string s;
            std::cout << "entering: ";
            std::cin >> s;
            s.push_back('\n');
            sock.write_some(boost::asio::buffer(s));
            sock.read_some(boost::asio::buffer(s));
            if (s == "bye\n") {
                sock.close();
                std::cout << "GOOD BYE!" << std::endl;
                return;
            }
            std::cout << "answer: " << s << std::endl;
        }
    }

    static void pinging(boost::asio::ip::tcp::socket *sock,
                        boost::asio::ip::tcp::endpoint *ep) {
        const std::string PING = "ping\n";
        while (true) {
            sleep(std::rand() % 8);
            try {
                sock->write_some(boost::asio::buffer(PING));
            } catch (...) {
                return;
            }
        }
    }

    void getLogin() {
        std::string answer;
        while (answer != LOGIN_OK) {
            std::cout << "Your login is: " << login << std::endl;
            login.push_back('\n');
            sock.write_some(boost::asio::buffer(login));
            char c[32];
            sock.read_some(boost::asio::buffer(c));
            answer = c;
            std::cout << "answer: '" << answer << "'" << std::endl;
        }
    }

public:
    std::string login = "Dellvin";

private:
    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep;
    boost::asio::ip::tcp::socket sock;
    const std::string LOGIN_OK = "login_ok\n";
};


#endif // INCLUDE_HEADER_HPP_
