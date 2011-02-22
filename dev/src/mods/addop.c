/*
   This mod is for adding nicknames to a channel in the file oplist.mod
   That file will be read by the mod op_by_list and will give OP prevs
   to the nicknames in the channel
*/

#include "../lib/libjackbot.h"
#include <unistd.h>

void addop(struct _Nfos_ *nfos);
void finish_opfile(void);

struct _Mods_ _mod_info =
{
  NULL,
  &addop,
  "Add OP",
  "addop",
  "",
  ""
};

FILE *fp_oplist;
FILE *fp_tmplist;
char *opfile_name  = "mods/oplist.mod";
char *tmpfile_name = "mods/tmpfile.mod";

void addop(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char *channel = nfos->sender->middle;
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];
  char name_to_add[NICK_NAME_MAX + 1];

  /*if(nfos->sender->message[0] == '#')
  {
    for(ctr = 0; nfos->sender->message[ctr] != ' '; ctr++)
      channel[ctr] = nfos->sender->message[ctr];             
    
    channel[ctr] = '\0';
  }

  while(nfos->sender->message[ctr] != ':')
    ctr++;
  ctr++;*/

  if(is_priv())
    return;

  if(get_from_message(name_to_add, GFM_WORD))
  {
    //name_to_add = &nfos->sender->message[ctr + 7];

    if(!(fp_oplist = fopen(opfile_name, "r+"))) // if file doesn't exist yet
      fp_oplist = fopen(opfile_name, "w+"); // create file

    fp_tmplist = fopen(tmpfile_name, "w");

    if(fp_oplist == NULL || fp_tmplist == NULL)
    {
      printf("Error on opening a file\n");
      return;
    }

    while(fgets(list_channel, CHAN_NAME_MAX + 1, fp_oplist) && fputs(list_channel, fp_tmplist))
    {
      if(list_channel[0] == '#')
      {

        //FIXXME - boeser hack! evtl. 
        if(list_channel[strlen(list_channel) - 1] == '\n')
          list_channel[strlen(list_channel) - 1] = '\0';
        //FIXXME

        if(!strcmp(channel, list_channel)) //I've found the searched channel
        {
          while(fgets(nickname, NICK_NAME_MAX + 1, fp_oplist) && fputs(nickname, fp_tmplist))
          {
            if(nickname[0] == '\n')
              break; // nickname not in the list - has to be added

            if(nickname[strlen(nickname) - 1] == '\n')
              nickname[strlen(nickname) - 1] = '\0';

            if(!strcmp(name_to_add, nickname)) // nickname is already in the list...
            {
              irc_cmd("PRIVMSG %s :%s is already in OP list", channel, name_to_add);
              fclose(fp_oplist);
              fclose(fp_tmplist);
              remove(tmpfile_name);
              return;
            }
          }
          // break destination
          // write name_to_add in file
          fprintf(fp_tmplist, "%s\n", name_to_add); 
          irc_cmd("PRIVMSG %s :%s added to OPs of channel %s!", channel, name_to_add, channel);
          finish_opfile();
          return;
        }
      }
    }
    //I didn't find the channel
    //channel and nickname have to be added
    fprintf(fp_tmplist, "%s\n%s\n", channel, name_to_add);
    irc_cmd("PRIVMSG %s :%s was proudly added as first OP of channel %s!", channel, name_to_add, channel);
    finish_opfile();
  }else
    irc_cmd("PRIVMSG %s :You did not enter a nickname!", channel);
}

void finish_opfile(void)
{
  char line[CHAN_NAME_MAX + 1];

  while(fgets(line, CHAN_NAME_MAX + 1, fp_oplist) && fputs(line, fp_tmplist));

  if(!rename(tmpfile_name, opfile_name))
    remove(tmpfile_name);
  else
    printf("Error on renaming %s", tmpfile_name);

  fclose(fp_oplist);
  fclose(fp_tmplist);
}
