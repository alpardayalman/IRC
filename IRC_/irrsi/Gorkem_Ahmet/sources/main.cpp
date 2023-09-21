/**
 * @file main.cpp
 * @author Ahmet KARACA (akaraca), Görkem SEVER (gsever)
 * @brief 
 * @version 0.1
 * @date 2023-09-16
 * 
 * @copyright Copyright (c) 2023
 * 
 * @link https://docs.racket-lang.org/irc/index.html#%28def._%28%28lib._irc%2Fmain..rkt%29._irc-set-nick%29%29
 * @link https://www.irchelp.org/protocol/rfc/chapter6.html
 * @link https://modern.ircdocs.horse/#rplliststart-321
 * @link https://www.rfc-editor.org/rfc/rfc2812#section-3.2.6
 * @link https://ircv3.net/specs/extensions/capability-negotiation.html
 * @link https://dd.ircdocs.horse/refs/commands/part
 * @link https://datatracker.ietf.org/doc/html/rfc2812#section-3.1.5
 * @link https://www.alien.net.au/irc/irc2numerics.html
 * @link https://irssi.org/documentation/help/dcc/
 * @link https://www.youtube.com/watch?v=JpLwB14L2Rk&ab_channel=TallPaulTech
 * @link https://www.youtube.com/watch?v=njKv66miR80&list=PL5tDlMcZytRqvYbDWLoayxAkUcHsOxX_p&index=9
 * @link https://beej.us/guide/bgnet/html/split/
 */
#include "../includes/Server.hpp"

/**
 * @brief Main function. :D
 * 
 * Server sample:	./ircserv <port> <password>
 * Client sample:	irssi -> /connect localhost 8888 asdf
 * 					telnet localhost 8080
 * 					nc localhost  8080
 * 					localhost:8080
 * 
 * If you watch (MacOs): nettop -m tcp
 * 
 * If you connect with 'nc' or 'telnet', you have to write in this order.
 * $> CAP END
 * $> PASS 1
 * $> NICK AHMET
 * $> USER AHMET akaraca localhost :Ahmet Karaca
 * 
 * If you want to transfer file, you must use 'irssi' client;
 * For send;
 * 	/dcc send TEST_ ./Desktop/1.txt
 * For get;
 * 	/dcc get TEST 1.txt /home/akaraca/Desktop
 * 
 * @fn start(): Starting the all server.
 * @fn what(): Writes the meaning of the exception.
 * 
 * @param argc: Server port number.
 * @param argv: Server password.
 * @return int 
 */
int	main( int argc, char **argv )
{
	try
	{
		Server Server(argc, argv);
		Server.start();
	}
	catch (std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_SUCCESS);
	}
}
