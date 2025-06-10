#include "WebInterface.h"

// Global variables
WebServer server(SERVER_PORT);

void setupWebServer() {
  // API routes setup
  server.on("/", HTTP_GET, handleRoot);
  server.on("/data", HTTP_GET, handleData);
  server.on("/tests", HTTP_GET, handleTestsList);
  server.on("/download", HTTP_GET, handleDownload);
  server.on("/delete", HTTP_DELETE, handleDelete);
  server.on("/delete-all", HTTP_DELETE, handleDeleteAll);

  // Captive portal for other routes
  server.onNotFound(handleCaptivePortal);

  // Start web server
  server.begin();
  Serial.println("HTTP server started on port " + String(SERVER_PORT));
}

void handleClientRequests() { server.handleClient(); }

void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>OBD2 Scanner</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 0;
      color: #000;
      background-color: #f0f0f0;
    }
    h1 {
      background-color: #fff;
      color: #000;
      padding: 15px;
      margin: 0;
      font-size: 24px;
      border-bottom: 1px solid #ddd;
    }
    .status-bar {
      background-color: #eee;
      padding: 10px 15px;
      border-bottom: 1px solid #ddd;
    }
    .status-item {
      margin: 5px 0;
      display: inline-block;
      margin-right: 30px;
    }
    .label {
      font-weight: bold;
      display: inline-block;
    }
    .status-value {
      color: #000;
    }
    .status-connected {
      color: green;
    }
    .status-disconnected {
      color: red;
    }
    .tab-container {
      display: flex;
      border-bottom: 1px solid #d32f2f;
      background-color: #fff;
    }
    .tab {
      padding: 10px 20px;
      cursor: pointer;
    }
    .tab.active {
      color: black;
      border-bottom: 3px solid #d32f2f;
      font-weight: bold;
    }
    .content {
      padding: 15px;
    }
    .tab-content {
      display: none;
    }
    .tab-content.active {
      display: block;
    }
    .button {
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 5px 10px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 14px;
      margin: 4px 2px;
      cursor: pointer;
      border-radius: 4px;
    }
    .button:disabled {
      background-color: #cccccc;
      cursor: default;
    }
    table {
      border-collapse: collapse;
      width: 100%;
    }
    th, td {
      border: 1px solid #ddd;
      padding: 8px;
      text-align: left;
    }
    th {
      background-color: #f2f2f2;
    }
    tr:nth-child(even) {
      background-color: #f2f2f2;
    }

    /* Compact dashboard grid */
    .dashboard-grid {
      display: grid;
      grid-template-columns: repeat(auto-fill, minmax(170px, 1fr));
      gap: 10px;
      margin-top: 20px;
    }
    .gauge {
      text-align: center;
      border: 1px solid #ddd;
      border-radius: 10px;
      padding: 15px;
      background-color: #fff;
    }
    .gauge-label {
      font-size: 14px;
      margin-bottom: 8px;
      color: #000;
      font-weight: normal;
    }
    .gauge-value {
      font-size: 24px;
      font-weight: bold;
      margin: 5px 0;
    }
    .control-buttons {
      margin: 15px 0;
    }
  </style>
</head>
<body>
<h1>OBD2 Scanner</h1>

<div class="status-bar">
  <div class="status-item">
    <span class="label">Current Time:</span>
    <span id="currentTime" class="status-value">Loading...</span>
  </div>
  <div class="status-item">
    <span class="label">Auto:</span>
    <span id="autoStatus" class="status-value status-disconnected">UIT</span>
  </div>
  <div class="status-item">
    <span class="label">Logging:</span>
    <span id="loggingStatus" class="status-value status-disconnected">Inactief</span>
  </div>
</div>

<div class="tab-container">
  <div id="dashboardTab" class="tab active" onclick="switchTab('dashboard')">Dashboard</div>
  <div id="historieTab" class="tab" onclick="switchTab('historie')">History</div>
</div>

