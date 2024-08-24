#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>

WSADATA WSAData;
SOCKET server, client;
SOCKADDR_IN serverAddr, clientAddr;
char buffer[1024];

void generarNombreUsuario(int longitud, char* nombreUsuario) {
    if (longitud < 5 || longitud > 15) {
        sprintf(nombreUsuario, "Longitud invalida. Debe ser entre 5 y 15 caracteres.");
        return;
    }

    char vocales[] = "aeiou";
    char consonantes[] = "bcdfghjklmnpqrstvwxyz";
    int esVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if (esVocal) {
            nombreUsuario[i] = vocales[rand() % 5];
        } else {
            nombreUsuario[i] = consonantes[rand() % 21];
        }
        esVocal = !esVocal;
    }
    nombreUsuario[longitud] = '\0';
}

void generarContrasena(int longitud, char* contrasena) {
    if (longitud < 8 || longitud > 50) {
        sprintf(contrasena, "Longitud invalida. Debe ser entre 8 y 50 caracteres.");
        return;
    }

    char caracteres[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
}

void iniciarServidor() {
    WSAStartup(MAKEWORD(2,0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5555);

    bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
    listen(server, 0);

    printf("Servidor iniciado.\n");
    int clientAddrSize = sizeof(clientAddr);
    if((client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize)) != INVALID_SOCKET) {
        printf("El cliente se ha conectado\n");
    }
}

int recibirEntero() {
    memset(buffer, 0, sizeof(buffer));
    recv(client, buffer, sizeof(buffer), 0);
    return atoi(buffer);
}

void enviar(char* mensaje) {
    strcat(mensaje, "\n"); // Añadir un salto de línea al final del mensaje
    send(client, mensaje, strlen(mensaje), 0);
    printf("Mensaje enviado!\n");
}

void cerrarSocket() {
    closesocket(client);
    WSACleanup();
    printf("Socket cerrado, cliente desconectado.\n");
}

int main() {
    srand(time(NULL));
    iniciarServidor();

    while(1) {
        int opcion = recibirEntero();
        int longitud = recibirEntero();

        printf("Opcion recibida: %d, Longitud recibida: %d\n", opcion, longitud);

        char respuesta[1024] = {0};

        switch (opcion) {
            case 1:
                generarNombreUsuario(longitud, respuesta);
                break;
            case 2:
                generarContrasena(longitud, respuesta);
                break;
            default:
                sprintf(respuesta, "Opcion invalida. Usa 1 para nombre de usuario, 2 para contraseña.");
                break;
        }

        enviar(respuesta);
    }

    cerrarSocket();
    return 0;
}
