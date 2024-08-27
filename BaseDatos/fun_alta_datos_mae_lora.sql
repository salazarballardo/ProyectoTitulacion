-- =================================================================================
-- Autor: msalazar
-- Fecha: 2024-07-30
-- Descripción General: Funcion para insertar registros a la tabla mae_lora_parametros_sql
-- ================================================================================= 
create or replace function fun_alta_datos_mae_lora(
    in character varying,
    in character varying,
    in character varying,
    in character varying,
    in character varying)
  RETURNS void AS
$BODY$
DECLARE
    Latitud_param         ALIAS FOR $1;
    Longitud_param        ALIAS FOR $2;
	Velocidad_param       ALIAS FOR $3;
	Temperatura_param     ALIAS FOR $4;
	Humedad_param    	ALIAS FOR $5;          		   
BEGIN
    insert into mae_lora_parametros_sql (Latitud, Longitud, Velocidad, Temperatura, Humedad) 
	     values (Latitud_param, Longitud_param, Velocidad_param, Temperatura_param, Humedad_param);
END;
$BODY$
LANGUAGE plpgsql