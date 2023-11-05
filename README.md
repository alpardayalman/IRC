
### To-do List

- [x] Socket and Multiplexing
- [x] Makefile / flags and relink
- [ ] RFC 1459 (protocols)
- [ ] Authentication.
- [x] Nickname, a username.
- [ ] Join a channel.
- [ ] Send and receive private messages using your reference client.
- [ ] All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
- [ ] Operators and regular users.
- [ ] Signal.
- [ ] fcntl.
- [ ] Leaks.
- [ ] Functions to know.
- [ ] Everyone's last word for this project.
- [ ] DIFI hanmen key exchangge. (let's look into that.)
-----------------------------------------

<img src="Additional/assets/irc.png">

-----------------------------------------

# Developers
- Emircan
- Talha
- Emre
- Sarp
- Alp

### The groups will be divided whilst examination.
-----------------------------------------

# NOTES
# Main Objective: finishing this project before 
##  1/11/2023
  ### Start of the project.
    Multiplexing handled using select. Uncharted territory in our school, 
    we are the only group that uses SELECT() for ft_IRC project.
    Transmiting of messages works.
    We are using Hexchat for reference.
    We need to chose which commands we need to implement for our project. For now we have beta PASS PRIVMSG INFO.
    5/5 day as a team. 
-----------------------------------------
##  2/11/2023
  ### Second day.
    Password check is near perfect. We are stress testing it.
    Colors added.
    Establishing modularity using a map for all commands.
    Our brain melted ...
    2/5 day because rain. frustrated kinda day.
-----------------------------------------
##  3/11/2023
  ### Third day.
    Ascii art added (Emre).
    Modularity added (Emircan).
    Kick started chanels (˚5) (Talha).
    /PASS changed to PASS in nc.
    We need to understand how to use signal ^D to send a message immediatly from nc.
    Well played boys.
-----------------------------------------
## 4/11/2023
  ### Fourth day.
    Made slight changes to the github repo.
    Nick and User added (Need to do slight changes to User to get the information in the right way).
    Channel class was updated, 1 user cannot join the same channel 2 times.
    Major step in hexchat we were able to create a channel. Number 5 indeed.
    ^D signal works.
    great day.
    Congrats indeed. Wp team.
-----------------------------------------
## 5/11/2023
  ### Fifth day.
    Debug isini duzeltelim. Her page'e ozel Debug olsun ve tanimlamalari Utilities de tutalim orn NICK.cpp #ifdef DEBUG_NICK, DEBUG_NICK_I, DEBUG_NICK_II..
    User'da aldigimiz formati degistirelim gorkemlerinki gibi 4 sifat girilsin. (3. input hatirladigim kadariyla.)
    hexchat ve nc ayrimi yapip ona gore bir mesaj gonderelim.
    to do listteki protocolslere bir goz atalim.
    remote iki client (farkli bilgisayardan) ortak bir chanelldan biribirine mesaj atabilsin.
    Diger Komutlara bakalim. (Pong, Ping kolay denilenleri hizlica ekleyelim.)
    Bu yazdiklarim 5'i icin degil direk bir sonraki zamana.
    Her gonderilen mesaj ve read write farkli renklerde server terminaline basilsin debugging isini cok kolaylastirir.
-----------------------------------------