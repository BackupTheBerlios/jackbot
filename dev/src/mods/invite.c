#include "../lib/libjackbot.h"

void invite(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = // for discribtions see ping.c
{
  NULL,
  &invite,
  "Invite",
  "",
  "INVITE",
  "" 
};

/*
 * The mod is started if the irc server sends the INVITE cmd to the jackbot (so someone did '/invite jackbot' in a channel)
 * the mod gets a message like 
 *   :JacK_McRiDER INVITE JacKBot #foo
 * it just needs the channel name (#foo)
 * After that it just needs to join 
 */
void invite(struct _Nfos_ *nfos)
{
  int msg_pos;

  for(msg_pos = 0; *(nfos->sender->message + msg_pos) != '#'; msg_pos++);

  irc_cmd("JOIN %s", nfos->sender->message + msg_pos); 

  irc_cmd("PRIVMSG %s :%s, thx for invitation!", nfos->sender->message + msg_pos, nfos->sender->nickname);
}
