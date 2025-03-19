/*
** EPITECH PROJECT, 2024
** Paradigms Seminar
** File description:
** christ ftp
*/

#include "./my.h"
extern bool is_user_connected;

int client_sockets[MAX_CLIENTS];
fd_set anonymousClients;

void pass_command(int client_socket, const char *command)
{
    const char *password = command + 5;
    char *response;

    if (FD_ISSET(client_socket, &anonymousClients) != 1) {
        response = "530 wrong\r\n";
        write(client_socket, response, strlen(response));
    } else {
        if (strcmp(password, "\r\n") != 0) {
            response = "530 not logged in.\r\n";
            write(client_socket, response, strlen(response));
        } else {
            is_user_connected = true;
            response = "230 user logged in, proceed.\r\n";
            write(client_socket, response, strlen(response));
        }
    }
}

void handle_new_connection(int server_socket,
    fd_set *active_fd_set, int *max_fd)
{
    struct sockaddr_in client_address;
    int i;
    socklen_t client_address_length = sizeof(client_address);
    int client_socket = accept(server_socket,
    (struct sockaddr *) &client_address, &client_address_length);
    char *message = "220 Welcome on Gdx Server\r\n";

    if (client_socket >= 0) {
        write(client_socket, message, strlen(message));
    }
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_socket;
            break;
        }
    }
    FD_SET(client_socket, active_fd_set);
    if (client_socket > *max_fd) {
        *max_fd = client_socket;
    }
}

void quit_command(int client_socket, fd_set* active_fd_set)
{
    char *message = "221 Goodbye.\r\n";

    write(client_socket, message, strlen(message));
    close(client_socket);
    FD_CLR(client_socket, active_fd_set);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_socket) {
            client_sockets[i] = 0;
            break;
        }
    }
}

void commande(char buffer[1024], char *message,
    int client_socket, fd_set* active_fd_set)
{
    if (command_suit(buffer, message, client_socket)) {
        return;
    }
    if (command_suit2(buffer, message, client_socket)) {
        return;
    }
    if (strncmp(buffer, "QUIT", 4) == 0) {
        quit_command(client_socket, active_fd_set);
        return;
    }
    if (strncmp(buffer, "DELE ", 5) == 0) {
        del_command(client_socket, buffer);
        return;
    }
    if (strcmp(buffer, "HELP\r\n") == 0) {
        help_command(client_socket);
        return;
    }
    message = "500 Syntax error, command unrecognized.\r\n";
    write(client_socket, message, strlen(message));
}

void handle_client_data(int client_socket, fd_set *active_fd_set)
{
    char buffer[1024];
    char *message;
    int bytes_received = read(client_socket, buffer, sizeof(buffer));

    buffer[bytes_received] = '\0';
    commande(buffer, message, client_socket, active_fd_set);
    if (bytes_received < 0) {
        exit(1);
    } else if (bytes_received == 0) {
        close(client_socket);
        FD_CLR(client_socket, active_fd_set);
    } else {
        buffer[bytes_received] = '\0';
        printf("Données reçues du client (%d) : %s\n",
        client_socket, buffer);
    }
}
