#ifndef DBACCESS_H
#define DBACCESS_H

void init_database(void);
void closeDatabase(void);

int add_user(char *login, char* passwd);
int replace_passwd(char *login, char* passwd);
int del_user(char *login);
int check_user(char *login, char *passwd);
int exists_user(char *login);

int add_login(char *netmask,char *login);
int check_login(char *netmask);
int del_login(char *netmask);
char *get_login(char *netmask);

int add_channel_access(char *name,char *access);
int replace_channel_access(char *name, char *access); 
int check_channel_access(char *name);
int del_channel_access(char *name);
char *get_channel_access(char *name);

int add_channel(char *channel,char *mode);
int replace_channel(char *channel, char *mode); 
int check_channel(char *channel);
int del_channel(char *channel);
char *get_channel(char *channel);

#endif
