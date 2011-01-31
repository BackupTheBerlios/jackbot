#include "../lib/libjackbot.h"
#include <time.h>

void log_irc(struct _Nfos_ *nfos); 

struct _Mods_ _mod_info = 
{                         
  NULL,                   
  &log_irc,                  
  "Log", 
  "",                    
  "EVR",                
  "" 
};

/*
 * This is the log mod. It writes all received messages into a server (hostname) specific file.
 * It will be the source for the stats mod
 */
void log_irc(struct _Nfos_ *nfos)
{
  static time_t eventtime;
  static FILE *logfile = NULL;
  static char filename[10 + HOST_NAME_MAX + 1]; // mods/logs/ + hostname + \0

  sprintf(filename, "mods/logs/%s", nfos->server->hostname);

  if(!logfile)
  {
    if(!(logfile = fopen(filename, "a")))
    {
      printf("Error on opening %s", filename);
      return;
    }else
      fprintf(logfile, "-----\n"); // new connection - mark (will be shown if needed...)
  }
  
  time(&eventtime);
  fprintf(logfile, "%.24s,%s,%s,%s,%s,%s,%s,%s\n",
      ctime(&eventtime),
      nfos->sender->nickname,
      nfos->sender->user,
      nfos->sender->host,
      nfos->sender->servername,
      nfos->sender->command,
      nfos->sender->request_nr,
      nfos->sender->message);

  fflush(logfile);

  debug_out("%s: %.24s,%s,%s,%s,%s,%s,%s,%s\n", 
      filename, 
      ctime(&eventtime),
      nfos->sender->nickname, 
      nfos->sender->user,
      nfos->sender->host,
      nfos->sender->servername,
      nfos->sender->command,
      nfos->sender->request_nr,
      nfos->sender->message);
}
