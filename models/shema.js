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
    date: {
        type: Date,
        default: function (){
            // Correccion zona horaria
            var utc = new Date();
            utc.setHours(utc.getHours() + -4);
            return utc;
        },
    },
    Hume_suelo: {
        type: Number,
        default: 0,
    },
    Agua: {
        type: Number,
        default: 0
    }
});

module.exports = model("metrica", iotShema);
