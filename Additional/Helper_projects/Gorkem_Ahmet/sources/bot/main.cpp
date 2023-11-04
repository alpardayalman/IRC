#include "Bot.hpp"

/**
 * @brief 
 * 
 * @note: OK: Gelen mesaji parse ile tokenlerine ayir,
 * tokenlerinin icerisinde;
 * 	PRIVMSG ve #channel ismi olanlari ele al,
 * 	Kufur token listesi olustur enum ile,
 * 	if (kosulunun icerisine & kufur1 | kufur2 | kufur3 | kufur4)
 * 		gibi olursa bunu kim yazarsa channel'den kickle.
 * 
 * Ekstra; isminde kufur olanlari da kickle,
 * 	
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int	main( int argc, char **argv )
{
	try
	{
		Bot	ircBot(argc, argv);
		ircBot.start();
	}
	catch (const std::exception& e)
	{
		std::cerr << RED << e.what() << END << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