<div class="content">
  <div id="dashboard" class="tab-content active">
    <div class="status-info">
      <p><strong>Automatic Logging:</strong> <span id="loggingStatusText">Ready - Waiting for car to start</span></p>
    </div>

    <div class="dashboard-grid">
      <div class="gauge">
        <div class="gauge-label">Speed (km/h)</div>
        <div class="gauge-value" id="speedValue">0</div>
      </div>
      <div class="gauge">
        <div class="gauge-label">RPM</div>
        <div class="gauge-value" id="rpmValue">0</div>
      </div>
      <div class="gauge">
        <div class="gauge-label">Coolant Temp (°C)</div>
        <div class="gauge-value" id="tempValue">0</div>
      </div>
      <div class="gauge">
        <div class="gauge-label">Engine Load (%)</div>
        <div class="gauge-value" id="loadValue">0</div>
      </div>
      <div class="gauge">
        <div class="gauge-label">Throttle Position (%)</div>
        <div class="gauge-value" id="throttleValue">0</div>
      </div>
      <div class="gauge">
        <div class="gauge-label">Turbo Pressure (bar)</div>
        <div class="gauge-value" id="turboValue">0</div>
      </div>
    </div>
  </div>

  <div id="historie" class="tab-content">
    <div style="margin-bottom: 15px;">
      <button onclick="refreshTests()" class="button">Refresh</button>
      <button onclick="deleteAllFiles()" class="button" style="background-color: #f44336;">Delete All Logs</button>
    </div>

    <table>
      <thead>
        <tr>
          <th>File</th>
          <th>Date</th>
          <th>Size</th>
          <th>Actions</th>
        </tr>
      </thead>
      <tbody id="testsList">
        <tr>
          <td colspan="4" style="text-align: center; padding: 15px;">No test files found</td>
        </tr>
      </tbody>
    </table>
  </div>
</div>

