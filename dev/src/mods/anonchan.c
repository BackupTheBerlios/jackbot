#include "../lib/libjackbot.h"
#include <stdlib.h>

void modmain(struct _Nfos_ *nfos); 

struct _Mods_ _mod_info = 
{                         
  NULL,                   
  &modmain,                  
  "anonchan", 
  "anonchan",                    
  "PRIVMSG",                
  "" 
};

typedef struct _Nicks_
{
  char nick[NICK_NAME_MAX];
  struct _Nicks_ *next;
  struct _Nicks_ *prev;
}Nicks;

Nicks *firstnick, *nicklist, *tmpnick;

/*
 * description
 */
void modmain(struct _Nfos_ *nfos)
{
  char nickname[NICK_NAME_MAX + 1];
  char cmd[MOD_CMD_MAX + 1];
  char param[MSG_MAX + 1]; // join/part + \0, we just need 5. but perhaps he wrote "!anonchan aoesuaosunthaoesunthaosentilrchb,.lrcp'p839g98ga[.e98uch" ...

  if(get_from_message(cmd, GFM_CMD) && !strcmp(cmd, nfos->mods->mod_cmd)) // someone typd "!anonchan ..."
  {
    get_from_message(param, GFM_PARAMS);

    if(!strcmp(param, "join")) // someone typed "!anonchan join"
    {
      for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // check, if he already joined...
      {
        if(!strcmp(nicklist->nick, nfos->sender->nickname)) // yes, he did!
        {
          irc_cmd("PRIVMSG %s :You already joined anonchan!", nfos->sender->nickname);
          return;
        }

        if(!nicklist->next)
          break;
      }

      // he isn't in anonchan! add him!
      if(!firstnick)
      {
        firstnick = calloc(1, sizeof(Nicks));
        nicklist = firstnick;
      }else
      {
        tmpnick = nicklist;
        nicklist = calloc(1, sizeof(Nicks));
        nicklist->prev = tmpnick;
        tmpnick->next = nicklist;
      }
      strcpy(nicklist->nick, nfos->sender->nickname);
      irc_cmd("PRIVMSG %s :Welcome to anonchan! You write, I mask!", nfos->sender->nickname);
    }else if(!strcmp(param, "part")) // someone wrote "!anonchan part", he wants to leave us...
    {
      for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // check, if he really joined...
      {
        if(!strcmp(nicklist->nick, nfos->sender->nickname)) // yes, he did!
        {
          if(nicklist->prev)
            nicklist->prev->next = nicklist->next;
          if(nicklist->next)
            nicklist->next->prev = nicklist->prev;
          if(nicklist == firstnick)
            firstnick = NULL;

          irc_cmd("PRIVMSG %s :Bye from anonchan...", nfos->sender->nickname);
          return;
        }

        if(!nicklist->next)
          break;
      }

      // no, he was not in anonchan...
      irc_cmd("NOTICE %s :You are not in anonchan! Do \"%canonchan join\" to join us!", nfos->sender->nickname, CMD_PREFIX);
    }else
    {
      irc_cmd("NOTICE %s :%s is not valid. Try \"%canonchan join\" to join anonchan and \"%canonchan part\" to part anonchan.", nfos->sender->nickname, param, CMD_PREFIX, CMD_PREFIX);
    }
  }else // message to broadcast to anonchan
  {
    if(nfos->sender->middle[0] != '#')
    {
      for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // check, if he is in anonchan, so is as able to write here
      {
        if(!strcmp(nicklist->nick, nfos->sender->nickname)) // ...
        {
          for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // broadcast it...
          {
            if(strcmp(nicklist->nick, nfos->sender->nickname)) // if it's NOT the senders nickname!
            {
              debug_out("PRIVMSG %s :%s\n", nicklist->nick, nfos->sender->message);
              irc_cmd("PRIVMSG %s :%s", nicklist->nick, nfos->sender->message);
            }

            if(!nicklist->next)
              break;
          } // sent!
        } 
      } // he is not in anonchan...
    }
  }
}
