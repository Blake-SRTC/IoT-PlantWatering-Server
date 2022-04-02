const express = require('express');

const sensores = require('./ambiente.router');

function routerApi(app) {
    const router = express.Router();
    app.use('/api/v1/', router);
    router.use('/ambiente', sensores);
}

module.exports = routerApi;