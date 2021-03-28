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

            document.getElementById("Accelerometer").textContent= data.acc;
            document.getElementById("Flex").textContent= data.flex;
            document.getElementById("RE").textContent= data.re;
            document.getElementById("Activity").textContent= data.activity;

    }
    getData();
}) 
}//getCounter

getCounter();
setInterval(getCounter, 2000);
