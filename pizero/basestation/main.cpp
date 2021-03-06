/*
********************************************************************************
** @file   pizero/basestation/main.c
** @author eden barby
** @date   1st may 2016
** @brief  
********************************************************************************
*/


/* includes *******************************************************************/

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>

#include <cstdlib>
#include <cstring>

/* scope declarations *********************************************************/

using boost::asio::ip::tcp;

/* private typedef ************************************************************/
/* private define *************************************************************/

#define EVER ;;

/* private macro **************************************************************/
/* private variables **********************************************************/

const int max_length = 1024;

/* private function prototypes ************************************************/

void session(boost::shared_ptr<tcp::socket> sock);
void server(boost::asio::io_service& io_service, unsigned short port);


/*
** @brief  none
** @param  none
** @retval none
*/
int main(int argc, char **argv)
{
    try {
        if(argc != 2) {
            std::cerr << "Usage: blocking_tcp_echo_server <port>\n";
            return 1;
        }

        boost::asio::io_service io_service;

        server(io_service, std::atoi(argv[1]));
    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

void session(boost::shared_ptr<tcp::socket> sock)
{
    try {
        for(EVER) {
            char data[max_length];

            boost::system::error_code error;

            size_t length = sock->read_some(boost::asio::buffer(data), error);

            if(error == boost::asio::error::eof) {
                break; // Connection closed cleanly by peer.
            } else if(error) {
                throw boost::system::system_error(error); // Some other error.
            }

            boost::asio::write(*sock, boost::asio::buffer(data, length));
        }
    } catch(std::exception& e) {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

void server(boost::asio::io_service& io_service, unsigned short port)
{
    tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));

    for(EVER) {
        boost::shared_ptr<tcp::socket> sock(new tcp::socket(io_service));

        a.accept(*sock);

        boost::thread t(boost::bind(session, sock));
    }
}