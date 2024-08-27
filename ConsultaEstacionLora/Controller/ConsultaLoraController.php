<?php

require_once "../Model/Credenciales.inc";
require_once "../Model/CmodeloConsultaLora.php";

header("Content-type: application/json; charset=utf-8");
$_POST=json_decode(file_get_contents('php://input'), true);

$id_peticion = $_POST["id_peticion"];

switch ($id_peticion)
{
    case "validacred":
        echo json_encode(controller_valida_credenciales($_POST["user"], $_POST["pass"]));
    break;
    case "consultadatoslora":
        echo json_encode(controller_consulta_datos_lora());
    break;
}

/**
 * Consulta las credenciales en el modelo
 *
 * @param String $user Usuario de la db
 * @param String $pass Password de la db
 * @author MSB 
 * @return String
 */ 
function controller_valida_credenciales($user, $pass)
{
    $Cmodelo = new Cmodeloconsultaestacionlora();
    return $Cmodelo->model_valida_credenciales_db($user, $pass);
}

/**
 * Consulta los datos enviados por la estacion
 *
 * @author MSB 
 * @return String
 */ 
function controller_consulta_datos_lora()
{        
    $Cmodelo = new Cmodeloconsultaestacionlora();
    return $Cmodelo->model_consulta_datos_lora();
}   

?>