const express = require('express');
const bodyParser = require('body-parser');
const http = require('http');
const WebSocket = require('ws');
const fs = require('fs');

const app = express();
const PORT = 3000;

// Middleware to parse JSON bodies
app.use(bodyParser.json());

// Path to store RFID logs persistently
const LOG_FILE = './rfid-logs.json';

// Initialize logs file if it doesn't exist
if (!fs.existsSync(LOG_FILE)) {
  fs.writeFileSync(LOG_FILE, JSON.stringify([]));
}

// Load logs from file
const loadLogs = () => JSON.parse(fs.readFileSync(LOG_FILE, 'utf8'));

// Save logs to file
const saveLog = (log) => {
  const logs = loadLogs();
  logs.push(log);
  fs.writeFileSync(LOG_FILE, JSON.stringify(logs, null, 2));
};

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
app.post('/validate-card', (req, res) => {
  const { cardUID } = req.body;
  const validCard = "07C85486"; // Hardcoded valid card UID

  if (!cardUID) {
    const errorResponse = { success: false, message: "Card UID is required" };
    res.status(400).json(errorResponse);
    return;
  }

  // Prepare log entry
  const timestamp = new Date().toISOString();
  const status = cardUID === validCard ? "Valid" : "Invalid";
  const logEntry = { cardUID, status, timestamp };

  // Save log to persistent storage
  saveLog(logEntry);

  // Broadcast log entry to WebSocket clients
  broadcast(logEntry);

  // Respond to client
  if (status === "Valid") {
    res.status(200).json({ success: true, message: "Card is valid" });
  } else {
    res.status(403).json({ success: false, message: "Invalid card" });
  }
});

// GET endpoint to fetch all logs
app.get('/rfid-logs', (req, res) => {
  const logs = loadLogs();
  res.json(logs);
});

// Start server
server.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});
