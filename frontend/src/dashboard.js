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
        row.insertCell().textContent = dataPoint.presdata;
        row.insertCell().textContent = dataPoint.altdata;
        row.insertCell().textContent = dataPoint.eco2data;
        row.insertCell().textContent = dataPoint.tvocdata;
        row.insertCell().textContent = dataPoint.pm01data;
        row.insertCell().textContent = dataPoint.pm25data;
        row.insertCell().textContent = dataPoint.pm10data;
    }
    console.log(sensorData)

    const sensorTable = document.getElementById("sensor-table");
    sensorTable.replaceChild(tableBody, sensorTable.tBodies[0]);
}

// Call the updateSensorTable function initially and every 5 seconds thereafter
updateSensorTable();
setInterval(updateSensorTable, 5000);