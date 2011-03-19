#include "../lib/libjackbot.h"
#include <stdlib.h>

void mod_init(void);
void mod_main(struct _Nfos_ *nfos); 
void mod_quit(void);

struct _Mods_ _mod_info = 
{                         
  .mod_init   = &mod_init,
  .mod_main   = &mod_main,
  .mod_quit   = &mod_quit,
  .name       = "anonchan", 
  .mod_cmd    = "anonchan",                    
  .server_cmd = "PRIVMSG",                
  .requests   = "" 
};

typedef struct _Nicks_
{
  char nick[NICK_NAME_MAX];
  struct _Nicks_ *next;
  struct _Nicks_ *prev;
}Nicks;

Nicks *firstnick, *nicklist, *tmpnick;
char *nickfile = "mods/anonchan.mod";

/*
 * description
 */

void mod_init(void)
{
  FILE *fp_list;
  char nickname[NICK_NAME_MAX + 1];

  fp_list = fopen(nickfile, "r");

  if(fp_list == NULL)
  {
    printf("Error on opening %s! Can't load nicks from anonchan!\n", nickfile);
    return;
  }

  while(fgets(nickname, NICK_NAME_MAX + 1, fp_list))
  {
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

// FIXXMEE!!!!
    if(nickname[strlen(nickname) - 1] == '\n')
      nickname[strlen(nickname) - 1] = '\0';
// FIXXMEE!!!

    strcpy(nicklist->nick, nickname);
    irc_cmd("PRIVMSG %s :Guess who's back!", nickname);
  }
}

void mod_main(struct _Nfos_ *nfos)
{
  char nickname[NICK_NAME_MAX + 1];
  char cmd[MOD_CMD_MAX + 1];
  char param[MSG_MAX + 1]; // join/part + \0, we just need 5. but perhaps he wrote "!anonchan aoesuaosunthaoesunthaosentilrchb,.lrcp'p839g98ga[.e98uch" ...

  if(get_from_message(cmd, GFM_CMD) && !strcmp(cmd, nfos->mods->mod_cmd)) // someone typd "!anonchan ..."
  {
    get_from_message(param, GFM_WORD);

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

      return;
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

      return;
    }else if(!strcmp(param, "kick"))
    {
      // FIXXME: WE NEED AUTHORIZATIOS!!!
      // FIXXME: if(is_nick_authed(nickname))
      if(!get_from_message(nickname, GFM_WORD)) // get nick to kick
      {
        irc_cmd("NOTICE %s :You didn't enter a nick to kick!", nfos->sender->nickname);
        return;
      }

      get_from_message(param, GFM_PARAMS); // kickmessage

      for(nicklist = firstnick; nicklist; nicklist = nicklist->next)
      {
        if(!strncmp(nicklist->nick, nickname, NICK_NAME_MAX)) // remove nick from nicklist
        {
          if(nicklist->prev)
            nicklist->prev->next = nicklist->next;
          if(nicklist->next)
            nicklist->next->prev = nicklist->prev;
          if(nicklist == firstnick)
            firstnick = NULL;

          irc_cmd("PRIVMSG %s :You was kicked out of anonchan... (%s)", nickname, param);
          irc_cmd("PRIVMSG %s :%s was kicked out of anonchan... (%s)", nfos->sender->nickname, nickname, param);

          return;
        }
      }

      irc_cmd("PRIVMSG %s :Nick %s is not in anonchan!", nfos->sender->nickname, nickname);
      return;
    }else
    {
      irc_cmd("NOTICE %s :%s is not valid. Try \"%canonchan join\" to join anonchan and \"%canonchan part\" to part anonchan.", nfos->sender->nickname, param, CMD_PREFIX, CMD_PREFIX);

      return;
    }
  }else // message to broadcast to anonchan
  {
    if(nfos->sender->middle[0] != '#' && nfos->sender->message[0] != CMD_PREFIX)
    {
      for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // check, if he is in anonchan, so is as able to write here
      {
        if(!strcmp(nicklist->nick, nfos->sender->nickname)) // ...
        {
          for(nicklist = firstnick; nicklist; nicklist = nicklist->next) // broadcast it...
          {
            if(strcmp(nicklist->nick, nfos->sender->nickname)) // if it's NOT the senders nickname!
            {
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

void mod_quit(void)
{
  FILE *fp_list;

  fp_list = fopen(nickfile, "w");

  if(fp_list == NULL)
  {
    printf("Error on opening %s! Can't save nicks from anonchan!\n", nickfile);
    return;
  }

  for(nicklist = firstnick; nicklist; nicklist = nicklist->next)
  {
    irc_cmd("PRIVMSG %s :Whyever... anonchan is closed now! I contact you when I'm back!", nicklist->nick);
    fprintf(fp_list, "%s\n", nicklist->nick);
  }
}
