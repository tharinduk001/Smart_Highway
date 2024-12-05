const express = require('express');
const bodyParser = require('body-parser');
const http = require('http');
const WebSocket = require('ws');
const mongoose = require('mongoose');

const app = express();
const PORT = process.env.PORT || 3000; // Use PORT from environment, default to 3000

// Middleware to parse JSON bodies
app.use(bodyParser.json());

// MongoDB connection
const MONGO_URI = process.env.MONGO_URI || 'mongodb://localhost:27017/rfid_logs'; // Use MONGO_URI from environment
mongoose.connect(MONGO_URI, { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() => console.log(`Connected to MongoDB at ${MONGO_URI}`))
  .catch(err => {
    console.error("MongoDB connection error:", err);
    process.exit(1); // Exit if MongoDB connection fails
  });

// Define Log Schema
const logSchema = new mongoose.Schema({
  cardUID: String,
  status: String,
  timestamp: { type: Date, default: Date.now },
});
const Log = mongoose.model('Log', logSchema);

// Create HTTP server and WebSocket server
const server = http.createServer(app);
const wss = new WebSocket.Server({ server });

// Function to broadcast messages to all WebSocket clients
const broadcast = (message) => {
  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      client.send(JSON.stringify(message));
    }
  });
};

// POST endpoint to validate RFID
app.post('/validate-card', async (req, res) => {
  const { cardUID } = req.body;
  const validCard = "07C85486"; // Hardcoded valid card UID

  if (!cardUID) {
    res.status(400).json({ success: false, message: "Card UID is required" });
    return;
  }

  const status = cardUID === validCard ? "Valid" : "Invalid";
  const logEntry = new Log({ cardUID, status });

  try {
    await logEntry.save(); // Save to MongoDB
    broadcast(logEntry); // Broadcast to WebSocket clients
    res.status(status === "Valid" ? 200 : 403).json({
      success: status === "Valid",
      message: status === "Valid" ? "Card is valid" : "Invalid card",
    });
  } catch (error) {
    console.error("Error saving log:", error);
    res.status(500).json({ success: false, message: "Internal server error" });
  }
});

// GET endpoint to fetch all logs
app.get('/rfid-logs', async (req, res) => {
  try {
    const logs = await Log.find().sort({ timestamp: -1 });
    res.json(logs);
  } catch (error) {
    console.error("Error fetching logs:", error);
    res.status(500).json({ success: false, message: "Internal server error" });
  }
});

// Start server
server.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
