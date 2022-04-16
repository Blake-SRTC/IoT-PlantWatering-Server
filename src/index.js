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
    io.emit("data", req.body);

    // Enviar a DB
    insertData(req.body)
        .then((datos_guardados) => console.log(datos_guardados))
        .catch((err) => console.error("Datos no validos"));
});

server.listen(5000, () => {
    console.log("listening on *:5000");
});

// Sockets tiempo real
io.on("connection", (socket) => {
    console.log("a user connected");
});

// ------------------------------------------------------------------

// Funcion repara datos y envia a DB
async function insertData(datos_guardar) {
    const ambiente = new data(datos_guardar);
    const datos_guardados = await ambiente.save();
    return datos_guardados;
}
