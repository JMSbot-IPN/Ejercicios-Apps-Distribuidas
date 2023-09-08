#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define MAX_CONNECTIONS 5

void *handle_client(void *socket_desc) {
    int client_socket = *(int *)socket_desc;
    char buffer[1024];

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Error en la recepción del mensaje del cliente");
            break;
        }

        printf("Mensaje del cliente: %s", buffer);

        if (send(client_socket, buffer, strlen(buffer), 0) < 0) {
            perror("Error al enviar mensaje al cliente");
            break;
        }
    }

    close(client_socket);
    free(socket_desc);
    pthread_exit(NULL);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Crear socket del servidor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura de dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección y el puerto
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error en la vinculación del socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Escuchar por conexiones entrantes
    if (listen(server_socket, MAX_CONNECTIONS) == -1) {
        perror("Error al escuchar por conexiones entrantes");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Servidor esperando conexiones en el puerto %d...\n", PORT);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error al aceptar la conexión del cliente");
            continue;
        }

        pthread_t client_thread;
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = client_socket;

        if (pthread_create(&client_thread, NULL, handle_client, (void *)client_socket_ptr) != 0) {
            perror("Error al crear el hilo del cliente");
            free(client_socket_ptr);
        }
    }

    close(server_socket);
    return 0;
}
