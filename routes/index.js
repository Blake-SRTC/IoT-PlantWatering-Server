const express = require('express');

const dht11 = require('./temp.hume.router');

function routerApi(app) {
    const router = express.Router();
    app.use('/api/v1/', router);
    router.use('/ambiente', dht11);
}

module.exports = routerApi;