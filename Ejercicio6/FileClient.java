import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class FileClient {
    public static void main(String[] args) {
        String serverAddress = "127.0.0.1"; // Dirección IP del servidor
        int port = 5000; // Puerto del servidor

        try (Socket socket = new Socket(serverAddress, port)) {
            DataOutputStream dos = new DataOutputStream(socket.getOutputStream());
            DataInputStream dis = new DataInputStream(socket.getInputStream());

            Scanner scanner = new Scanner(System.in);
            System.out.println("¿Qué acción quieres realizar? (upload/download)");
            String action = scanner.nextLine();

            if (action.equalsIgnoreCase("upload")) {
                // Para subir un archivo al servidor
                System.out.println("Introduce la ruta completa del archivo que deseas subir: ");
                String filePath = scanner.nextLine();
                File file = new File(filePath); // Ruta al archivo en la máquina cliente
                FileInputStream fis = new FileInputStream(file);

                // Enviar el nombre del archivo al servidor
                dos.writeUTF(file.getName());

                // Envío del archivo al servidor
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = fis.read(buffer)) != -1) {
                    dos.write(buffer, 0, bytesRead);
                }
                dos.flush(); // Asegurarse de que todos los datos se envíen

                // Confirmación de la finalización del envío
                System.out.println("Archivo enviado al servidor: " + file.getName());

                // Cerrar el flujo de entrada de archivos
                fis.close();
            } else if (action.equalsIgnoreCase("download")) {
                // Para descargar un archivo del servidor
                System.out.println("Introduce el nombre del archivo que deseas descargar: ");
                String fileName = scanner.nextLine();
                // Enviar el nombre del archivo al servidor
                dos.writeUTF(fileName);

                // Crear un flujo de salida para escribir el archivo recibido
                FileOutputStream fos = new FileOutputStream(fileName);

                // Recibir el archivo del servidor y escribirlo en la máquina cliente
                byte[] buffer = new byte[4096];
                int bytesRead;
                while ((bytesRead = dis.read(buffer)) != -1) {
                    fos.write(buffer, 0, bytesRead);
                }

                // Confirmar la recepción del archivo
                System.out.println("Archivo descargado del servidor: " + fileName);

                // Cerrar el flujo de salida de archivos
                fos.close();
            } else {
                System.out.println("Acción no reconocida. Por favor, elige 'upload' o 'download'.");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
