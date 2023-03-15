const url_get = 'http://localhost:8000/getLatestData'

async function updateSensorTable() {
    const response = await fetch(url_get);
    const sensorData = await response.json();

    const tableBody = document.createElement("tbody");
    for (const dataPoint of sensorData) {
        const row = tableBody.insertRow();
        row.insertCell().textContent = dataPoint.time;
        row.insertCell().textContent = dataPoint.tempdata;
        row.insertCell().textContent = dataPoint.humidata;
    }

    const sensorTable = document.getElementById("sensor-table");
    sensorTable.replaceChild(tableBody, sensorTable.tBodies[0]);
}

// Call the updateSensorTable function initially and every 5 seconds thereafter
updateSensorTable();
setInterval(updateSensorTable, 5000);