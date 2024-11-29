const express = require('express');
const bodyParser = require('body-parser');

const app = express();
const PORT = 3000;

// Middleware to parse JSON bodies
app.use(bodyParser.json());

// Hardcoded valid RFID card
const validCard = "07C85486";

// POST endpoint to validate RFID
app.post('/validate-card', (req, res) => {
  const { cardUID } = req.body;

  if (!cardUID) {
    return res.status(400).json({ success: false, message: "Card UID is required" });
  }

  if (cardUID === validCard) {
    return res.status(200).json({ success: true, message: "Card is valid" });
  } else {
    return res.status(403).json({ success: false, message: "Invalid card" });
  }
});

// Start the server
app.listen(PORT, () => {
  console.log(`RFID Validation API running on http://localhost:${PORT}`);
});
