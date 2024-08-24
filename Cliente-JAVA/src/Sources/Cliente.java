package Sources;
import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class Cliente {

    private static Socket socket;
    private static PrintWriter out;
    private static BufferedReader in;

    private static void conectarServidor() {
        try {
            System.out.println("Conectando al servidor...");
            socket = new Socket("192.168.56.1", 5555); // IP del servidor y puerto
            out = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()), true);
            in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            System.out.println("Conectado al Servidor");
        } catch (IOException e) {
            System.out.println("Error al conectar al servidor: " + e.getMessage());
            System.exit(1);
        }
    }

    private static void enviar(int opcion, int longitud) {
        // Enviar la opción
        out.println(opcion);
        // Enviar la longitud
        out.println(longitud);
    }

    private static void recibir() {
        try {
            String respuesta = in.readLine();
            if (respuesta != null) {
                System.out.println("\nRespuesta del servidor: " + respuesta);
            } else {
                System.out.println("No se recibió respuesta del servidor.");
            }
        } catch (IOException e) {
            System.out.println("Error al recibir datos del servidor: " + e.getMessage());
        }
    }

    private static void cerrarSocket() {
        try {
            if (out != null) out.close();
            if (in != null) in.close();
            if (socket != null) socket.close();
            System.out.println("Socket cerrado.");
        } catch (IOException e) {
            System.out.println("Error al cerrar el socket: " + e.getMessage());
        }
    }

    private static void mostrarMenu() {
        System.out.println("\nMenú:");
        System.out.println("1. Generar nombre de usuario");
        System.out.println("2. Generar contraseña");
        System.out.println("3. Salir");
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int opcion, longitud;

        conectarServidor();

        while (true) {
            mostrarMenu();
            System.out.print("Elige una opción: ");
            opcion = scanner.nextInt();

            if (opcion == 3) {
                break; // Salir del bucle si se elige la opción 3
            }

            if (opcion == 1 || opcion == 2) {
                System.out.print("Ingresa la longitud deseada: ");
                longitud = scanner.nextInt();
                enviar(opcion, longitud);
                recibir();
            } else {
                System.out.println("Opción inválida, por favor elige una opción válida.");
            }
        }

        cerrarSocket();
        scanner.close();
    }
}
