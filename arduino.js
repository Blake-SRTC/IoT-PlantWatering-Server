const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");

const port = new SerialPort({ path: "COM3", baudRate: 9600 });
const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

port.on("open", function () {
    console.log("Conexion Abierta");
});

port.on("error", function (err) {
    console.log(err);
});

// parser.on("data", function (data) {
//     console.log(data);
// });

module.exports.parser = parser;
