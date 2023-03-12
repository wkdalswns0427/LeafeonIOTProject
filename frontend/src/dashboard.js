var url_get = 'http://localhost:8000/getLatestData'

function getLatestData(){

    fetch(url_get,
        {
            method : "GET"
        })
        .then((response) => response.json())
        .then((data) => console.log(data));
}