const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

const path = require("path");
const router = express.Router();

// Configuracion base de datos
require("./db");
// Modelos para DB
const data = require("../models/shema");

// Middleware para usar otras caracteristicas de json
app.use(express.json());

// Variables de cache de datos
let temp = 0;
let hume = 0;
let hume_sue = 0;
let agua = 0;

// Rutas API
app.use(express.static("public"));
app.use("/", router);

router.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "../public/index.html"));
});

router.post("/api/v1/ambiente", (req, res) => {
    console.log(req.body);
    res.send("Recibido");

    // Enviar a front
    temp = req.body.Temperatura;
    hume = req.body.Humedad;
    hume_sue = req.body.Hume_suelo;
    agua = req.body.Agua;

    io.emit("data", { Temperatura: temp, Humedad: hume, Hume_suelo: hume_sue, Agua: agua });

    // Enviar a DB
    insertData(req.body)
        .then((datos_guardados) => console.log(datos_guardados))
        .catch((err) => console.error("Datos no validos"));
});

server.listen(4050, () => {
    console.log("listening on *:4050");
});

// Sockets tiempo real
io.on("connection", (socket) => {
    io.emit("data", { Temperatura: temp, Humedad: hume, Hume_suelo: hume_sue, Agua: agua });
    console.log("a user connected");
});

// ------------------------------------------------------------------

// Funcion repara datos y envia a DB
async function insertData(datos_guardar) {
    const ambiente = new data(datos_guardar);
    const datos_guardados = await ambiente.save();
    return datos_guardados;
}
