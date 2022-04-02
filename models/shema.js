const { Schema, model } = require("mongoose");

const iotShema = new Schema({
    Temperatura: {
        type: Number,
        default: 0,
    },
    Humedad: {
        type: Number,
        default: 0,
    },
    // hume_suelo: {
    //     type: Number,
    //     default: 00,
    // },
});

module.exports = model("ambiente", iotShema);
