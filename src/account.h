#ifndef ACCOUNT_H
#define ACCOUNT_H

void log_on(char *netmask,char *login);
void log_out(char *netmask,char *login);
void rmDeadLogins(long checkTime);

#endif
