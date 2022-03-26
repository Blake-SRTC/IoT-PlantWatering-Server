const express = require("express");
const router = express.Router();

router.post("/", (req, res) => {
    const { temp, hume } = req.query;
    if (temp && hume) {
        res.status(200).json({
            temp,
            hume,
        });
    } else {
        res.send("No existen parametros :v");
    }
});

module.exports = router;
