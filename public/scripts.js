var socket = io();

socket.on("data", function (data) {

    console.log(data);

    document.getElementById("temperature").innerHTML = data.Temperatura;
    document.getElementById("humidity").innerHTML = data.Humedad;
});
