/*
** EPITECH PROJECT, 2024
** Paradigms Seminar
** File description:
** christ ftp
*/

#include "./my.h"

extern fd_set anonymousClients;
extern bool is_user_connected;

/*
void commande(char buffer[1024], char *message,
    int client_socket, fd_set* active_fd_set)
{
    if (strncmp(buffer, "USER ", 5) == 0) {
        user_command(client_socket, buffer);
    } else if (strncmp(buffer, "PASS ", 5) == 0) {
        pass_command(client_socket, buffer);
    } else if (strncmp(buffer, "QUIT", 4) == 0) {
        handle_client_disconnect(client_socket, active_fd_set);
    } else {
        message = "Commande non reconnue\r\n";
        write(client_socket, message, strlen(message));
    }
}

void list_command(int client_socket, const char *command) {

    int result = system("ls");
    char *cc = strtok((buffer + 5), "\r\n");

    if (result == 0) {
        write(client_socket,"250 Requested file
        action okay, completed.\r\n", 44);
    } else {
        write(client_socket, "550 Requested action not taken.\r\n", 33);
    }
}
void quit_command(int client_socket)
{
    char *message = "221 Goodbye.\r\n";
    write(client_socket, message, strlen(message));
    //FD_CLR(client_socket, &anonymousClients);
    //shutdown(client_socket, SHUT_RDWR);
    close(client_socket);
}
shutdown()
   print_listening_message(server_socket, port);
void print_listening_message(int server_socket, int port)
{
    char message[1024];
    snprintf(message, sizeof(message),
    "Listening on port %d\r\n", port);
    write(server_socket, message, strlen(message));
}*/

void user_command(int client_socket, const char *command)
{
    const char *username = command + 5;
    char *response;

    if (strcmp(username, "Anonymous\r\n") == 0) {
        FD_SET(client_socket, &anonymousClients);
    }
    response = "331 User name okay, need password\r\n";
    write(client_socket, response, strlen(response));
}

void del_command(int client_socket, char *buffer)
{
    char *message;
    char *pathname = strtok((buffer + 5), "\r\n");

    if (remove(pathname) == 0) {
        message = "250 Requested file action okay, completed.\r\n";
    } else {
        message = "550 Requested action not taken.\r\n";
    }
    write(client_socket, message, strlen(message));
}

bool command_suit(char buffer[1024],
    char *message, int client_socket)
{
    if (strncmp(buffer, "USER ", 5) == 0) {
        user_command(client_socket, buffer);
        return true;
    }
    if (strncmp(buffer, "PASS ", 5) == 0) {
        pass_command(client_socket, buffer);
        return true;
    }
    if (strcmp(buffer, "NOOP\r\n") == 0) {
        message = "200 Command okay.\r\n";
        write(client_socket, message, strlen(message));
        return true;
    }
    if (strcmp(buffer, "PWD\r\n") == 0) {
        pwd_command(client_socket);
        return true;
    }
    return false;
}

void help_command(int client_socket)
{
    char *message = "214 Pleeeeaaaase Help !!!\r\n";

    write(client_socket, message, strlen(message));
}

void pwd_command(int client_socket)
{
    char cwd[1024];
    char *response;

    if (!is_user_connected) {
        response = "530 Not logged in\r\n";
        write(client_socket, response, strlen(response));
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        response = "257 /\r\n";
        write(client_socket, response, strlen(response));
    } else {
        response = "550 Failed.\r\n";
        write(client_socket, response, strlen(response));
    }
}
