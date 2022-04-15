const mongoose = require("mongoose");

const URI =
    "mongodb+srv://blake:Mandrildenoche77@cluster0.ovida.mongodb.net/myFirstDatabase?retryWrites=true&w=majority";

mongoose
    //.connect("mongodb://mongo/iotdb")
    .connect(URI)
    .then((db) => console.log("DB is connected to: ", db.connection.host))
    .catch((err) => console.error(err));
