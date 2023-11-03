import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;

public class FileServer {
    public static void main(String[] args) {
        int port = 5000; // Puerto del servidor

        try (ServerSocket serverSocket = new ServerSocket(port)) {
            System.out.println("El servidor está escuchando en el puerto " + port);
            while (true) {
                Socket clientSocket = serverSocket.accept();
                System.out.println("Cliente conectado desde " + clientSocket.getInetAddress().getHostAddress());

                try (DataInputStream dis = new DataInputStream(clientSocket.getInputStream());
                     DataOutputStream dos = new DataOutputStream(clientSocket.getOutputStream())) {

                    // Recibir el nombre del archivo del cliente
                    String fileName = dis.readUTF();

                    // Crear un flujo de salida para escribir el archivo recibido
                    FileOutputStream fos = new FileOutputStream("/home/chuy/Escritorio/Apps-Distribuidas/transferencia de archivos ( Ws y Sockets)/" + fileName);

                    // Recibir el archivo del cliente y escribirlo en el servidor
                    byte[] buffer = new byte[4096];
                    int bytesRead;
                    while ((bytesRead = dis.read(buffer)) != -1) {
                        fos.write(buffer, 0, bytesRead);
                    }

                    // Confirmar la recepción del archivo
                    System.out.println("Archivo recibido del cliente: " + fileName);

                    // Cerrar el flujo de salida de archivos
                    fos.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}