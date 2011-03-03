#include "../lib/libjackbot.h"

void op_by_list(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = // for describtion see pong.c
{
  NULL,
  &op_by_list,
  "OP by list",
  "",
  "JOIN",
  ""
};

char *opfile = "mods/oplist.mod";

/*
 * This bot is for giving users op rights if the are listet in the opfile, under the specific channel name,
 * If a user joins a channel, the bot gets something like this:
 *   :JacK_McRiDER JOIN #foo
 * The mod searches, if the name JacK_McRiDER is listet in the opfile under the channel #foo
 * if yes, he gives him op rights
 *
 * The opfile looks like this:
 *   #foo
 *   sergiorey
 *   
 *   #bar
 *   sergiorey
 *   JacK_McRiDER
 *
 * TODO
 * - give op rights on getting them, so users don't have to rejoin for getting op
 * - any authentication, because just the nickname is no secure one...
 */
void op_by_list(struct _Nfos_ *nfos)
{
  static FILE *fp_list;
  int ctr, ctr2;
  char *channel = nfos->sender->middle; //[CHAN_NAME_MAX + 1];;
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];

  // OPENING THE OPFILE
  if(!fp_list)
    fp_list = fopen(opfile, "r");
  else
    rewind(fp_list);
  
  if(fp_list == NULL)
  {
    printf("Error on opening %s\n", opfile);
    return;
  }
  
  // GETTING THE CHANNEL NAME FROM THE JOIN MESSAGE
  /*for(ctr = 0; nfos->sender->middle[ctr] != '#'; ctr++);
  for(ctr2 = 0; nfos->sender->middle[ctr + ctr2] != ' ' && ctr2 <= CHAN_NAME_MAX; ctr2++)
    channel[ctr2] = nfos->sender->middle[ctr + ctr2];
  channel[ctr2] = '\0';*/

  
  // SEARCHING THROUGH THE OPFILE LINE BY LINE
  while(fgets(list_channel, CHAN_NAME_MAX + 1, fp_list))
  {
    if(list_channel[0] == '#') // if this line is a channel name...
    {
      
      // ...take it!
//FIXXME - maybe an evil hack... 
      if(list_channel[strlen(list_channel) - 1] == '\n')
	list_channel[strlen(list_channel) - 1] = '\0';
//FIXXME

      if(!strcmp(channel, list_channel)) // yes, it's the channel we are in
      {
	      while(fgets(nickname, NICK_NAME_MAX + 1, fp_list)) // get next line, it must be a nickname
	      {
	        if(nickname[0] == '\n')
	          break;
	  
	        if(nickname[strlen(nickname) - 1] == '\n')
	          nickname[strlen(nickname) - 1] = '\0';

	        if(!strcmp(nfos->sender->nickname, nickname) || !strcmp(nickname, "*")) // yes, it's the nickname that joined
	        {
	          irc_cmd("MODE %s +o %s", channel, nfos->sender->nickname); // give him '+o' which means op rights
	          return;
	        }
	      }
      }
    }
  }
}
