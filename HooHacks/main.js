// CODE DUMP FOR HTML FILES!!!

var api_url = "https://trialproject-55deb-default-rtdb.firebaseio.com/Devices/DeviceID1.json";
var counter_url ="https://trialproject-55deb-default-rtdb.firebaseio.com/CTR.json";

function getCounter(){    
    fetch(counter_url).then(response=>{
    return response.json();
    }).then(counter=>{
        

for(var i = 1; i <= counter; i++){
    console.log("hello");
}
// function fetchData(){    
// fetch(api_url).then(response=>{
// return response.json();
// }).then(answer=>{
//     console.log(answer);
//})
//})

//fetchData();






















// const api_url = "https://trialproject-55deb-default-rtdb.firebaseio.com/product.json";
// async function getData(){
//     const response = await fetch(api_url);
//     const data =await response.json();
//     //#1
//     document.getElementById("name").textContent= data.name;
//     document.getElementById("price").textContent= data.price;
//     document.getElementById("description").textContent= data.description;
//     //#2
//     const {name, description, price} = data; // const{name, desc, price}={x:maggie, y:12, z: noodle}

//     console.log(name, description,price);
//     makeChart(name, description, price);
// }

// getData();

// function makeChart(n, d, p){
//     //console.log(n,d,p);

//     var ctx = document.getElementById('chart').getContext('2d');
// var myChart = new Chart(ctx, {
//     type: 'bar',
//     data: {
//         labels: ['Red', 'Blue', 'Yellow', 'Green', 'Purple', 'Orange'],
//         datasets: [{
//             label: '# of Votes',
//             data: [name, desc, pri, 5, 2, 3],
//             backgroundColor: [
//                 'rgba(255, 99, 132, 0.2)',
//                 'rgba(54, 162, 235, 0.2)',
//                 'rgba(255, 206, 86, 0.2)',
//                 'rgba(75, 192, 192, 0.2)',
//                 'rgba(153, 102, 255, 0.2)',
//                 'rgba(255, 159, 64, 0.2)'
//             ],
//             borderColor: [
//                 'rgba(255, 99, 132, 1)',
//                 'rgba(54, 162, 235, 1)',
//                 'rgba(255, 206, 86, 1)',
//                 'rgba(75, 192, 192, 1)',
//                 'rgba(153, 102, 255, 1)',
//                 'rgba(255, 159, 64, 1)'
//             ],
//             borderWidth: 1
//         }]
//     },
//     options: {
//         scales: {
//             yAxes: [{
//                 ticks: {
//                     beginAtZero: true
//                 }
//             }]
//         }
//     }
// });

// }
