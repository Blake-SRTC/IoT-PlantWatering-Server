const express = require("express");
const router = express.Router();
// Configuracion base de datos
const data = require("../models/shema");

// APIs
router.post("/", (req, res) => {
    console.log(req.body);

    res.send("Recibido");

    // Enviar a DB
    insertData(req.body)
        .then((datos_guardados) => console.log(datos_guardados))
        .catch((err) => console.error("Datos no validos"));
});

module.exports = router;

// Funcion repara datos y envia a DB
async function insertData(datos_guardar) {
    const ambiente = new data(datos_guardar);
    const datos_guardados = await ambiente.save();
    return datos_guardados;
}
