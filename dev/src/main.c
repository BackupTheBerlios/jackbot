/**************************************\
 * project....: JacKbot               *
 * file.......: main.c                *
 * description: main part of the code *
\**************************************/
#include "main.h"

void main_while(void);

int main(int argc, char *argv[])
{
  int arg, is_server = 0;

  init();

  // welcome message - using ncurses, ncurses should be annihilated from the whole project...
  printf(
    "WELCOME! YOU'VE GOT THE JACKBOT\n"
    "  visit http://developer.berlios.de/projects/jackbot/\n"
    "  for more informations...\n\n");
 
  if(argc < 2)
  {
    usage(argv[0]);
    exit(0);
  }

  while((arg = getopt(argc, argv, "s:n:p:h?")) != -1)
  {
    switch(arg)
    {
      case 's': 
        is_server = 1;
        strncpy(nfos->server->hostname, optarg, HOST_NAME_MAX + 1);
        printf("Server  : %s\n", nfos->server->hostname);
        break;
      case 'p':
        strncpy(nfos->server->port, optarg, PORT_MAX + 1);
        printf("Port    : %s\n", nfos->server->port);
        break;
      case 'n':
        strncpy(nfos->server->nickname, optarg, NICK_NAME_MAX + 1);
        printf("Nickname: %s\n", nfos->server->nickname);
        break;
      case 'h':
      case '?':
      default:
        usage(argv[0]);
        exit(0);
    }
  }

  if(!is_server)
  {
    usage(argv[0]);
    exit(0);
  }

  do_connect();   // pre.c
  load_mods();    // pre.c
  main_while();   // here...

  return 0;
}

void usage(char *prog_name)
{
#ifdef DEBUG
  printf("COMPILED WITH DEBUG OUTPUT\n");
#endif
  printf("%s", prog_name);
  printf(" -s server\tserver to connect to\n"
     " -n nick\tnickname (default: %s)\n"
     " -p port\tIRC port to connect to (default: 6667)\n"
     " -h\t\tshow this help message and exit\n",
     APP_NAME);

  return;
}

void main_while(void)
{
  int bytes;
  int pos;
  int msg_pos = 0;
  int cmd_len;
  int ctr = 0;
  char msg[MSG_MAX + 1];
  char buffer[1024];
  char cmd[MOD_CMD_MAX + 1];

  while(1)
  {
    bytes = recv(nfos->server->socket, buffer, 1024, 0);
#ifdef DEBUG    
    printf("buff: %.*s\n", bytes, buffer);
#endif // DEBUG
    
    for(pos = 0; pos < bytes;)
    {
      if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
      {
	msg[msg_pos] = '\0';
        parse_msg(msg);
#ifdef DEBUG
	printf("msg:  %s\n", msg);
        printf("PRIVMSG #bot :\n"
          "nfos\n"
          "  server\n"
          "    int  socket       = %d;\n"
          "    char hostname[]   = \"%s\";\n"
          "    char ip[]         = \"%s\";\n"
          "    char port[]       = \"%s\";\n"
          "  sender\n"
          "    char nickname[]   = \"%s\";\n"
          "    char user[]       = \"%s\";\n"
          "    char host[]       = \"%s\";\n"
          "    char servername[] = \"%s\";\n"
          "    char command[]    = \"%s\";\n"
          "    char request_nr[] = \"%s\";\n"
          "    char message[]    = \"%s\";\n",
          nfos->server->socket,
	  nfos->server->hostname,
	  nfos->server->ip,
	  nfos->server->port,
          (nfos->sender->nickname) ? nfos->sender->nickname : "",
	  (nfos->sender->user) ? nfos->sender->user : "",
	  (nfos->sender->host) ? nfos->sender->host : "",
	  (nfos->sender->servername) ? nfos->sender->servername : "",
	  (nfos->sender->command) ? nfos->sender->command : "",
	  (nfos->sender->request_nr) ? nfos->sender->request_nr : "",
	  nfos->sender->message);

	fflush(stdout);
	  
        //send_irc(buffer);
#endif // DEBUG
	
        nfos->mods = nfos->first_mod;
        memset(cmd, 0, MOD_CMD_MAX + 1);

	ctr = 0;
	while(nfos->sender->message[ctr] != ':')
          ctr++;
        ctr++; // point after the colon

        if((!strcmp(nfos->sender->command, "PRIVMSG") || !strcmp(nfos->sender->command, "NOTICE")) && ctr > 1 && (nfos->sender->message[ctr] == CMD_PREFIX && isgraph(nfos->sender->message[ctr + 1])))
        {
	  ctr++; // point after the CMD_PREFIX
          for(cmd_len = 0; nfos->sender->message[ctr + cmd_len] != ' ' && cmd_len < MOD_CMD_MAX; cmd_len++)
            cmd[cmd_len] = nfos->sender->message[ctr + cmd_len];
	  
	  cmd[cmd_len] = '\0';
        }else
	  strcpy(cmd, "0");

        do
        {

#ifdef DEBUG
	  printf("MOD: %s\n", nfos->mods->name);
#endif // DEBUG

          if(!strcmp(nfos->sender->command, nfos->mods->server_cmd) || !strcmp(cmd, nfos->mods->mod_cmd))
            (*nfos->mods->mod_main)(nfos);
          nfos->mods = nfos->mods->next;
        }while(nfos->mods);
      
        msg_pos = 0; // new line
	pos += 2; // point behind <crlf>
      }else
      {
        msg[msg_pos++] = buffer[pos++];
      }
    }
  }

  return;
}

void quit(void)
{
  printf("Exit!\n");
  printf(BREAK); 
  fflush(stdout);

  //endwin(); // unload curses
  close(nfos->server->socket);
}
