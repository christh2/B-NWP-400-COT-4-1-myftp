/*
** EPITECH PROJECT, 2024
** ntw
** File description:
** ftp
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/select.h>
#include <dirent.h>
#include <stdbool.h>

#define MAX_CLIENTS 60

int create_socket(int);
void run_server_loop(int);
void coup(fd_set *, fd_set *, int *, int *);
void handle_new_connection(int, fd_set *, int *);
void handle_client_data(int, fd_set *);
void user_command(int, const char *);
void pass_command(int, const char *);
void quit_command(int, fd_set*);
void del_command(int, char *);
void help_command(int);
bool command_suit(char buffer[1024], char *, int);
void pwd_command(int);
void cwd_command(int, char *);
bool command_suit2(char buffer[1024], char *, int);
void cdup_command(int client_socket);
void list_command(int client_socket);
void pasv_command(int client_socket);