<script>
  function switchTab(tabName) {
    document.querySelectorAll('.tab-content').forEach(content => {
      content.classList.remove('active');
    });

    document.querySelectorAll('.tab').forEach(tab => {
      tab.classList.remove('active');
    });

    document.getElementById(tabName).classList.add('active');
    document.getElementById(tabName + 'Tab').classList.add('active');
  }

  let lastData = {};
  let dataRefreshInterval = null;

  function updateUIWithData(data) {
    console.log("Ontvangen data:", data);
    lastData = data;

    document.getElementById('autoStatus').textContent = data.autoOn ? 'ON' : 'OFF';
    document.getElementById('autoStatus').className = 'status-value ' + (data.autoOn ? 'status-connected' : 'status-disconnected');

    document.getElementById('loggingStatus').textContent = data.logging ? 'Active' : 'Inactive';
    document.getElementById('loggingStatus').className = 'status-value ' + (data.logging ? 'status-connected' : 'status-disconnected');

    document.getElementById('currentTime').textContent = data.currentTime; // Update the current time

    // Update dashboard values with proper formatting
    document.getElementById('speedValue').textContent = Math.round(data.speed || 0);
    document.getElementById('rpmValue').textContent = Math.round(data.rpm || 0);
    document.getElementById('tempValue').textContent = data.coolantTemp ? data.coolantTemp.toFixed(1) : '0.0';
    document.getElementById('loadValue').textContent = data.engineLoad ? data.engineLoad.toFixed(1) : '0.0';
    document.getElementById('throttleValue').textContent = data.throttle ? data.throttle.toFixed(1) : '0.0';
    document.getElementById('turboValue').textContent = data.turboPress ? data.turboPress.toFixed(2) : '0.00';
  }

  function fetchData() {
    fetch('/data')
      .then(response => response.json())
      .then(data => {
        updateUIWithData(data);
      })
      .catch(error => {
        console.error('Error fetching data:', error);
      });
  }

  // Automatic logging - no manual controls needed

  function refreshTests() {
    fetch('/tests')
      .then(response => response.json())
      .then(data => {
        const testsList = document.getElementById('testsList');
        testsList.innerHTML = '';

        if (data.files && data.files.length > 0) {
          data.files.forEach(file => {
            const row = document.createElement('tr');

            const nameCell = document.createElement('td');
            nameCell.textContent = file.name;
            nameCell.style.padding = '8px';
            nameCell.style.border = '1px solid #ddd';

            const dateCell = document.createElement('td');
            dateCell.textContent = file.date;
            dateCell.style.padding = '8px';
            dateCell.style.border = '1px solid #ddd';

            const sizeCell = document.createElement('td');
            sizeCell.textContent = file.size;
            sizeCell.style.padding = '8px';
            sizeCell.style.border = '1px solid #ddd';

            const actionsCell = document.createElement('td');
            actionsCell.style.padding = '8px';
            actionsCell.style.border = '1px solid #ddd';

            const downloadCsvBtn = document.createElement('button');
            downloadCsvBtn.textContent = 'CSV';
            downloadCsvBtn.className = 'button';
            downloadCsvBtn.style.backgroundColor = '#2196F3';
            downloadCsvBtn.style.marginRight = '5px';
            downloadCsvBtn.style.fontSize = '12px';
            downloadCsvBtn.onclick = () => window.location.href = '/download?file=' + file.path + '&format=csv';

            const downloadLogBtn = document.createElement('button');
            downloadLogBtn.textContent = 'LOG';
            downloadLogBtn.className = 'button';
            downloadLogBtn.style.backgroundColor = '#ff9800';
            downloadLogBtn.style.marginRight = '5px';
            downloadLogBtn.style.fontSize = '12px';
            downloadLogBtn.onclick = () => window.location.href = '/download?file=' + file.path + '&format=log';

            const deleteBtn = document.createElement('button');
            deleteBtn.textContent = 'Delete';
            deleteBtn.className = 'button';
            deleteBtn.style.backgroundColor = '#f44336';
            deleteBtn.onclick = () => deleteFile(file.path);

            actionsCell.appendChild(downloadCsvBtn);
            actionsCell.appendChild(downloadLogBtn);
            actionsCell.appendChild(deleteBtn);

            row.appendChild(nameCell);
            row.appendChild(dateCell);
            row.appendChild(sizeCell);
            row.appendChild(actionsCell);

            testsList.appendChild(row);
          });
        } else {
          const row = document.createElement('tr');
          const cell = document.createElement('td');
          cell.colSpan = 4;
          cell.textContent = 'No files found';
          cell.style.textAlign = 'center';
          cell.style.padding = '15px';
          cell.style.border = '1px solid #ddd';
          row.appendChild(cell);
          testsList.appendChild(row);
        }
      })
      .catch(error => {
        console.error('Error loading tests:', error);
      });
  }

  function deleteFile(path) {
    if (confirm('Are you sure you wanna Delete this file? This cannot be undone!')) {
      fetch(`/delete?file=${path}`, { method: 'DELETE' })
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            refreshTests();
          } else {
            alert('Fout bij verwijderen bestand: ' + data.message);
          }
        })
        .catch(error => {
          console.error('Error deleting file:', error);
          alert('Fout bij verwijderen bestand');
        });
    }
  }

  function deleteAllFiles() {
    if (confirm('Are you sure you wanna Delete ALL files? This cannot be undone!')) {
      fetch('/delete-all', { method: 'DELETE' })
        .then(response => response.json())
        .then(data => {
          if (data.success) {
            alert(data.message);
            refreshTests();
          } else {
            alert('Fout bij verwijderen van alle bestanden: ' + data.message);
          }
        })
        .catch(error => {
          console.error('Error deleting all files:', error);
          alert('Fout bij verwijderen van alle bestanden');
        });
    }
  }

  document.addEventListener('DOMContentLoaded', function() {
    fetchData();
    dataRefreshInterval = setInterval(fetchData, 200); // Faster refresh for smoother animation
    refreshTests();

    // Initialize button states
    document.getElementById('stopTestButton').disabled = true;
  });
</script>
</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

