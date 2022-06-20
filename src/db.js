const mongoose = require("mongoose");

const URI = process.env.URL;

mongoose
    //.connect("mongodb://mongo/iotdb") // Conexion local
    .connect(URI)
    .then((db) => console.log("DB is connected to: ", db.connection.host))
    .catch((err) => console.error(err));
