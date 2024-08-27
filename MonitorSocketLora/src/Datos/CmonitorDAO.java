/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package Datos;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author msalazar
 */
public class CmonitorDAO 
{
    /**
    * Inserta el reporte en la bd posgrest, no manejaremos el error
    * @param ReporteMeteo reporte.
    * @return void
    * @throws 
    */
    public void insertaenDb(Creportelora reporte)
    {
        Connection connection = null;
        PreparedStatement preparedsts = null;
        
        try
        {
            Class.forName("org.postgresql.Driver");          
            
            Properties props = new Properties();
            props.setProperty("user", Cconstantes.userBd);
            props.setProperty("password", Cconstantes.passBd);
            
            connection = DriverManager.getConnection(Cconstantes.urlPosgrest, props);
            preparedsts = connection.prepareStatement(Cconstantes.fun_alta_param);            
            preparedsts.setString(1, reporte.getLatitud());                 // insertamos los parametros al query
            preparedsts.setString(2, reporte.getLongitud());
            preparedsts.setString(3, reporte.getVelocidad());
            preparedsts.setString(4, reporte.getTemperatura());
            preparedsts.setString(5, reporte.getHumedadpcr());                        
            preparedsts.execute();
            System.out.println("Se inserto el registro...");
        }
        catch (Exception ex)
        {
            System.out.println("Ocurrio un error al insertar el registro " + ex.getMessage());
        }
        finally
        {
            try {
                if (connection != null)
                    connection.close();
            } catch (SQLException ex) {
                Logger.getLogger(CmonitorDAO.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }
}