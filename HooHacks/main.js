var api_url = "https://smartwidget-ae5c7-default-rtdb.firebaseio.com/Website/DeviceId1";
var counter_url ="https://smartwidget-ae5c7-default-rtdb.firebaseio.com/Website/ctr.json";
var link;

function getCounter(){    
    fetch(counter_url).then(response=>{
    return response.json();
    }).then(counter=>{
        console.log(counter);
for(var i = 1; i <= counter; i++){
    var str = i.toString();
    var slash = "/"
    link = api_url.concat(slash.concat(str.concat(".json")));

    async function getData(){
        const response = await fetch(link);
        const data =await response.json();

        const {acc, activity, flex, re, timestamp} = data;
        console.log(acc,activity,flex,re,timestamp);
        }
        getData();
    }//for loop
    return link;
}).then(final_url=>{
    async function getData(){
            const response = await fetch(final_url);
            const data =await response.json();

            document.getElementById("Accelerometer").textContent= Math.round(data.acc * 100) / 100;
            document.getElementById("Flex").textContent= Math.round(data.flex * 100) / 100;
            document.getElementById("RE").textContent= Math.round(data.re * 100) / 100;

            if (data.activity == 1){
                document.getElementById("Activity").textContent= "Low";
            } 

            if (data.activity == 2){
                document.getElementById("Activity").textContent= "Medium";
            } 

            if (data.activity == 3){
                document.getElementById("Activity").textContent= "High";
            } 
            document.getElementById("latest_time").textContent= data.timestamp;

    }
    getData();
}) 
}//getCounter

getCounter();
setInterval(getCounter, 2000);
