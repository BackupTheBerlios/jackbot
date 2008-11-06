#include "../lib/libjackbot.h"

void invite(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &invite,
  "Invite",
  "",
  "INVITE",
  "" 
};

void invite(struct _Nfos_ *nfos)
{
  int msg_pos;

  for(msg_pos = 0; *(nfos->sender->message + msg_pos) != '#'; msg_pos++);
  irc_cmd("JOIN %s", (nfos->sender->message + msg_pos));
}
