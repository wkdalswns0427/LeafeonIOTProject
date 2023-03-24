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
        temperature = dataPoint.tempdata;
    }
    console.log(sensorData)

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
     },10);
 }
 function color1(i, classname,colorname){
    $(classname).css({
         "background":"conic-gradient("+colorname+" 0% "+i+"%, #ffffff "+i+"% 100%)"
    });
 }

$(window).ready(function(){
  updateSensorTable();
  draw(humidity, '.pie-chart-humi', '#ccc'); // humidity
  draw(temperature, '.pie-chart-temp', '#8b22ff'); //temp
//    draw(30, '.pie-chart3','#ff0');
});


 // Call the updateSensorTable function initially and every 5 seconds thereafter
// updateSensorTable();
setInterval(updateSensorTable, 5000);
setInterval(draw, 5000);