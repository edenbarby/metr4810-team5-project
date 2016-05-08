/*
********************************************************************************
** @file   pizero/rescuedrone/main.c
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
#define PACKET_LENGTH_MAX 1024

/* private macro **************************************************************/
/* private variables **********************************************************/
/* private function prototypes ************************************************/


/*
** @brief  none
** @param  none
** @retval none
*/
int main(int argc, char **argv)
{
    char packet;

    try {
        if(argc != 3) {
            std::cerr << "Usage: " << argv[0] << " <host> <port>" << std::endl;
            return 1;
        }

        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query query(tcp::v4(), argv[1], argv[2]);
        tcp::resolver::iterator iterator = resolver.resolve(query);

        tcp::socket s(io_service);
        boost::asio::connect(s, iterator);

        for(EVER) {
            packet = std::cin.get();

            boost::asio::write(s, boost::asio::buffer(&packet, (size_t)1));

            boost::asio::read(s, boost::asio::buffer(&packet, (size_t)1));

            std::cout << packet << std::endl;
        }

        // std::cout << "Enter message: ";
        // char request[PACKET_LENGTH_MAX];
        // std::cin.getline(request, PACKET_LENGTH_MAX);
        // size_t request_length = std::strlen(request);
        // boost::asio::write(s, boost::asio::buffer(request, request_length));

        // char reply[PACKET_LENGTH_MAX];
        // size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));

        // std::cout << "Reply is: ";
        // std::cout.write(reply, reply_length);
        // std::cout << std::endl;

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}