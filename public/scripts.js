var socket = io();

socket.on("dashOne", function (data) {
    document.getElementById("dash-1").src = data.Dash_1;
    document.getElementById("dash-2").src = data.Dash_2;
})

socket.on("data", function (data) {

    console.log(data);

    document.getElementById("temperature").innerHTML = data.Temperatura;
    document.getElementById("humidity").innerHTML = data.Humedad;
    document.getElementById("humidity_ground").innerHTML = data.Hume_suelo;
    document.getElementById("water").innerHTML = data.Agua;
});

socket.on("date", function (date) {
    document.getElementById("time").innerHTML = date.date
})