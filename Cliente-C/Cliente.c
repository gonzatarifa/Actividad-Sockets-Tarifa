#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

WSADATA WSAData;
SOCKET server;
SOCKADDR_IN addr;
char buffer[1024];

void conectarServidor() {
    printf("Conectando al servidor...\n\n");
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_addr.s_addr = inet_addr("192.168.56.1");  // IP del servidor
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);
    connect(server, (SOCKADDR *)&addr, sizeof(addr));
    printf("Conectado al Servidor\n");
}

void enviar(int opcion, int longitud) {
    // Enviar la opción
    sprintf(buffer, "%d", opcion);
    send(server, buffer, strlen(buffer), 0);
    Sleep(100); // Breve pausa para asegurarse de que los datos se envíen por separado

    // Enviar la longitud
    sprintf(buffer, "%d", longitud);
    send(server, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
}

void recibir() {
    recv(server, buffer, sizeof(buffer), 0);
    printf("\nRespuesta del servidor: %s\n", buffer);
    memset(buffer, 0, sizeof(buffer));
}

void cerrarSocket() {
    closesocket(server);
    WSACleanup();
    printf("Socket cerrado.\n\n");
}

void mostrarMenu() {
    printf("\nMenu:\n");
    printf("1. Generar nombre de usuario\n");
    printf("2. Generar contrasena\n");
    printf("3. Salir\n");
}

int main() {
    int opcion, longitud;

    conectarServidor();

    while(1) {
        mostrarMenu();
        printf("Elige una opcion: ");
        scanf("%d", &opcion);

        if (opcion == 3) {
            break;  // Salir del bucle si se elige la opción 3
        }

        if (opcion == 1 || opcion == 2) {
            printf("Ingresa la longitud deseada: ");
            scanf("%d", &longitud);
            enviar(opcion, longitud);
            recibir();
        } else {
            printf("Opcion invalida, por favor elige una opción valida.\n");
        }
    }

    cerrarSocket();
    return 0;
}
