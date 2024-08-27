/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Class.java to edit this template
 */
package Datos;

/**
 *
 * @author msalazar
 */
public class Cconstantes 
{
    public static final int PORT = 12599;           // jdbc:postgresql://10.28.120.4:5432/tienda.0002         props
    public static String urlPosgrest = "jdbc:postgresql://localhost/sistemasmecatronica";
    public static String userBd = "usermecatronica1";
    public static String passBd = "1234";
    public static String fun_alta_param = "SELECT fun_alta_datos_mae_lora (?, ?, ?, ?, ?)";      // SELECT fun_alta_datos_mae_lora ('-107.3878200', '24.7903200', '12.00', '25.34', '42.5')
}