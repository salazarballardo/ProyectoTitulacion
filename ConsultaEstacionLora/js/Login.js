/**
 * 
 */// File javascript Login.js

  /**
 * Al cargar la pagina
 */
 document.addEventListener("DOMContentLoaded", function() {
    const btn_login = document.getElementById("btnLogin_id");    
    // eventos click
    btn_login.addEventListener("click", evt_btnLogin_click);
    document.getElementById("usuario_id").focus();
});    

/**
 * Evento click btn login
 */
function evt_btnLogin_click()
{
    // se realiza la peticion
    if (validaCampos())
    {
        consultaCredenciales();    
    }    
}

  /**
 * Se valida que se hayan llenado los campos
 */
function validaCampos()
{
    // input user
    if (document.getElementById("usuario_id").value === "")
    {                
        Swal.fire({
            title: '',		       
            icon: 'error',
            confirmButtonColor: '#3D9EF9',
            html: 'El campo usuario tiene formato incorrecto o esta vacio',
            didClose: () => { document.getElementById("usuario_id").value = ""; document.getElementById("usuario_id").focus(); }
        });
                
        return false;
    }

    // input pass
    if (document.getElementById("password_id").value === "")
    {                
        Swal.fire({
            title: '',		       
            icon: 'error',
            confirmButtonColor: '#3D9EF9',
            html: 'El campo password tiene formato incorrecto o esta vacio',
            didClose: () => { document.getElementById("password_id").value = ""; document.getElementById("password_id").focus(); }
        });
                
        return false;
    }

    return true;
}

  /**
 * Se validan las credenciales del usuario para ingresar
 */
function consultaCredenciales()
{
    const url = "./Controller/ConsultaLoraController.php";
    const usuario = document.getElementById("usuario_id").value.trim();
    const password = document.getElementById("password_id").value.trim();
    
    fetch(url, {
		method: 'POST',
		body: JSON.stringify( { id_peticion : "validacred", user : usuario, pass : password } ),
		headers: { "Content-type": "application/json; charset=UTF-8" } 
		}).then(responseText => responseText.text())
        .then(textresponse => {
            console.log("respuesta validaexiste : " + textresponse);                                    
            const json_parse = JSON.parse(textresponse.substring(textresponse.indexOf("{"), textresponse.length));
            
            if (json_parse.error === "")
            {
                if (json_parse.existe == "1")
                {
                    location.href ="Index.html";
                }
                else
                {
                    Swal.fire({
                        title: '',		       
                        icon: 'info',
                        confirmButtonColor: '#3D9EF9',
                        html: 'No tiene derechos para entrar a la pagina'
                    });                
                }
            }
            else
            {
                console.log(json_parse.error);
                Swal.fire({
                    title: '',		       
                    icon: 'error',
                    confirmButtonColor: '#3D9EF9',
                    html: 'Ocurri&oacute; un problema al validar las credenciales'
                });
            }
        })
		.catch(error => console.log("error : " + error) );
}