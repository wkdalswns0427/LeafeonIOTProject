
require("dotenv").config()

function function1(){
    var dsc = document.getElementById("discount1");
    if(document.getElementById("esp1").checked == true){
        var carno = document.getElementById("carno1").value;
        console.log("on");
        fetch(url_on,
            {
                method : "POST",
                headers : {
                    "Content-Type" : "application/json",
                },
                body: JSON.stringify({
                name: "esp",
                number: 1,
                content: "avalon", 
                carnumber: carno, 
                }),
            })
            .then(type => type.json())
            .then(json => {
                console.log(json);
                if(json["discount"]==true){
                    console.log("discount!!")
                    dsc.style.display = 'block'
                }
            });
    } else{
        console.log("off");
        fetch(url_off,
            {
                method : "POST",
                headers : {
                    "Content-Type" : "application/json",
                },
                body: JSON.stringify({
                name: "esp",
                number: 1,
                content: "babilon",  
                }),
            }).then((response)=>console.log(response))
            .then(dsc.style.display = 'none');
    }
}

var check1 = $("input[id='esp1']");
check1.click(function(){
	$("p1").toggle();
});
