# include "../../includes/Server.hpp"

/**
 * @brief Client's first 'handshake' connection steps.
 * 
--CAP LS 302--
IRC client'ten gelen CAP LS 302, IRC serverinin kullanıcılara sunabileceği ek özelliklerin bir listesini içeren bir mesajdır.
	CAP, "Capability" kelimesinin kısaltmasıdır.
	LS, "List" kelimesinin kısaltmasıdır.
	302, ek özelliklerin bir listesinin sunulduğunu belirtir.

CAP LS 302 mesajında yer alan ek özellikler, IRC serverinin işlevselliğini artırabilir.
	Örneğin, serverin kullanıcılarına özel mesajlaşma, dosya paylaşımı veya oyun oynama gibi özellikler sunabilir.


--CAP * LS :multi-prefix sasl--
CAP * LS :multi-prefix sasl mesajı, IRC client'in desteklediği ek özelliklerin bir listesini içeren bir mesajdır.

CAP, "Capability" kelimesinin kısaltmasıdır.
	*, tüm özelliklerin listelenmesini belirtir.
	LS, "List" kelimesinin kısaltmasıdır.
	:multi-prefix sasl, desteklenen ek özelliklerdir.

	multi-prefix: IRC client'in birden fazla önek kullanmasına izin verir.
	sasl: IRC client'in SASL kimlik doğrulama protokolünü desteklediğini belirtir.

--CAP REQ :multi-prefix--
"CAP REQ :multi-prefix" komutu, IRC client'in IRC serverından multi-prefix özelliğini talep ettiğini belirtir.
CAP, "Capability" kelimesinin kısaltmasıdır.
	REQ, "Request" kelimesinin kısaltmasıdır.
	:multi-prefix, talep edilen özelliktir.

--CAP * ACK multi-prefix--
"CAP * ACK multi-prefix" mesajı, IRC serverının sahibi veya yöneticisinin, IRC client'in talep ettiği multi-prefix özelliğini sağladığını belirtir.
CAP, "Capability" kelimesinin kısaltmasıdır.
	*, tüm özelliklerin sağlandığını belirtir.
	ACK, "Acknowledge" kelimesinin kısaltmasıdır.
	:multi-prefix, sağlanan özelliktir.

IRC client, bu mesajı aldıktan sonra, multi-prefix özelliğini kullanabilir.

--CAP END--
"CAP END" mesajı, IRC client'in, IRC kanalıyla olan kapasite müzakerelerini tamamladığını belirtir.

KISACA:
Örneğin, bir IRC client, IRC serverına katılmak için CAP LS 302 mesajı gönderir.
	IRC serverının sahibi veya yöneticisi, CAP * LS :multi-prefix sasl mesajı ileterek, client'in desteklediği ek özelliklerin bir listesini gönderir.
	IRC client, CAP REQ :multi-prefix mesajı ileterek, multi-prefix özelliğini talep eder.
	IRC serverının sahibi veya yöneticisi, CAP * ACK multi-prefix mesajı ileterek, özelliğin sağlandığını belirtir.
	Son olarak, IRC client, CAP END mesajı ileterek, müzakerelerin tamamlandığını belirtir.

----------------------


 * >CAP LS 302<
 * An IRC client message that lists the additional features that an IRC server can offer to users.
 * 	• CAP: Short for "Capability".
 * 	• LS: Short for "List".
 * 	• 302: Indicates that a list of additional features is being offered.
 * The additional features listed in a CAP LS 302 message can enhance the functionality of an IRC server.
 * 	• For example, a server might offer features such as private messaging, file sharing, or gaming to its users.
 * 
 * >CAP * LS :multi-prefix sasl<
 * An IRC client message that lists the additional features that the client supports.
 * 	• CAP: Short for "Capability".
 * 	• *: Indicates that all features should be listed.
 * 	• LS: Short for "List".
 * 	• :multi-prefix sasl: The supported additional features.
 * 	 ø multi-prefix: Allows an IRC client to use multiple prefixes.
 * 	 ø sasl: Indicates that the IRC client supports the SASL authentication protocol.
 * 
 * >CAP REQ :multi-prefix<
 * A command that indicates that an IRC client is requesting the multi-prefix feature from an IRC server.
 * 	• CAP: Short for "Capability".
 * 	• REQ: Short for "Request".
 * 	• :multi-prefix: The requested feature.
 * 
 * >CAP * ACK multi-prefix<
 * A message from an IRC server's owner or administrator that indicates that the server is providing the multi-prefix feature that was requested by an IRC client.
 * 	• CAP: Short for "Capability"
 * 	• *: Indicates that all features are being provided
 * 	• ACK: Short for "Acknowledge"
 * 	• :multi-prefix: The provided feature
 * After receiving this message, the IRC client can use the multi-prefix feature.

 * >CAP END<
 * A message that indicates that an IRC client has completed capacity negotiations with an IRC channel.
 * 
 * IN SUMMARY;
 * For example, an IRC client might send a CAP LS 302 message to join an IRC server.
 * 	• The IRC server's owner or administrator might respond with a CAP * LS :multi-prefix sasl message, listing the additional features that the server supports.
 * 	• The IRC client might then send a CAP REQ :multi-prefix message, requesting the multi-prefix feature.
 * 	• The IRC server's owner or administrator might respond with a CAP * ACK multi-prefix message, indicating that the feature is being provided.
 * 	• Finally, the IRC client might send a CAP END message, indicating that the negotiations are complete.
 * 
 * @fn Client::getIRCstatus(); Getting client's register status.
 * @fn Client::sendMessageFd(); Sending to client(fd) message.
 * @fn Client::getPrefix(); Getting client's full information.
 * @fn Client::getNickname(); Getting nickname.
 * 
 * @param it 
 * @param tokenArr 
 */
void	Server::cap( Client* it, std::vector<std::string> tokenArr )
{
	std::cout << YELLOW << "CAP" << END << std::endl;

	if (it->getIRCstatus() != CONNECTING)
	{
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client's status is insufficient."));
		return ;
	}
	if (!tokenArr.at(0).compare("CAP")
			&& !tokenArr.at(1).compare("LS")
			&& !tokenArr.at(2).compare("302"))
		it->sendMessageFd("CAP * LS :multi-prefix sasl");
	if (!tokenArr.at(0).compare("CAP")
			&& !tokenArr.at(1).compare("REQ")
			&& !tokenArr.at(2).compare(":multi-prefix"))
		it->sendMessageFd("CAP * ACK multi-prefix");
	if (!tokenArr.at(0).compare("CAP")
			&& !tokenArr.at(1).compare("END"))
	{
		it->setIRCstatus(HANDSHAKE);
		it->sendMessageFd(RPL_NOTICE(it->getPrefix(), it->getNickname(), "Client status is now: HANDSHAKE."));
	}
}
