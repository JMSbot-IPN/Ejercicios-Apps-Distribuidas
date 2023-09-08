#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024];

    // Crear socket del cliente
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error al crear el socket del cliente");
        exit(EXIT_FAILURE);
    }

    // Configurar la estructura de dirección del servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr));

    // Conectar al servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error al conectar al servidor");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("Escribe un mensaje para enviar al servidor (o 'salir' para cerrar la conexión): ");
        fgets(message, sizeof(message), stdin);

        if (strncmp(message, "salir", 5) == 0) {
            break;
        }

        if (send(client_socket, message, strlen(message), 0) < 0) {
            perror("Error al enviar mensaje al servidor");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        memset(message, 0, sizeof(message));
        int bytes_received = recv(client_socket, message, sizeof(message), 0);
        if (bytes_received <= 0) {
            perror("Error en la recepción del mensaje del servidor");
            close(client_socket);
            exit(EXIT_FAILURE);
        }

        printf("Respuesta del servidor: %s", message);
    }

    close(client_socket);
    return 0;
}
