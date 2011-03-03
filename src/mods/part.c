#include "../lib/libjackbot.h"

void part(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = // for description see pong.c
{
  NULL,
  &part,
  "Part",
  "part",
  "",
  ""
};

/*
 * This is the opposite of the invite mod. If someone sends a !part in the channel, the bot will get
 *   :JacK_McRiDER PRIVMSG #foo :!part
 * This mod reads the channel (#foo) and leaves it (with a little exit message)...
 * Another way of make him part is 
 *   /NOTICE JacKBot !part #foo
 */
void part(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char channel[CHAN_NAME_MAX + 1];

  debug_out("It's %s!\n", nfos->sender->nickname);
  if(is_priv()) // if it's from a channel, no private message
  {
    get_from_message(channel, GFM_CHANNEL);
  }else
  {
    strncpy(channel, nfos->sender->middle, CHAN_NAME_MAX);
  }
    
  irc_cmd("PART %s :see ya'll, %s wants me to leave!", channel, nfos->sender->nickname);
}
