var express = require('express');
var bodyParser = require('body-parser');
var cron = require('node-cron');
var fs = require('fs');
var moment = require('moment-timezone');

var app = express();
const DB_FILE_PATH = './db/state.json';

app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

app.use(function (req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});


// Database File Methods
function getDbFile() {
  if (fs.existsSync(DB_FILE_PATH)) {
    let fileData = fs.readFileSync(DB_FILE_PATH).toString();
    let file = JSON.parse(fileData);
    return file;
  }
  else return null;
}

function updateDbFile(data) {
  let fileExists = fs.existsSync(DB_FILE_PATH);

  if (!fileExists) {
    fs.writeFileSync(DB_FILE_PATH, JSON.stringify({
      state: {
        centerLight: data.state.centerLight || false,
        sideLight: data.state.sideLight || false
      },
      turnOnTime: data.turnOnTime || null,
      daysOfWeek: data.daysOfWeek || ['SEG', 'TER', 'QUA', 'QUI', 'SEX', 'SAB', 'DOM']
    }))
  }
  else {
    let file = getDbFile();

    file.state = {
      centerLight: data.state.centerLight != undefined ? data.state.centerLight : file.state.centerLight,
      sideLight: data.state.sideLight != undefined ? data.state.sideLight : file.state.sideLight
    }
    file.turnOnTime = data.turnOnTime || file.turnOnTime;
    file.daysOfWeek = data.daysOfWeek || file.daysOfWeek;

    fs.writeFileSync(DB_FILE_PATH, JSON.stringify(file))
  }
}

// Endpoints
app.post('/lights', function (req, res) {
  const { centerLight, sideLight, daysOfWeek, turnOnTime } = req.body;

  try {
    updateDbFile({
      state: {
        centerLight: centerLight,
        sideLight: sideLight
      },
      daysOfWeek: daysOfWeek,
      turnOnTime: turnOnTime
    })
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }

  return res.status(200)
    .send("OK");
});

app.post('/lights/toggle', function (req, res) {
  try {
    let oldState = getDbFile().state;

    switch (oldState) {
      case 'off':
        updateDbFile({ state: 'full' })
        break;
      case 'half':
      case 'full':
        updateDbFile({ state: 'off' })
        break;
    }
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }

  return res.status(200)
    .send("OK");
});

app.get('/lights', function (req, res) {

  try {
    let file = getDbFile();
    return res.status(200)
      .json(file)
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }
});

app.get('/lights/state', function (req, res) {
  try {
    let file = getDbFile().state;
    let state = "default";

    if (file.centerLight)
      state = "center";

    if (file.sideLight)
      state = "half";

    if (file.centerLight && file.sideLight)
      state = "full";

    if (!file.centerLight && !file.sideLight)
      state = "off";

    return res.status(200)
      .json(state)
  }
  catch (error) {
    return res.status(500)
      .send(error);
  }
});

// Crons
cron.schedule('* * * * *', () => {
  try {
    const momentDate = moment().tz('America/Sao_Paulo').locale('pt');
    const dayOfWeek = momentDate.format('ddd').toUpperCase();
    const time = momentDate.format('HH:mm');

    const file = getDbFile();
    if (file && file.daysOfWeek && file.turnOnTime) {
      if (file.daysOfWeek.find(d => d == dayOfWeek) && file.turnOnTime == time) {
        file.state.centerLight = true;
        file.state.sideLight = true;
        updateDbFile(file);
      }
    }
  }
  catch (error) {
    console.log(error);
  }
})

// Start
app.listen(3000, function () {
  console.log('Felipe\'s Bedroom api listening on port 3000!');
});