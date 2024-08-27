<?php

class CDatabasecon
{

    private $server_dsn;
    private $server_user;
    private $server_pass;
    public $conex_pgst;
    
    /**
	* Constructor	 	 
    * @param $dsn
    * @param $user
    * @param $pass
	*/
    function __construct($dsn, $user, $pass)
    {
        $this->server_dsn = $dsn;
        $this->server_user = $user;
        $this->server_pass = $pass;
        $this->conex_pgst = null;                 
    }    

    /**
	* Abre la conexion a la base de datos	 	 
	*/
    function dbConnect()
    {
        $this->conex_pgst = new PDO($this->server_dsn, $this->server_user, $this->server_pass, array(\PDO::ATTR_ERRMODE => \PDO::ERRMODE_EXCEPTION));
        //$this->$conex_pgst->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }

    /**
	* Cierra la conexion a la base de datos	 	 
	*/
    function dbClose()
    {
        $this->conex_pgst = null;
    }

    /**
	* Ejecuta el query en la db	 	 
    * @param $query
    * @param $array_param Cparam
	*/
    function querytoDb($query, $array_cparam = false)
    {
        $stament = null;
        $int_cont_param = 1;
        
        $stament = $this->conex_pgst->prepare($query);
		
        // si traemos parametros
        if ($array_cparam != false)
        {
            foreach ($array_cparam as $array_item)
            {
                $stament->bindParam($int_cont_param, $array_item->param_value, $array_item->param_type);
                $int_cont_param++;
            }
        }
		
        $stament->execute();           
        return $stament;             
    }
	
    /**
	* Ejecuta el query en la db	 	 
    * @param $query_stm     
	*/
    function query_fetch_column($query, $array_cparam = false)
    {
        $stament = null;
        $int_cont_param = 1;
		
        $ret_value = $this->conex_pgst->query($query)->fetchColumn();
        return $ret_value;
    }
}

/**
 * Objeto para crear tipos parametro
 */
class Cparam
{
    public $param_value;
    public $param_type;

    /**
	* Constructor	 	 
    * @param $param_value
    * @param $param_type     
	*/
    function Cparam($param_value, $param_type)
    {
        $this->param_value = $param_value;
        $this->param_type = $param_type;
    }
}

?>