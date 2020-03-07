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
#include <string>

#define BOOST_ASIO_SEPARATE_COMPILATION

using boost::posix_time::ptime;

void connection(const boost::system::error_code &ec);

void pinging(boost::asio::ip::tcp::socket *sock, boost::asio::ip::tcp::endpoint *ep) {
    while(true){
        sleep(std::rand() % 8);
        try {
            sock->write_some(buffer("ping\n"));
        } catch (...) {
            return;
        }
    }
}


int main() {
    boost::asio::io_service service;
    boost::asio::ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 1024);
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer("Alice\n"));
    char c[32];
    sock.read_some(buffer(c));
    std::cout << c;
    std::thread pingThread(pinging, &sock, &ep);
    while (true) {
        char c[32];
        memset(c, '\0', 32);
        std::string s;
        std::cout << "entering: ";
        std::cin >> s;
        s.push_back('\n');
        sock.write_some(buffer(s.c_str()));
        sock.read_some(buffer(s.c_str()));
        if (s == "bye\n") {
            sock.close();
            std::cout << "GOOD BYE!" << std::endl;
            break;
        }
        std::cout << "answer: " << s << std::endl;
    }
    return 0;
}


#endif // INCLUDE_HEADER_HPP_
