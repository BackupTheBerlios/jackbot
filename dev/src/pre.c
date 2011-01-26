/*************************************\
 * project....: JacKBot              *
 * file.......: pre.c                *
 * desrciption: functins to init bot *
\*************************************/
#include "pre.h"

inline void init(void)
{
  if(!(nfos = (_Nfos_t*)malloc(sizeof(_Nfos_t))) ||
      !(nfos->server = (_Server_t*)malloc(sizeof(_Server_t))) ||
      !(nfos->sender = (_Sender_t*)malloc(sizeof(_Sender_t))))
  {
    printf("malloc() failed\n");
    exit(0);
  }

  nfos->first_mod = NULL;

  atexit(quit);
  //initscr();
  //start_color();
  //clear();

  //init_pair(CLR_ERROR, COLOR_RED, COLOR_BLACK);
  //init_pair(CLR_MSG, COLOR_WHITE, COLOR_BLACK);
  //init_pair(CLR_OK, COLOR_GREEN, COLOR_BLACK);
  //init_pair(CLR_HEAD, COLOR_BLUE, COLOR_BLACK);

  //scrollok(stdscr, true);

  memset(nfos->server->hostname, 0, HOST_NAME_MAX + 1);
  strncpy(nfos->server->nickname, APP_NAME, NICK_NAME_MAX);
  strncpy(nfos->server->port, "6667", PORT_MAX);

  return;
}

void do_connect(void)
{
  struct sockaddr_in sin;
  struct hostent *hp;

  printf("  Get the server ip...");

  if((hp = gethostbyname(nfos->server->hostname)) == NULL)
  {
    printf("%s", hstrerror(h_errno));
    exit(EXIT_FAILURE);
  }else
  {
    printf(" OK!\n");
    strncpy(nfos->server->ip, inet_ntoa(*(struct in_addr *)hp->h_addr), 16);
  }  
  
  printf(BREAK);

  sin.sin_family = PF_INET;
  sin.sin_port   = htons((unsigned int)atoi(nfos->server->port));
  
  printf("  Convert server ip...");
  if((signed int)(sin.sin_addr.s_addr = inet_addr(nfos->server->ip)) == -1)
  {
    printf(" failed!\n    ERROR: inet_addr();: conversion failed!\n");
    exit(EXIT_FAILURE);
  }else
    printf(" OK!\n");

  printf(BREAK);

  printf("  Create socket...");
  if((nfos->server->socket = socket(PF_INET, SOCK_STREAM, 0)) == -1)
  {
    printf(" failed!\n    ERROR: socket();\n");
    exit(EXIT_FAILURE);
  }else
    printf(" OK!\n");

  printf(BREAK);

  printf("  Connect to server...");
  if(connect(nfos->server->socket, (struct sockaddr *)&sin, sizeof(sin)) == -1)
  {
    printf(" failed!\n    ERROR: connect();\n");
    exit(EXIT_FAILURE);
  }else
    printf(" OK!\n");

  printf(BREAK);

  printf("  Set nickname...");

  irc_cmd("NICK %s", nfos->server->nickname);
  // next two lines are just for debugging
  //recv(nfos->server->socket, buffer_, 1024, 0);
  //printf(buffer_);
  irc_cmd("USER %s 0 0 :%s by %s", nfos->server->nickname, APP_NAME, AUTHOR);
  // next two lines are just for debugging
  //recv(nfos->server->socket, buffer_, 1024, 0);
  //printf(buffer_);

  printf(" OK!\n");
  printf(BREAK);

  return;
}

inline void load_mods(void)
{
  struct dirent *dir;
  struct _Mods_ *tmp_mod = NULL;
  DIR *dir_p;
  int file_name_len;
  char mod_file[MOD_PATH_MAX + 1];
  void *mod = NULL;
  
  printf("  Loading mods...\n");
  if(!(dir_p = opendir(MOD_PATH)))
  {
    printf("ERROR: opendir();\n");
    exit(EXIT_FAILURE);
  }

  while((dir = readdir(dir_p)))
  {
    if(dir->d_type == 8)
    {
      file_name_len = strlen(dir->d_name);
      if(dir->d_name[file_name_len - 3] == '.' && dir->d_name[file_name_len - 2] == 's' && dir->d_name[file_name_len - 1] == 'o')
      {
        snprintf(mod_file, MOD_PATH_MAX, "%s/%s", MOD_PATH, dir->d_name);

        // load the mod
        printf("    %s", mod_file);
        if(!(mod = dlopen(mod_file, (RTLD_LAZY | RTLD_LOCAL))))
        {
          printf("%s", dlerror());
          exit(EXIT_FAILURE);
        }

        if(!(tmp_mod = (struct _Mods_*)dlsym(mod, "_mod_info")))
        {
          printf("%s", dlerror());
          dlclose(mod);
        }else
        {
          if(!nfos->first_mod) // first mod
          {
            nfos->mods = nfos->first_mod = tmp_mod;
            nfos->mods->next = NULL;
          }else
          {
            nfos->mods->next = tmp_mod;    // set pointer to the new mod
            nfos->mods = nfos->mods->next; // set new mod as actual
            nfos->mods->next = NULL; // set pointer of next on NULL
          }
          printf(" OK!\n");
        }
        // printf(nfos->mods->name);
      } // if is *.so
    } // if is d_type == 8
  } // while((dir = readdir(dir_p)))
  printf(BREAK);
}
