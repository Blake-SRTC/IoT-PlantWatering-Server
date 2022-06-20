const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

const path = require("path");
const router = express.Router();

// Puerto server, entorno o default
const port = process.env.PORT || 4070

// Dashboards
const dash_1 = process.env.DASHONE;
const dash_2 = process.env.DASHTWO;

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
let fecha = "0000-00-00 00:00:00";

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
        .then((datos_guardados) => {
            let fecha_iso = datos_guardados.date.toISOString();
            let fecha_new = fecha_iso.split("T");
            let tiempo = fecha_new[1].split(".");
            fecha = fecha_new[0] + " " + tiempo[0];
            
            // Socket ultimo registro
            io.emit("date", {date: fecha});
        })
        .catch((err) => console.error("Datos no validos"));
});

server.listen(port, () => {
    console.log(`listening on: ${port}`);
});

// Sockets tiempo real
io.on("connection", (socket) => {

    // Enviar Dashboard correspondiente
    io.emit("dashOne", {Dash_1: dash_1, Dash_2: dash_2});

    // Enviar datos iniciales Cache
    io.emit("data", { Temperatura: temp, Humedad: hume, Hume_suelo: hume_sue, Agua: agua });
    io.emit("date", {date: fecha});
    console.log("a user connected");
});

// ------------------------------------------------------------------

// Funcion repara datos y envia a DB
async function insertData(datos_guardar) {
    const ambiente = new data(datos_guardar);
    const datos_guardados = await ambiente.save();
    return datos_guardados;
}
