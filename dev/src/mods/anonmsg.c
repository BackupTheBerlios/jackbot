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
  char *receiver, *message;
  // get receivers nick
  for( ctr = 0; nfos->sender->message[ctr] != ':'; ctr++ );

  ctr += strlen(nfos->mods->mod_cmd) + 3; // plus :, ! and space

  receiver = &nfos->sender->message[ctr];

  while( nfos->sender->message[ctr] != ' ' )
    ctr++; 
  nfos->sender->message[ctr] = '\0';

  message = &nfos->sender->message[++ctr];

  irc_cmd("NOTICE %s :%s", receiver, message);
}
