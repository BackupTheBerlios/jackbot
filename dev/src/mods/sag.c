#include "../lib/libjackbot.h"

void sag(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = // for description see ping.c
{
  NULL,
  &sag,
  "Sag",
  "sag",
  "",
  ""
};

/*
 * A message from the server will look like this:
 *   :JacK_McRiDER PRIVMSG #foo :!sag tell me if you're fuckin' alive!
 * The bot takes the channel (#foo) and sends to it everything after the !sag
 * If there is the username of the bot (JacKBot) instead of #foo, it's a private message
 * and the message (after !sag) will be sent to the username after the first colon
 */
void sag(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char message[MSG_MAX];
  
  get_from_message(message, GFM_PARAMS); 
  irc_cmd("PRIVMSG %s :%s", nfos->sender->middle, message);
}
