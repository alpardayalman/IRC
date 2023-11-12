# Internet Relay Chat (FT_IRC)

This project is about creating your own IRC server.
You will use an actual IRC client to connect to your server and test it.
Internet is ruled by solid standards protocols that allow connected computers to interact
with each other.
It’s always a good thing to know.
Version: 6

-----------------------------------------
## To-do List

- [x] Socket and Multiplexing
- [x] Makefile / flags and relink
- [x] RFC 1459 (protocols)
- [x] Authentication.
- [x] Nickname, a username.
- [x] Join a channel.
- [x] Send and receive private messages using your reference client.
- [x] All the messages sent from one client to a channel have to be forwarded to every other client that joined the channel.
- [x] Operators and regular users.
- [x] flood
- [x] Notice, ping, pong
- [x] Kick, Topic, Part
- [x] File transfer lessgo
- [x] Bot
- [x] proper usage of clients gui
- [x] Leaks. & seg correction
- [ ] Signal. ^D & ^Z
- [ ] Preliminary last checks (checkout 13'th day)
- [ ] Evo for all 2 of the teams.
- [ ] Everyone's last word for this project.

-----------------------------------------

## Developers
<table>
  <tbody>
    <tr>
      <td align="center" valign="top" width="20%"><a href="https://github.com/EmirKymz"><img src="https://avatars.githubusercontent.com/u/99013427?v=4" width="100px;" alt="Emircan Kaymaz"/><br /><sub><b>Emircan Kaymaz</b></sub></a><br /></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/TalhaAcikgoz"><img src="https://avatars.githubusercontent.com/u/89697506?v=4" width="100px;" alt="Talha Acikgoz"/><br /><sub><b>Talha Acikgoz</b></sub></a><br /></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/yeaktas"><img src="https://avatars.githubusercontent.com/u/96894640?v=4" width="100px;" alt="Yunus Emre Aktas"/><br /><sub><b>Yunus Emre Aktas</b></sub></a><br /></td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/Higlix"><img src="https://avatars.githubusercontent.com/u/109249128?v=4" width="100px;" alt="Sarp Erkiralp"/><br /><sub><b>Sarp Erkiralp</b></sub></a><br /> 
</td>
      <td align="center" valign="top" width="20%"><a href="https://github.com/alpardayalman"><img src="https://avatars.githubusercontent.com/u/82611850?v=4" width="100px;" alt="Alp A. Yalman"/><br /><sub><b>Alp A. Yalman</b></sub></a><br /></td>
    </tr>
  </table>
</tbody>

   The groups were divided whilst examination.
   -----------------------------------------


   <img src="Additional/assets/irc.png">
31
## NOTES
### Main Objective: finishing this project before our 2nd week.

  ### 1/11/2023 Start of the project.
  
    Multiplexing handled using select. Uncharted territory in our school, 
    we are the only group that uses SELECT() for ft_IRC project.
    Transmiting of messages works.
    We are using Hexchat for reference.
    We need to chose which commands we need to implement for our project. For now we have beta PASS PRIVMSG INFO.
    5/5 day as a team. 
-----------------------------------------

  ### 2/11/2023 Second day.
  
    Password check is near perfect. We are stress testing it.
    Colors added.
    Establishing modularity using a map for all commands.
    Our brain melted ...
    2/5 day because rain. frustrated kinda day.
-----------------------------------------
  ### 3/11/2023 Third day.
    Ascii art added (Emre).
    Modularity added (Emircan).
    Kick started chanels (˚5) (Talha).
    /PASS changed to PASS in nc.
    We need to understand how to use signal ^D to send a message immediatly from nc.
    Well played boys.
-----------------------------------------
  ### 4/11/2023 Fourth day.
    Made slight changes to the github repo.
    Nick and User added (Need to do slight changes to User to get the information in the right way).
    Channel class was updated, 1 user cannot join the same channel 2 times.
    Major step in hexchat we were able to create a channel. Number 5 indeed.
    ^D signal works.
    great day.
    Congrats indeed. Wp team.
-----------------------------------------

  ### 5/11/2023 Fifth day.
  
    Vacational and busy day for all of us. We worked really good so far.
-----------------------------------------

  ### 6/11/2023 Sixth day.
  
    What a good day, the purpose of this day was to create a channel that we could chat on.
    We were able to do more than that.
    We recreated the function TOPIC and INVITE today.
    We eased on Authentication it works really good.
    Divided the socket and the server files because Server::Run() was getting overwhelming.
    We added a CAP file to understand which client was being used HEX or nc.
    Funny enough whilst connecting hexchat sended CAP LS, PASS, NICK and USER piece by piece always in this order.
    Every now and then it sends CAP LS than PASS ... algather. or CAP LS and PASS than the rest ex. 
    For that reason we recreated a buffer system which gets them line by line.
    In general this was probably the day that we worked the best.
-----------------------------------------

  ### 7/11/2023 Seventh day.
  
    We did Topic, Notice, Part.
    We elaborated on Nick and now Topic can be changed for everybody.
    We did ^D signal.
    We now see the nickname in the left side of the chanel we can send private messages.
    !!!! We can transfer filess babay.
    We can exit the chanel and now it doesn't say that we are in the chanel we fr get kicked now.
    This must be the most productive day yet.
    
-----------------------------------------
  ### 8/11/2023 Eight'st day.

    Overall good day,
    we were able to improve the gui, we did quit and semi whois.
    We can see people and change nicknames.
    We got some errors during the day.
    We did ping and pong. 
    We need to do more unit testing however we only need to do the bot, 
    the mode command for the channel,
    better the gui and add an operator than we are done with this project.
    whois added.
    I am proud of my team.
    
-----------------------------------------
  ### 9/11/2023 Ninth day.
    Vacation day for fam', I was sick today. Almost every team members got sick this week.
    I did some backup coding for gui part and some systemic improvements however I
    could not test them in hexChat (I had troubles instaling hex to my mac). Instead I used limeChat
    Listing and some other stuff works however The apperance of names are kinda bad on the right side.
    We must Start on the operator priviledges and chanel mods than we are 99% done with the mandetory part.
    Once finished we need to program a bot than easy peasy. IRC project is done.

    
-----------------------------------------
  ### 10/11/2023 Tenth day.
  Sinavda bol sans beyler.
  First step to operators.
  Fixed notice.
  The exam was bad for the team we had some troubles concentrating.
  Next day fam'.
    
-----------------------------------------
  ### 11/11/2023 Eleventh day.
We updated the makefile which
We learned more about iterators. (creazy stuff).
We fixed a major bug in join.
Topic was added to the list.
Nick display all users.
Exeption class added.

  
-----------------------------------------
  ### 12/11/2023 Twelth day.
Fix in gui, server topic and nick. Had some problems with ping. We reached 200 commits.
As we are coming to an end of the project, I had some time to add the bot. For the bot I have recreated a new
command called names which sends to the bot. Than the bot sends the message to all the users.
We fixed Mode.
We completed MODE error replies.
    
-----------------------------------------
 ### 13/11/2023 Thirteenth day.
Ping, Notice, Quit and user fixed. Whist I am writing this We are almost done.
It's 2 am in the morning and the team is exhausted however overjoyed with what we have done.
Before doing some preliminary checks of our project, I would like to thank every one of you.
Let's keep up the paste for the last project people.

Ps: Evo page test, stress test and flags (shadow and -cpp=98). Welcome message delete.
-----------------------------------------
