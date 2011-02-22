#include "../lib/libjackbot.h"

void main(struct _Nfos_ *nfos); 

struct _Mods_ _mod_info = 
{                         
  NULL,                   
  &main,                  
  "Anonymous Message",
  "anonmsg",                    
  "",                
  "" 
};

/*
 * This mod sends a message from one nick to one other. The receiver
 * does only see the bot as the sender - but not the original one.
 * Bot get the following:
 *   :JacKMcRiDER PRIVMSG JacKBot :!anonmsg sergiorey Merry Christmas!
 * It gets the receivers nickname and the message to send anonymously.
 * To use it do
 *   /NOTICE JacKBot !anonmsg sergiorey Merry Christmas!
 */
void main(struct _Nfos_ *nfos)
{
  int ctr;
  char receiver[NICK_NAME_MAX + 1], message[MSG_MAX + 1];

  // get receivers nick
  get_from_message(receiver, GFM_WORD);

  // get message
  get_from_message(message, GFM_PARAMS);

  irc_cmd("PRIVMSG %s :%s", receiver, message);
}
