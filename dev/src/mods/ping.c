#include "../lib/libjackbot.h"

void ping(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &ping,
  "Ping",
  "",
  "PING",
  ""
};

void ping(struct _Nfos_ *nfos)
{
  irc_cmd("PONG %s", nfos->sender->message);
}
