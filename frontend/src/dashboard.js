const url_get = 'http://localhost:8000/getLatestData'
var humidity;

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
        humidity = dataPoint.humidata;

        var tempGauge = createVerGauge('temp', -20, 60, ' °C').setVal(dataPoint.tempdata);
        var pm1_0Gauge = createVerGauge2('pm1_0', 0, 100, ' ug/m3').setVal(dataPoint.pm01data);
        var pm2_5Gauge = createVerGauge('pm2_5', 0, 76, ' ug/m3').setVal(dataPoint.pm25data);
        var pm10Gauge = createVerGauge('pm10_0', 0, 151, ' ug/m3').setVal(dataPoint.pm10data);
    }
    const sensorTable = document.getElementById("sensor-table");
    sensorTable.replaceChild(tableBody, sensorTable.tBodies[0]);
}

async function draw(max, classname, colorname){
  var i=1;
  var func1 = setInterval(function(){
    if(i<max){
      color1(i,classname,colorname);
      i++;
    } else{
      clearInterval(func1);
    }
  },
  10);
}

function color1(i, classname,colorname){
    $(classname).css({
         "background":"conic-gradient("+colorname+" 0% "+i+"%, #ffffff "+i+"% 100%)"
    });
}

async function update_pie_chart(max, classname, colorname){
  draw(humidity, '.pie-chart-humi', '#ccc');
}

// ------------------------------ dummy ----------------------------------------
function click_test()
{
  var inputTempVariable = document.getElementById('input-data').value;
  document.getElementById('current-temp').innerHTML = inputTempVariable;
}
// ------------------------------------------------------------------------------

// ------------------------------------------------------------------------------
//actual action part
$(window).ready(function(){
  updateSensorTable();
  draw(humidity, '.pie-chart-humi', '#ccc'); // humidity
});
// Call the updateSensorTable function initially and every 5 seconds thereafter
setInterval(updateSensorTable, 60000);
setInterval(update_pie_chart, 60000);
// ------------------------------------------------------------------------------