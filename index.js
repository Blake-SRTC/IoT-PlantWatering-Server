const express = require("express");
const app = express();
const http = require("http");
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

const routerApi = require("./routes/index");

app.get("/", (req, res) => {
    //res.sendFile(__dirname + '/index.html');
    res.send("Bienveinido");
});

// Middleware para usar otras caracteristicas de json
app.use(express.json());

routerApi(app);

// io.on('connection', (socket) => {
//   console.log('a user connected');
// });

server.listen(3000, () => {
    console.log("listening on *:3000");
});
