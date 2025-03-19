/*
** EPITECH PROJECT, 2024
** Paradigms Seminar
** File description:
** christ ftp
*/

#include "./my.h"

bool command_suit2(char buffer[1024],
    char *message, int client_socket)
{
    if (strncmp(buffer, "CWD ", 4) == 0) {
        cwd_command(client_socket, buffer);
        return true;
    }
    if (strcmp(buffer, "CDUP\r\n") == 0) {
        cdup_command(client_socket);
        return true;
    }
    if (strncmp(buffer, "LIST", 4) == 0) {
        list_command(client_socket);
        return true;
    }
    if (strncmp(buffer, "PORT", 4) == 0) {
        pasv_command(client_socket);
        return true;
    }
    return false;
}

void cdup_command(int client_socket)
{
    char *message;

    if (chdir("..") == 0) {
        message = "250 Directory successfully changed.\r\n";
        write(client_socket, message, strlen(message));
    } else {
        message = "550 Failed to change directory.\r\n";
        write(client_socket, message, strlen((message)));
    }
}

void list_command(int client_socket)
{
    DIR *d;
    struct dirent *dir;

    d = opendir(".");
    dir = readdir(d);
    if (d) {
        while (dir != NULL) {
            write(client_socket, dir->d_name, strlen(dir->d_name));
            write(client_socket, "\n", 1);
            dir = readdir(d);
        }
        closedir(d);
    }
    write(client_socket, "226 LIST status okay\n", 22);
}

void pasv_command(int client_socket)
{
    char *message;

    message = "227 Entering Passive Mode\r\n";
    write(client_socket, message, strlen((message)));
}
