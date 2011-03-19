#include "../lib/libjackbot.h"
#include <stdlib.h>

void rejoin(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = 
{                         
  &rejoin,                  
  "Rejoin", 
  "part",                    
  "JOIN",                
  "001" 
};

typedef struct _channels_ {
  char channel[CHAN_NAME_MAX + 1];
  struct _channels_ *next;
  struct _channels_ *prev;
}Channels;

Channels *firstchan, *channels, *tempchan; 

/*
 * This is the first mod using the new Request Number Statement
 * It saves all joined channels in the Channels struct. If the bot got the !part command, the channel will be deleted from the list (kicks are not recognized...).
 * The important thing: The bot joins all the channels after a reconnection (if it gets Request Number 376 - "End of MOTD")!
 * So if the bot gets
 *   JOIN :#foobar
 * the channel #foobar will be added to the joined channels list (Channels struct).
 *   irc.ser.ver 376
 * makes him joining all the channels in the list.
 * If someone types
 *   !part
 * the parted channel is deleted from the rejoin list.
 */
void rejoin(struct _Nfos_ *nfos)
{
  char channel[CHAN_NAME_MAX + 1];

  if(!strcmp(nfos->sender->command, "JOIN"))
  {
    //for(pos = 0; nfos->sender->message[pos] != '#'; pos++);
    get_from_message(channel, GFM_CHANNEL);
    
    // search in struct
    for(channels = firstchan; channels; channels = channels->next)
    {
      // found: return
      if(!strcmp(channel, channels->channel))
      {
        debug_out("Channel allready in list!\n");
        return;
      }

      if(!channels->next)
        break;
    }

    // not found: add
    if(!firstchan)
    {
      firstchan = calloc(1, sizeof(Channels));
      channels = firstchan;
    }else
    {
      tempchan = channels;
      channels = calloc(1, sizeof(Channels));
      channels->prev = tempchan;
      tempchan->next = channels;
    }
    strncpy(channels->channel, channel, CHAN_NAME_MAX);
    debug_out("Channel %s not in list. Was added!\n", channel);
  }else if(!strcmp(nfos->sender->request_nr, "001"))
  {
    for(channels = firstchan; channels != NULL; channels = channels->next)
    {
      if(channels->channel[0] == '#')
      {
        irc_cmd("JOIN %s", channels->channel);
      }
    }
  }else // !part
  {
    for(channels = firstchan; channels != NULL; channels = channels->next)
    {
      if(!strcmp(nfos->sender->middle, channels->channel))
      {
        if(channels->prev) // if we are not the first chan
          channels->prev->next = channels->next;
        if(channels->next) // if we are not the last chan
          channels->next->prev = channels->prev;
        if(channels == firstchan) // if we just have one chan
          firstchan = NULL;

        return;
      }
    }
  }
}
