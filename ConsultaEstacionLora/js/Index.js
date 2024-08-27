/**
 * 
 */// File javascript Index.js

  /**
 * Al cargar la pagina
 */
document.addEventListener("DOMContentLoaded", function() {    
    // const btn_cargar = document.getElementById("btnCargar_id");    
    // eventos click
    // btn_cargar.addEventListener("click", evt_btnCargar_click);
    consultaDatosLoraEstacion();    
});

 /**
 * Consulta los datos grabados por la estacion
 */
function consultaDatosLoraEstacion()
{
    const url = "./Controller/ConsultaLoraController.php";

    fetch(url, {
		method: 'POST',
		body: JSON.stringify( { id_peticion : "consultadatoslora"} ),
		headers: { "Content-type": "application/json; charset=UTF-8" } 
		}).then(responseText => responseText.text())
        .then(textresponse => {
            console.log("respuesta consultadatoslora : " + textresponse);            
            const json_parse = JSON.parse(get_json_response(textresponse));
                
            if (!json_parse.flagerror)
            {                
                carga_datos_sku_form(JSON.parse(json_parse.reporte_estacion_lora));
            }            
            else
            {
                console.log(json_parse.error);
				Swal.fire({
					title: '',		       
					icon: 'error',
					confirmButtonColor: '#3D9EF9',
					html: 'Ocurri&oacute; un problema al obtener los datos del SKU'
				});
            }
        })
		.catch(error => console.log("error : " + error) );
}

 /**
 * carga los datos del json al formulario
 */
function carga_datos_sku_form(jsonresponse)
{
    console.log(jsonresponse.latitud);
    console.log(jsonresponse.longitud);
    console.log(jsonresponse.temperatura);
    console.log(jsonresponse.humedad);
    console.log(jsonresponse.fechaactualizacion);

    document.getElementById("lbl_lat_id").innerHTML = "Latitud : " + jsonresponse.latitud;
    document.getElementById("lbl_lon_id").innerHTML = "Longitud : " +jsonresponse.longitud;
    document.getElementById("lbl_temp_id").innerHTML = "Temperatura" + jsonresponse.temperatura;
    document.getElementById("lbl_hum_id").innerHTML = "Humedad : " + jsonresponse.humedad;
    document.getElementById("lbl_hora_act").innerHTML = "Fecha actualizacion : " + jsonresponse.fechaactualizacion;
    generaMapa(jsonresponse.latitud, jsonresponse.longitud);
}

 /**
 * Obtiene el json del str
 */
function get_json_response(strjson)
{    
    let flagPasollave = false;
    let strfinalret = "";  

    for (let indexstr = 0; indexstr < strjson.length; indexstr++)
    {              
        if (strjson[indexstr] == '{')
        {
            flagPasollave = true;            
        }

        if (flagPasollave)
        {
            strfinalret += strjson[indexstr];
        }
    }

    console.log("json:");
    console.log(strfinalret);
    return strfinalret;
}

 /**
 * Genera el mapa
 */
function generaMapa(coord_latitud, coord_longitud)
{
    var coord = {lat:parseFloat(coord_latitud) ,lng: parseFloat(coord_longitud)};
    var map = new google.maps.Map(document.getElementById('div_mapa'),{
      zoom: 8,
      center: coord
    });
    var marker = new google.maps.Marker({
      position: coord,
      map: map
    });
}