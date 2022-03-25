const express = require('express');
const app = express();
const http = require('http');
const server = http.createServer(app);
const { Server } = require("socket.io");
const io = new Server(server);

// app.get('/', (req, res) => {
//   res.sendFile(__dirname + '/index.html');
// });

// io.on('connection', (socket) => {
//   console.log('a user connected');
// });

server.listen(3000, () => {
  console.log('listening on *:3000');
});

// Escucha Arduino
const { parser } = require("./arduino");

parser.on("data", function (data) {
    console.log(data);
});


