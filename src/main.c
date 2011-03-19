/**************************************\
 * project....: JacKbot               *
 * file.......: main.c                *
 * description: main part of the code *
\**************************************/
#include "main.h"

void main_while(void);

int main(int argc, char *argv[])
{
  int arg, is_server = 0, is_daemon = 0, pid;

  init();

  printf(
    "WELCOME! YOU'VE GOT THE JACKBOT\n"
    "  visit http://jackbot.berlios.de/\n"
    "  for more informations...\n\n");

  debug_out("Compiled with debug output!\n");
 
  if(argc < 2)
  {
    usage(argv[0]);
    exit(0);
  }

  while((arg = getopt(argc, argv, "s:n:p:dh?")) != -1)
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
      case 'd':
        is_daemon = 1;
        printf("Mode    : Daemon\n");
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

  // daemonize...
  if(is_daemon)
  {
    printf("Daemonize...\n");
    
    if((pid = fork()) > 0)
    {
      printf("PID: %d\n", pid);
      exit(0);
    }else if( pid < 0)
    {
      printf("Daemonazing failed... running normal!\n");
    }
  }

  main_while();

  return 0;
}

void usage(char *prog_name)
{
  printf("%s", prog_name);
  printf(" -s server\tserver to connect to\n"
     " -n nick\tnickname (default: %s)\n"
     " -p port\tIRC port to connect to (default: 6667)\n"
     " -d\t\trun %s as a daemon\n"
     " -h\t\tshow this help message and exit\n",
     APP_NAME, APP_NAME);

  return;
}

void main_while(void)
{
  fd_set fds;
  struct timeval timeout;
  int bytes;
  int pos;
  int msg_pos = 0;
  int cmd_len;
  int ctr = 0;
  char msg[MSG_MAX + 1];
  char buffer[1024];
  char cmd[MOD_CMD_MAX + 1];
#if (DEBUG)
  time_t eventtime;
#endif

  while(1)
  {
    timeout.tv_sec = 300;
    timeout.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(nfos->server->socket, &fds);
    select(nfos->server->socket + 1, &fds, NULL, NULL, &timeout);

    if (FD_ISSET(nfos->server->socket, &fds) == 0)
    {
      printf("Connection timed out. Reconnect...\n");
      do_connect();
      continue;
    }

    pos = 0;
    if((bytes = recv(nfos->server->socket, buffer, 1024, 0)) == 0)
    {
      do_connect();
      continue;
    }

#if (DEBUG)
    time(&eventtime);
#endif
    debug_out("\n%s", ctime(&eventtime));
    debug_out("buff: %.*s", bytes, buffer);
    
    while(pos < bytes)
    {
      if(buffer[pos] == '\r' && buffer[pos + 1] == '\n')
      {
        msg[msg_pos] = '\0';
        parse_msg(msg);
        
        debug_out("msg:  %s\n", msg);
        debug_out("PRIVMSG #bot :\n"
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
            "    char middle[]     = \"%s\";\n"
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
            nfos->sender->middle,
            nfos->sender->message);

        nfos->mods = nfos->first_mod;
        memset(cmd, 0, MOD_CMD_MAX + 1);

        if((!strcmp(nfos->sender->command, "PRIVMSG") || !strcmp(nfos->sender->command, "NOTICE")))
        {
          if((nfos->sender->message[0] == CMD_PREFIX && isgraph(nfos->sender->message[1])))
          {
            ctr++; // point after the CMD_PREFIX
            for(cmd_len = 0; nfos->sender->message[1 + cmd_len] != ' ' && cmd_len < MOD_CMD_MAX; cmd_len++)
              cmd[cmd_len] = nfos->sender->message[1 + cmd_len];
            cmd[cmd_len] = '\0';
          }else
            strcpy(cmd, "0");
        }else
          strcpy(cmd, "0");

        do
        {
          debug_out("MOD: %s\n", nfos->mods->name);

          get_from_message(NULL, GFM_NEW);

          if(
              !strcmp(nfos->sender->command, nfos->mods->server_cmd) || 
              !strcmp(cmd, nfos->mods->mod_cmd) || 
              !strcmp("EVR", nfos->mods->server_cmd) || 
              (nfos->mods->requests[0] != 0 && 
                (!strncmp(nfos->sender->request_nr, nfos->mods->requests, 3) || 
              (nfos->mods->requests[3] != 0 && 
                (!strncmp(nfos->sender->request_nr, &nfos->mods->requests[3], 3) || 
              (nfos->mods->requests[6] != 0 && 
                (!strncmp(nfos->sender->request_nr, &nfos->mods->requests[6], 3))))))))
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
  for(nfos->mods = nfos->first_mod; nfos->mods; nfos->mods = nfos->mods->next)
  {
    if(nfos->mods->mod_quit)
      (*nfos->mods->mod_quit)();
  }

  printf("Exit!\n");
  printf(BREAK); 
  fflush(stdout);
  close(nfos->server->socket);
}
