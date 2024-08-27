/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package Controlador;

import java.io.DataInputStream;
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import Datos.Cconstantes;
import Datos.CmonitorDAO;
import Datos.Creportelora;

/**
 *
 * @author msalazar
 */
public class ControladorMonitor 
{
    private Thread hiloEscucha;
    
    // constructor
    public ControladorMonitor()
    {
        init();
    }
    
    /**
    * despliega el formulario      
    * @param void
    * @return void
    */
    private void init() 
    {
        System.out.println("Escuchando el socket por el puerto " + String.valueOf(Cconstantes.PORT) + "...");
        hiloEscucha = new Thread(new CsocketEscucha());
        hiloEscucha.start();
    }
    
    /**
    *  clase interna para el hilo de escucha del socket
    */
    class CsocketEscucha implements Runnable
    {
        ServerSocket server;
        
        @Override
        public void run()
        {
            String[] valuesReporteLora;
            String msjfromLora;            
            byte[] arrayBytes = new byte[120];
            int indexInit = 0;
            int indexEnd = 0;
            Creportelora reportelora;      
            CmonitorDAO monitordao = new CmonitorDAO();
            
            try
            {
                server = new ServerSocket(Cconstantes.PORT);
                
                while (true)
                {
                    try (Socket socket = server.accept()) //se mantiene a la escucha del puerto 9999
                    {
                        System.out.println("Mensaje recibido");
                        DataInputStream streamSocket = new DataInputStream(socket.getInputStream());
                        streamSocket.read(arrayBytes);
                        msjfromLora = new String(arrayBytes, StandardCharsets.UTF_8);
                        
                        // obtenemos el contenido del mensaje
                        for (int indexStr = 0; indexStr < msjfromLora.length(); indexStr++)
                        {
                            if (msjfromLora.charAt(indexStr) == '{')
                            {
                                indexInit = indexStr + 1;
                            }
                            
                            if (msjfromLora.charAt(indexStr) == '}')
                            {
                                indexEnd = indexStr;
                            }
                        }
                        
                        msjfromLora = msjfromLora.substring(indexInit, indexEnd);
                        System.out.println(msjfromLora);                                                                        
                        valuesReporteLora = msjfromLora.split(":");
                        reportelora = new Creportelora();
                        reportelora.setLatitud(valuesReporteLora[0]);
                        reportelora.setLongitud(valuesReporteLora[1]);
                        reportelora.setVelocidad(valuesReporteLora[2]);
                        reportelora.setTemperatura(valuesReporteLora[3]);
                        reportelora.setHumedadpcr(valuesReporteLora[4]);
                        
                        // insertamos en la db
                        monitordao.insertaenDb(reportelora);
                    }
                    
                    Arrays.fill(arrayBytes, (byte)0);
                }
            }
            catch (IOException ex)
            { 
                ex.printStackTrace();
            }
        }
    }
}