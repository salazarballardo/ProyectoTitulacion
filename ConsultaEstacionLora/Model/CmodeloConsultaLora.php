<?php

class Cmodeloconsultaestacionlora
{

    /**
	 * Consulta en la db y valida las credenciales existen
	 * @param String user
	 * @return String pass	 
	 */
    function model_valida_credenciales_db($user, $pass)
    {
        include "Databasecon.php";        

        $query = "";
        $ret_existe = 0;
        $error = "";
        $flagerror = false;
        
        $params = parse_ini_file('database.ini');                // $dsn_db = "pgsql:host=$serv_db;port=$port_db;dbname=$database_db;";
        $dsn_db = sprintf("pgsql:host=%s;port=%s;dbname=%s;", $params["host"], $params["port"], $params["database"]);
        $databasecon = new CDatabasecon($dsn_db, $params["user"], $params["password"]);
        
        try
        {
            $databasecon->dbConnect();
            $query = sprintf("SELECT COUNT(*) registros FROM mae_credeenciales_consulta_lora WHERE usuariodb = '%s' AND passworddb = '%s'", $user, $pass);
            $ret_existe = $databasecon->query_fetch_column($query);
        }
        catch (PDOException $ex)
        {
            $flagerror = true;
            $error = "ocurrio un error : " . $ex->getMessage();
        }
        finally
        {
            $databasecon->dbClose();
        }
        
        $jsonresp = array("existe" => $ret_existe, "flagerror" => $flagerror, "error" => $error);
        return $jsonresp;
    }

    /**
	 * Consulta en la db y se trae los datos de la estacion
	 * @param String user
	 * @return String pass	 
	 */
    function model_consulta_datos_lora()
    {
        include "Databasecon.php";
        
        $error = "";
        $flagerror = false;
        $array_estacionresult = null;
        
        $params = parse_ini_file('database.ini');
        $dsn_db = sprintf("pgsql:host=%s;port=%s;dbname=%s;", $params["host"], $params["port"], $params["database"]);
        $databasecon = new CDatabasecon($dsn_db, $params["user"], $params["password"]);
        
        try
        {
            $databasecon->dbConnect();            
            $result_query = $databasecon->querytoDb("SELECT Latitud, Longitud, Velocidad, Temperatura, Humedad, FechaActualizacion FROM mae_lora_parametros_sql ORDER BY FechaActualizacion;");
            $resultado = $result_query->fetchObject();
            $array_estacionresult = array("latitud" => $resultado->latitud, "longitud" => $resultado->longitud, "velocidad" => $resultado->velocidad, "temperatura" => $resultado->temperatura, 
                "humedad" => $resultado->humedad, "fechaactualizacion" => $resultado->fechaactualizacion);            
        }
        catch (PDOException $ex) 
        {
            $flagerror = true;
            $error = "ocurrio un error : " . $ex->getMessage();
        }
        finally
        {
            $databasecon->dbClose();
        }

        $jsonresp = array("reporte_estacion_lora" => json_encode($array_estacionresult), "flagerror" => $flagerror, "error" => $error);
        return $jsonresp;
    }
}

?>