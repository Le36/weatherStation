const char graphPage[] PROGMEM = R"=====(
<!doctype html>
<html>
<head>
  <title>Live Chart | Weather Station</title>
  <script src = "Chart.min.js"></script>
  <style>
  canvas{
    -moz-user-select: none;
    -webkit-user-select: none;
    -ms-user-select: none;
  }

  /* Data Table Styling */
  #dataTable {
    font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
    border-collapse: collapse;
    width: 100%;
    color: white;
  }

  #dataTable td, #dataTable th {
    border: 1px solid #ddd;
    padding: 8px;
    color: white;
  }

  #dataTable tr:nth-child(even){background-color: #515151; color: white;}

  #dataTable tr:hover {background-color: #515151; color: white;}

  #dataTable th {
    padding-top: 12px;
    padding-bottom: 12px;
    text-align: left;
    background-color: #515151;
    color: white;
  }
  </style>
</head>

<body>
    <div style="background-color: #333333;"</div>
    <div style="text-align:center; color: white;"><b>Weather Station</b><br>Refresh rate is 5 seconds</div>
    <div class="chart-container" position: relative; height:150px; width:100%"></div>
    <canvas id="Chart" width="400" height="300"></canvas>
    <body style="background-color:#333333;">
<div>
  <table id="dataTable">
    <tr><th>Time</th><th>Temperature1 (&deg;C)</th><th>Temperature2 (&deg;C)</th><th>Humidity1 (%)</th><th>Humidity2 (%)</th><th>Pressure (Pa)</th></tr>
  </table>
</div>
<br>
<br>

<script>
//Graphs visit: https://www.chartjs.org
var T1values = [];
var T2values = [];
var H1values = [];
var H2values = [];
var timeStamp = [];
function showGraph()
{
    var ctx = document.getElementById("Chart").getContext('2d');
    var Chart2 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timeStamp,  //Bottom Labeling
            datasets: [{
                label: "Temperature1",
                fill: false,  //Try with true
                backgroundColor: 'rgba(255, 28, 28, 1)', //Dot marker color
                borderColor: 'rgba( 255, 28, 28, 1)', //Graph Line Color
                data: T1values,
            },{
                label: "Temperature2",
                fill: false,  //Try with true
                backgroundColor: 'rgba(255, 110, 0, 1)', //Dot marker color
                borderColor: 'rgba( 255, 110, 0 , 1)', //Graph Line Color
                data: T2values,
            },{
                label: "Humidity1",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 0, 97, 255 , 1)', //Dot marker color
                borderColor: 'rgba( 0, 97, 255 , 1)', //Graph Line Color
                data: H1values,
            },{
                label: "Humidity2",
                fill: false,  //Try with true
                backgroundColor: 'rgba(0, 187, 255, 1)', //Dot marker color
                borderColor: 'rgba(0, 187, 255 , 1)', //Graph Line Color
                data: H2values,
            }],
        },
        options: {
            title: {
                    display: false,
                    text: "Weather Station"
                },
                legend: {
                    position: 'top',
                    labels: {
                      fontColor: 'white'
                    }
             },
            maintainAspectRatio: true,
            elements: {
              line: {
                      tension: 0.5 //Smoothening (Curved) of data lines
                  }
            },
            scales: {
                    yAxes: [{
                        gridLines: {
                          color: 'rgba(255, 255, 255, 0.4)'
                        },
                        ticks: {
                            beginAtZero:false,
                            fontColor: 'white'
                        }
                    }],
                    xAxes: [{
                        gridLines: {
                          color: 'rgba(255, 255, 255, 0.4)'
                        },
                        ticks: {
                            fontColor: 'white'
                        }
                    }]
            }
        }
    });

}

//On Page load show graphs
window.onload = function() {
  console.log(new Date().toLocaleTimeString('it-IT'));
};



setInterval(function() {
  // Call a function repetatively with 5 Second interval
  getData();
}, 5000); //5000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
  var time = new Date().toLocaleTimeString('it-IT');
  var txt = this.responseText;
  var obj = JSON.parse(txt); //Ref: https://www.w3schools.com/js/js_json_parse.asp
      T1values.push(obj.temp1);
      T2values.push(obj.temp2);
      H1values.push(obj.hum1);
      H2values.push(obj.hum2);
      timeStamp.push(time);
      showGraph();  //Update Graphs
  //Update Data Table
    var table = document.getElementById("dataTable");
    var row = table.insertRow(1); //Add after headings
    var cell1 = row.insertCell(0);
    var cell2 = row.insertCell(1);
    var cell3 = row.insertCell(2);
    var cell4 = row.insertCell(3);
    var cell5 = row.insertCell(4);
    var cell6 = row.insertCell(5);
    cell1.innerHTML = time;
    cell2.innerHTML = obj.temp1;
    cell3.innerHTML = obj.temp2;
    cell4.innerHTML = obj.hum1;
    cell5.innerHTML = obj.hum2;
    cell6.innerHTML = obj.pres2;
    }
  };
  xhttp.open("GET", "readData", true);
  xhttp.send();
}

</script>
</body>

</html>

)=====";