void handleData() {
  DynamicJsonDocument jsonDoc(1024);
  
  extern bool autoIsOn;
  extern bool testMode;

  jsonDoc["autoOn"] = autoIsOn;
  jsonDoc["logging"] = isLogging;
  // Delete this line if you want to disable test mode
  jsonDoc["testMode"] = testMode;

  if (testMode) {
    float timestamp = millis() / 1000.0;
    float baseTime = timestamp;

    // Start from 0 km/h and gradually increase speed
    float testSpeed = 30 + (sin(baseTime * 0.3) * 40);
    float testRPM = 1000 + (sin(baseTime * 0.5) * 2000) + 1500;
    float testLoad = 20 + (sin(baseTime * 0.4) * 30);
    float testTemp = 90 + (sin(baseTime * 0.1) * 10);
    float testThrottle = 10 + (sin(baseTime * 0.6) * 40);
    float testTurboPress = 0.8 + (sin(baseTime * 0.7) * 0.6);

    jsonDoc["speed"] = (int)testSpeed;
    jsonDoc["rpm"] = (int)testRPM;
    jsonDoc["coolantTemp"] = testTemp;
    jsonDoc["engineLoad"] = testLoad;
    jsonDoc["throttle"] = testThrottle;
    jsonDoc["fuelLevel"] = 75.2; // Static for test
    jsonDoc["intakeTemp"] = 25 + sin(baseTime * 0.2) * 5;
    jsonDoc["oilTemp"] = 95 + sin(baseTime * 0.15) * 10;
    jsonDoc["turboPress"] = testTurboPress;
    jsonDoc["transTemp"] = 85 + sin(baseTime * 0.12) * 5;
  } // delete this else block if you want to disable test mode
  else {
    // Use real OBD2 data
    jsonDoc["speed"] = obd2Data.vehicleSpeed;            // Get current vehicle speed
    jsonDoc["rpm"] = obd2Data.engineRPM;                // Get current RPM
    jsonDoc["coolantTemp"] = obd2Data.coolantTemp;      // Get current coolant temperature
    jsonDoc["engineLoad"] = obd2Data.engineLoad;        // Get current engine load
    jsonDoc["throttle"] = obd2Data.throttlePosition;    // Get current throttle position
    jsonDoc["fuelLevel"] = obd2Data.fuelLevel;          // Get current fuel level
    jsonDoc["intakeTemp"] = obd2Data.intakeTemp;        // Get current intake temperature
    jsonDoc["oilTemp"] = obd2Data.oilTemp;              // Get current oil temperature
    jsonDoc["turboPress"] = obd2Data.turboPressure;     // Get current turbo pressure
    jsonDoc["transTemp"] = obd2Data.transmissionTemp;   // Get current transmission temperature
}

 // Get the current time and format it
  time_t now = time(nullptr);
  struct tm *timeinfo = localtime(&now);
  char buffer[20]; // Buffer for formatted time
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
  jsonDoc["currentTime"] = buffer; // Add formatted current time to the JSON response

  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleTestsList() {
  DynamicJsonDocument jsonDoc(4096);
  JsonArray files = jsonDoc.createNestedArray("files");

  Serial.println("Request received for test files list");

  // Try to open logs directory first
  File root = SPIFFS.open(LOG_DIR);
  int fileCount = 0;

  if (root && root.isDirectory()) {
    Serial.println("Successfully opened logs directory");
    File file = root.openNextFile();
    while (file) {
      String fileName = String(file.name());
      Serial.println("Found file: " + fileName);
      if (!file.isDirectory() && fileName.endsWith(".csv")) {
        fileCount++;
        Serial.println("CSV file found: " + fileName);

        JsonObject fileObj = files.createNestedObject();
        fileObj["name"] = fileName.substring(fileName.lastIndexOf('/') + 1);
        fileObj["path"] = fileName;
        fileObj["date"] = extractDateTime(fileName);
        fileObj["size"] = humanReadableSize(file.size());
      }
      file = root.openNextFile();
    }
  } else {
    Serial.println(
        "Cannot open logs directory as directory, trying alternative method");

    // Alternative method - scan all files in SPIFFS
    File allFiles = SPIFFS.open("/");
    if (allFiles) {
      File file = allFiles.openNextFile();
      while (file) {
        String filePath = String(file.path());
        Serial.println("Scanning file: " + filePath);
        if (filePath.startsWith(LOG_DIR) && filePath.endsWith(".csv") &&
            !file.isDirectory()) {
          fileCount++;
          Serial.println("CSV file found via alternative method: " + filePath);

          JsonObject fileObj = files.createNestedObject();
          fileObj["name"] = filePath.substring(filePath.lastIndexOf('/') + 1);
          fileObj["path"] = filePath;
          fileObj["date"] = extractDateTime(filePath);
          fileObj["size"] = humanReadableSize(file.size());
        }
        file = allFiles.openNextFile();
      }
    }
  }

  Serial.println("Total CSV files found: " + String(fileCount));

  String jsonResponse;
  serializeJson(jsonDoc, jsonResponse);
  Serial.println("JSON respons: " + jsonResponse);
  server.send(200, "application/json", jsonResponse);
}

void handleDownload() {
  String filename = server.arg("file");
  String format = server.arg("format");

  if (filename.isEmpty()) {
    server.send(400, "text/plain", "Missing file parameter");
    return;
  }

  if (!SPIFFS.exists(filename)) {
    server.send(404, "text/plain", "File not found");
    return;
  }

  File file = SPIFFS.open(filename, "r");
  if (!file) {
    server.send(500, "text/plain", "Failed to open file");
    return;
  }

  String downloadName = filename.substring(filename.lastIndexOf('/') + 1);

  // Change file extension based on format
  if (format == "log") {
    downloadName.replace(".csv", ".log");
    server.sendHeader("Content-Disposition",
                      "attachment; filename=" + downloadName);
    server.sendHeader("Content-Type", "text/plain");
    size_t sent = server.streamFile(file, "text/plain");
  } else {
    // Default to CSV format
    server.sendHeader("Content-Disposition",
                      "attachment; filename=" + downloadName);
    server.sendHeader("Content-Type", "text/csv");
    size_t sent = server.streamFile(file, "text/csv");
  }

  file.close();
}

void handleDelete() {
  String filename = server.arg("file");

  if (filename.isEmpty()) {
    server.send(400, "application/json",
                "{\"success\":false,\"message\":\"Missing file parameter\"}");
    return;
  }

  // Ensure the filename starts with / if it doesn't already
  if (!filename.startsWith("/")) {
    filename = "/" + filename;
  }

  Serial.println("Attempting to delete file: " + filename);

  if (!SPIFFS.exists(filename)) {
    Serial.println("File not found: " + filename);
    server.send(404, "application/json",
                "{\"success\":false,\"message\":\"File not found\"}");
    return;
  }

  if (SPIFFS.remove(filename)) {
    Serial.println("Successfully deleted file: " + filename);
    server.send(200, "application/json",
                "{\"success\":true,\"message\":\"File deleted\"}");
  } else {
    Serial.println("Failed to delete file: " + filename);
    server.send(500, "application/json",
                "{\"success\":false,\"message\":\"Failed to delete file\"}");
  }
}

void handleDeleteAll() {
  int count = 0;
  int attempts = 0;

  File root = SPIFFS.open("/");
  if (!root) {
    server.send(
        500, "application/json",
        "{\"success\":false,\"message\":\"Cannot access file system\"}");
    return;
  }

  // First, collect all CSV file names
  std::vector<String> csvFiles;
  File file = root.openNextFile();
  while (file) {
    String fileName = String(file.path());
    if (!file.isDirectory() && fileName.endsWith(".csv")) {
      csvFiles.push_back(fileName);
      Serial.println("Found CSV file to delete: " + fileName);
    }
    file = root.openNextFile();
  }
  root.close();

  // Now delete all collected CSV files
  for (const String &fileName : csvFiles) {
    attempts++;
    if (SPIFFS.remove(fileName)) {
      count++;
      Serial.println("Deleted: " + fileName);
    } else {
      Serial.println("Failed to delete: " + fileName);
    }
  }

  String response = "{\"success\":true,\"message\":\"" + String(count) +
                    " of " + String(attempts) + " files deleted\"}";
  server.send(200, "application/json", response);
}

void handleCaptivePortal() {
  server.sendHeader("Location",
                    "http://" + server.client().localIP().toString(), true);
  server.send(302, "text/plain", "");
}