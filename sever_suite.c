/*
** EPITECH PROJECT, 2024
** Paradigms Seminar
** File description:
** christ ftp
*/

#include "./my.h"
bool is_user_connected = false;

int create_socket(int port)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in my_addr_server;

    if (server_socket < 0) {
        perror("Erreur lors de la création du socket");
        exit(1);
    }
    my_addr_server.sin_family = AF_INET;
    my_addr_server.sin_addr.s_addr = INADDR_ANY;
    my_addr_server.sin_port = htons(port);
    if (bind(server_socket, (struct sockaddr *) &my_addr_server,
    sizeof(my_addr_server)) < 0) {
        perror("Erreur lors de la liaison du socket");
        exit(1);
    }
    if (listen(server_socket, 60) < 0) {
        perror("Erreur lors de l'écoute du socket");
        exit(1);
    }
    return server_socket;
}

void coup(fd_set *read_fd_set, fd_set *active_fd_set,
    int *max_fd, int *server_socket)
{
    for (int fd = 0; fd <= *max_fd; ++fd) {
        if (FD_ISSET(fd, read_fd_set)) {
            ((fd == *server_socket) ?
            handle_new_connection(*server_socket,
                active_fd_set, max_fd) :
            handle_client_data(fd, active_fd_set));
        }
    }
}

void run_server_loop(int server_socket)
{
    fd_set read_fd_set;
    fd_set active_fd_set;
    int max_fd = server_socket;

    FD_ZERO(&active_fd_set);
    FD_SET(server_socket, &active_fd_set);
    while (1) {
        read_fd_set = active_fd_set;
        if (select(max_fd + 1, &read_fd_set, NULL, NULL, NULL) < 0)
            exit(1);
        coup(&read_fd_set, &active_fd_set, &max_fd, &server_socket);
    }
    close(server_socket);
}

void cwd_command(int client_socket, char *buffer)
{
    char *response;
    char *pathname = strtok((buffer + 4), "\r\n");

    if (!is_user_connected) {
        response = "530 Not logged in\r\n";
        write(client_socket, response, strlen(response));
        return;
    }
    if (chdir(pathname) == 0) {
        response = "250 Directory successfully changed.\r\n";
        write(client_socket, response, strlen(response));
    } else {
        response = "550 Failed to change directory.\r\n";
        write(client_socket, response, strlen(response));
    }
}

int main(int argc, char *argv[])
{
    int port = 0;
    int server_socket;
    char *path = NULL;

    if (argc < 3) {
        printf("Usage: %s <port> <path>\n", argv[0]);
        return 84;
    }
    port = atoi(argv[1]);
    path = argv[2];
    server_socket = create_socket(port);
    if (chdir(path) != 0) {
        perror("Erreur lors du changement de répertoire");
        return 84;
    }
    run_server_loop(server_socket);
    return 0;
}
